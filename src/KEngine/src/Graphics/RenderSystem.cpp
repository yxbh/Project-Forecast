#include "KEngine/Graphics/RenderSystem.hpp"

#include "KEngine/Interfaces/IEvent.hpp"
#include "KEngine/Interfaces/IGraphicsCommandRenderer.hpp"
#include "KEngine/Graphics/RenderLayer.hpp"
#include "KEngine/Graphics/DearImgui/ImguiRenderer.hpp"
#include "KEngine/Graphics/SFML/LineRenderer.hpp"
#include "KEngine/Graphics/SFML/CircleShapeRenderer.hpp"
#include "KEngine/Graphics/SFML/SpriteRenderer.hpp"

#include "KEngine/Common/Libs/concurrentqueue.h"
#pragma warning(push)
#pragma warning(disable:6255)
#pragma warning(disable:26495)
#include "KEngine/Common/Libs/readerwriterqueue.h"
#pragma warning(pop)
#include "KEngine/Core/EventManager.hpp"
#include "KEngine/Graphics/GraphicsCommand.hpp"
#include "KEngine/Events/AppEvents.hpp"
#include "KEngine/Events/OtherGraphicsEvents.hpp"
#include "KEngine/Common/Queues.hpp"
#include "KEngine/Log/Log.hpp"

#include <SFML/Graphics.hpp>

#include <cassert>
#include <cstdint>
#include <functional>
#include <memory>
#include <unordered_map>
#include <queue>

namespace
{

    static ke::RenderSystem * instance = nullptr;

    template<typename Type>
    //using ConcurrentQueue = moodycamel::ConcurrentQueue<Type>;
    using ConcurrentQueue = moodycamel::ReaderWriterQueue<Type>;
    //using ConcurrentQueue = ke::ThreadSafeQueue<Type>;

    using GraphicsCommandList = std::vector<ke::GraphicsCommand>;

    static bool graphicsCommandRenderTypeComparer(const ke::GraphicsCommand & lhs, const ke::GraphicsCommand & rhs)
    {
        return
            lhs.type < rhs.type;
    }

    static bool graphicsRenderCommandRenderTypeComparer(const ke::GraphicsCommand & lhs, const ke::GraphicsCommand & rhs)
    {
        return
            lhs.depth < rhs.depth;
    }

    /// <summary>
    /// Implementation detail of the KEngine default render system.
    /// </summary>
    class RenderSystemImpl : public ke::IRenderSystem
    {
        KE_DEFINE_SYSTEM_COMMON_PROPERTIES(RenderSystemImpl, 0x618CEC9E)

    public:
        using GraphicsCommandList = std::vector<ke::GraphicsCommand>;

        RenderSystemImpl()
        {
            this->clearColour = ke::Colour::WHITE;

            this->m_lineRenderer = std::make_unique<ke::LineRenderer>();
            this->m_circleShapeRenderer = std::make_unique<ke::CircleShapeRenderer>();
            this->m_spriteRenderer = std::make_unique<ke::SpriteRenderer>(&textureStore);
            this->m_imguiRenderer = std::make_unique<ke::DearImguiRenderer>();
        }
        virtual ~RenderSystemImpl() {};

        virtual bool initialise() override
        {
            this->orderedRenderCommandList.reserve(10240);
            ke::EventManager::registerListener<ke::TextureLoadViaFileRequestEvent>(this, &RenderSystemImpl::receiveEvent);
            ke::EventManager::registerListener<ke::TexturesBulkLoadViaFilesRequestEvent>(this, &RenderSystemImpl::receiveEvent);
            ke::EventManager::registerListener<ke::SetClearColourRequestEvent>(this, &RenderSystemImpl::receiveEvent);

            // do an initial broadcast of the window's size.
            ke::EventManager::enqueue(ke::makeEvent<ke::WindowResizedEvent>(this->window->getDimension()));

            return true;
        }

        virtual void shutdown() override
        {
            ke::EventManager::deregisterListener<ke::TextureLoadViaFileRequestEvent>(this, &RenderSystemImpl::receiveEvent);
            ke::EventManager::deregisterListener<ke::TexturesBulkLoadViaFilesRequestEvent>(this, &RenderSystemImpl::receiveEvent);
            ke::EventManager::deregisterListener<ke::SetClearColourRequestEvent>(this, &RenderSystemImpl::receiveEvent);
        }

        virtual void update(ke::Time elapsedTime) override
        {
            KE_UNUSED(elapsedTime);
        }

        virtual size_t prepareCommands(ke::Scene* scene) override
        {
            if (!scene) return 0; // ignore null scenes;

            if (commandGenThreadCmdListQueue.size_approx() == 0)
            {
                currentCommandGenThreadCmdList = GraphicsCommandList();
            }
            else
            {
                GraphicsCommandList throwAway;
                while (commandGenThreadCmdListQueue.size_approx() > 50)
                {
                    commandGenThreadCmdListQueue.try_dequeue(throwAway);
                }

                if (!commandGenThreadCmdListQueue.try_dequeue(currentCommandGenThreadCmdList))
                {
                    return 0;
                }
            }

            currentCommandGenThreadCmdList.clear();
            currentCommandGenThreadCmdList.reserve(8192);

            // Generate view command.
            auto cameraNode = scene->getCameraNode();
            assert(cameraNode);
            currentCommandGenThreadCmdList.push_back(cameraNode->getGraphicsCommand());

            // Generate command from scene tree.
            //
            // We use a recursive searcher to grab the graphics command minimise mem allocation
            // and keep most things on the stack to improve performance (especially in debug mode).
            std::function<void(ke::ISceneNode*)> graphicCmdGetter = [&](ke::ISceneNode * node)
            {
                currentCommandGenThreadCmdList.push_back(node->getGraphicsCommand());
                for (auto& child : node->getChildren())
                {
                    graphicCmdGetter(child.get());
                }
            };
            graphicCmdGetter(scene->getRootNode());

            return currentCommandGenThreadCmdList.size();
        }

        virtual size_t dispatchCommands() override
        {
            auto commandCount = currentCommandGenThreadCmdList.size();
            renderThreadCmdListQueue.enqueue(std::move(currentCommandGenThreadCmdList));
            return commandCount;
        }

        virtual void processCommands(ke::Time elapsedTime) override
        {
            KE_UNUSED(elapsedTime);
            if (renderThreadCmdListQueue.size_approx() == 0) return;

            GraphicsCommandList throwAway;
            while (renderThreadCmdListQueue.size_approx() > 2)
            {
                renderThreadCmdListQueue.try_dequeue(throwAway);
                //throwAway.~vector();
            }

            if (renderThreadCmdListQueue.size_approx() == 0) return;
            if (!renderThreadCmdListQueue.try_dequeue(currentRenderThreadCmdList))
            {
                return;
            }

            auto renderTarget = static_cast<sf::RenderWindow*>(this->window.get()->get());
            auto lineRenderer = static_cast<ke::LineRenderer*>(this->m_lineRenderer.get());
            auto circleShapeRenderer = static_cast<ke::CircleShapeRenderer*>(this->m_circleShapeRenderer.get());
            auto spriteRenderer = static_cast<ke::SpriteRenderer*>(this->m_spriteRenderer.get());
            lineRenderer->setRenderTarget(renderTarget);
            circleShapeRenderer->setRenderTarget(renderTarget);
            spriteRenderer->setRenderTarget(renderTarget);

            // filter the commands to their respective processors.
            this->orderedRenderCommandList.clear();
            this->orderedRenderCommandList.reserve(8192);
            for (const auto& command : currentRenderThreadCmdList)
            {
                switch (command.type)
                {
                case ke::GraphicsCommand::Types::SetViewContext:
                {
                    using SfFloatType = float;
                    auto view = renderTarget->getView();
                    auto& newViewInfo = std::get<ke::graphics::ViewContextInfo>(command.info);
                    view.setCenter(static_cast<SfFloatType>(newViewInfo.transform.x), static_cast<SfFloatType>(-newViewInfo.transform.y));
                    view.setSize(static_cast<SfFloatType>(newViewInfo.dimension.width), static_cast<SfFloatType>(newViewInfo.dimension.height));
                    renderTarget->setView(view);
                    break;
                }

                case ke::GraphicsCommand::Types::DrawLine:
                case ke::GraphicsCommand::Types::DrawCircleShape:
                case ke::GraphicsCommand::Types::DrawRectangleShape:
                case ke::GraphicsCommand::Types::DrawConvexShape:
                case ke::GraphicsCommand::Types::DrawSprite:
                case ke::GraphicsCommand::Types::DrawImgui:
                {
                    this->orderedRenderCommandList.push_back(command);
                    break;
                }

                } // switch command type.
            } // for each command.

            std::sort(this->orderedRenderCommandList.begin(), this->orderedRenderCommandList.end(),
                ::graphicsRenderCommandRenderTypeComparer);
        }

        virtual size_t render() override
        {
            auto renderTarget = static_cast<sf::RenderWindow * const>(this->window.get()->get());
            size_t drawCallCount = 0;

            if (this->clearFinalRenderTarget)
            {
                renderTarget->clear(ke::SfmlHelper::convert(this->clearColour));
            }

            if (this->orderedRenderCommandList.size() == 0)
            {
                return 0;
            }

            auto renderAndFlushAll = [&]()
            {
                this->m_lineRenderer->render();
                drawCallCount += this->m_lineRenderer->getLastDrawCallCount();
                this->m_lineRenderer->flush();

                this->m_circleShapeRenderer->render();
                drawCallCount += this->m_circleShapeRenderer->getLastDrawCallCount();
                this->m_circleShapeRenderer->flush();

                this->m_spriteRenderer->render();
                drawCallCount += this->m_spriteRenderer->getLastDrawCallCount();
                this->m_spriteRenderer->flush();
            };


            unsigned long currentDepthValue = this->orderedRenderCommandList[0].depth;
            auto lastDepthValue = currentDepthValue;
            for (const ke::GraphicsCommand& cmd : this->orderedRenderCommandList)
            {
                currentDepthValue = cmd.depth;
                if (lastDepthValue != currentDepthValue)
                {
                    renderAndFlushAll();
                    lastDepthValue = currentDepthValue;
                }

                switch (cmd.type)
                {
                case ke::GraphicsCommand::Types::DrawLine:
                {
                    this->m_lineRenderer->queueCommand(cmd);
                    break;
                }

                case ke::GraphicsCommand::Types::DrawCircleShape:
                {
                    this->m_circleShapeRenderer->queueCommand(cmd);
                    break;
                }

                case ke::GraphicsCommand::Types::DrawSprite:
                {
                    this->m_spriteRenderer->queueCommand(cmd);
                    break;
                }

                case ke::GraphicsCommand::Types::DrawImgui:
                {
                    this->m_imguiRenderer->queueCommand(cmd);
                    break;
                }

                default:
                {
                    ke::Log::instance()->critical("Unsupported graphics command type: {0:#x}", static_cast<std::uint16_t>(cmd.type));
                    assert(false);
                }
                }
            }
            renderAndFlushAll();

            // Only draw the GUI once and always last.
            this->m_imguiRenderer->render();
            drawCallCount += this->m_imguiRenderer->getLastDrawCallCount();
            this->m_imguiRenderer->flush();

            renderTarget->display();
            commandGenThreadCmdListQueue.enqueue(std::move(currentRenderThreadCmdList));

            return drawCallCount;
        }

        virtual void setWindow(ke::WindowSptr p_window) override
        {
            this->window = p_window;

            auto imguiRenderer = static_cast<ke::DearImguiRenderer*>(this->m_imguiRenderer.get());
            if (this->window)
            {
                auto renderTarget = static_cast<sf::RenderWindow*>(this->window.get()->get());
                imguiRenderer->setSfRenderWindow(renderTarget);
            }
            else
            {
                imguiRenderer->setSfRenderWindow(nullptr);
            }
        }

        void receiveEvent(ke::EventSptr event)
        {
            events.push(event);
        }

        void processEvents()
        {
            while (!this->events.isEmpty())
            {
                auto event = events.pop();
                switch (event->getType())
                {
                    //case ke::WindowResizedEvent::TYPE:
                    //{
                    //    auto resizedEvent = static_cast<ke::WindowResizedEvent*>(event.get());
                    //    const auto & newSize = resizedEvent->getNewSize();
                    //    sf::RenderWindow * sfWindow = static_cast<sf::RenderWindow*>(this->window.get()->get());
                    //    auto view = sfWindow->getView();
                    //    view.setSize({ static_cast<float>(newSize.width), static_cast<float>(newSize.height) });
                    //    sfWindow->setView(view);
                    //    break;
                    //}

                case ke::SetClearColourRequestEvent::TYPE:
                {
                    auto colourEvent = static_cast<ke::SetClearColourRequestEvent*>(event.get());
                    this->clearColour = colourEvent->getColour();
                    break;
                }

                case ke::TextureLoadViaFileRequestEvent::TYPE:
                {
                    auto textureLoadRequest = static_cast<ke::TextureLoadViaFileRequestEvent*>(event.get());
                    auto textureId = textureLoadRequest->getTextureId();
                    if (this->textureStore.find(textureId) == this->textureStore.end())
                    {
                        ke::Log::instance()->info("Loading texture {} from {}", textureLoadRequest->getTextureName(), textureLoadRequest->getTextureSourcePath());
                        auto texture = std::make_unique<sf::Texture>();
                        if (texture->loadFromFile(textureLoadRequest->getTextureSourcePath()))
                        {
                            this->textureStore.insert_or_assign(textureId, std::move(texture));
                        }
                        else
                        {
                            ke::Log::instance()->error("Texture loading failed for: {}", textureLoadRequest->getTextureSourcePath());
                        }
                    }
                    break;
                }

                case ke::TexturesBulkLoadViaFilesRequestEvent::TYPE:
                {
                    auto textureLoadRequest = static_cast<ke::TexturesBulkLoadViaFilesRequestEvent*>(event.get());
                    ke::Log::instance()->info("Loading {} textures ...", textureLoadRequest->getAllTextureInfo().size());
                    for (const auto& textureInfo : textureLoadRequest->getAllTextureInfo())
                    {
                        const auto& [textureName, textureId, textureSrcPath] = textureInfo;
                        if (this->textureStore.find(textureId) == this->textureStore.end())  // load if not in store.
                        {
                            ke::Log::instance()->info("Loading texture {}(id:{}) from {}", textureName, textureId, textureSrcPath);
                            auto texture = std::make_unique<sf::Texture>();
                            if (texture->loadFromFile(textureSrcPath))
                            {
                                this->textureStore.insert_or_assign(textureId, std::move(texture));
                            }
                            else
                            {
                                ke::Log::instance()->error("Texture loading failed for: {}", textureSrcPath);
                            }
                        }
                    }
                    break;
                }
                }
            }
        }

        virtual void updateOnRenderLoop(ke::Time elapsedTime) override
        {
            KE_UNUSED(elapsedTime);
            this->processEvents();
        }

    private:
        ke::WindowSptr window;

        ke::ThreadSafeQueue<ke::EventSptr> events;

        GraphicsCommandList orderedRenderCommandList;

        ke::GraphicsCommandRendererUptr m_lineRenderer;
        ke::GraphicsCommandRendererUptr m_circleShapeRenderer;
        ke::GraphicsCommandRendererUptr m_spriteRenderer;
        ke::GraphicsCommandRendererUptr m_imguiRenderer;

        ke::Colour clearColour;
        bool clearFinalRenderTarget = true;

        GraphicsCommandList currentCommandGenThreadCmdList;
        GraphicsCommandList currentRenderThreadCmdList;
        ConcurrentQueue<GraphicsCommandList> commandGenThreadCmdListQueue;
        ConcurrentQueue<GraphicsCommandList> renderThreadCmdListQueue;

        using TextureMapType = std::unordered_map<size_t, std::unique_ptr<sf::Texture>>;
        TextureMapType textureStore;
    };
}

namespace ke
{

    RenderSystem::RenderSystem()
        : impl(std::make_unique<::RenderSystemImpl>())
    {
        assert(!::instance);
        ::instance = this;
    }

    RenderSystem::~RenderSystem()
    {
        ::instance = nullptr;
    }

    bool RenderSystem::initialise()
    {
        return this->impl->initialise();
    }

    void RenderSystem::shutdown()
    {
        this->impl->shutdown();
    }

    void RenderSystem::update(ke::Time elapsedTime)
    {
        this->impl->update(elapsedTime);
    }

    size_t RenderSystem::prepareCommands(ke::Scene * scene)
    {
        return this->impl->prepareCommands(scene);
    }

    size_t RenderSystem::dispatchCommands()
    {
        return this->impl->dispatchCommands();
    }

    void RenderSystem::processCommands(ke::Time elapsedTime)
    {
        this->impl->processCommands(elapsedTime);
    }

    size_t RenderSystem::render()
    {
        return this->impl->render();
    }

    void RenderSystem::setWindow(ke::WindowSptr p_window)
    {
        this->impl->setWindow(p_window);
    }

    void RenderSystem::updateOnRenderLoop(ke::Time elapsedTime)
    {
        this->impl->updateOnRenderLoop(elapsedTime);
    }

}
