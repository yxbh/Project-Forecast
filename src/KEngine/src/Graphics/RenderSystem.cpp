#include "KEngine/Graphics/RenderSystem.hpp"

#include "KEngine/Graphics/RenderLayer.hpp"
#include "KEngine/Graphics/SFML/LineRenderer.hpp"
#include "KEngine/Graphics/SFML/CircleShapeRenderer.hpp"
#include "KEngine/Graphics/SFML/SpriteRenderer.hpp"

#include "KEngine/Common/Libs/concurrentqueue.h"
#include "KEngine/Common/Libs/readerwriterqueue.h"
#include "KEngine/Core/EventManager.hpp"
#include "KEngine/Events/AppEvents.hpp"
#include "KEngine/Events/OtherGraphicsEvents.hpp"
#include "KEngine/Log/Log.hpp"

#include <SFML/Graphics.hpp>

#include <cassert>
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

    static GraphicsCommandList currentCommandGenThreadCmdList;
    static GraphicsCommandList currentRenderThreadCmdList;
    static ConcurrentQueue<GraphicsCommandList> commandGenThreadCmdListQueue;
    static ConcurrentQueue<GraphicsCommandList> renderThreadCmdListQueue;

    using TextureMapType = std::unordered_map<size_t, std::unique_ptr<sf::Texture>>;
    static TextureMapType TextureStore;


    static bool graphicsCommandRenderTypeComparer(const ke::GraphicsCommand & lhs, const ke::GraphicsCommand & rhs)
    {
        return
            lhs.type < rhs.type;
    }

    static bool graphicsRenderCommandRenderTypeComparer(const ke::GraphicsCommand & lhs, const ke::GraphicsCommand & rhs)
    {
        return
            lhs.render.depth < rhs.render.depth;
    }
}

namespace ke
{

    RenderSystem::RenderSystem()
    {
        assert(!::instance);
        ::instance = this;

        this->clearColour = ke::Colour::WHITE;

        this->m_lineRenderer        = std::make_unique<ke::LineRenderer>();
        this->m_circleShapeRenderer = std::make_unique<ke::CircleShapeRenderer>();
        this->m_spriteRenderer      = std::make_unique<ke::SpriteRenderer>(&TextureStore);
    }

    RenderSystem::~RenderSystem()
    {
        ::instance = nullptr;
    }

    bool RenderSystem::initialise()
    {
        this->orderedRenderCommandList.reserve(10240);
        //ke::EventManager::registerListener<ke::WindowResizedEvent>(this, &RenderSystem::receiveEvent);
        ke::EventManager::registerListener<ke::TextureLoadViaFileRequestEvent>(this, &RenderSystem::receiveEvent);
        ke::EventManager::registerListener<ke::TexturesBulkLoadViaFilesRequestEvent>(this, &RenderSystem::receiveEvent);
        ke::EventManager::registerListener<ke::SetClearColourRequestEvent>(this, &RenderSystem::receiveEvent);

        // do an initial broadcast of the window's size.
        ke::EventManager::enqueue(ke::makeEvent<ke::WindowResizedEvent>(this->window->getDimension()));

        return true;
    }

    void RenderSystem::shutdown()
    {
        //ke::EventManager::deregisterListener<ke::WindowResizedEvent>(this, &RenderSystem::receiveEvent);
        ke::EventManager::deregisterListener<ke::TextureLoadViaFileRequestEvent>(this, &RenderSystem::receiveEvent);
        ke::EventManager::deregisterListener<ke::TexturesBulkLoadViaFilesRequestEvent>(this, &RenderSystem::receiveEvent);
        ke::EventManager::deregisterListener<ke::SetClearColourRequestEvent>(this, &RenderSystem::receiveEvent);
    }

    void RenderSystem::update(ke::Time elapsedTime)
    {
        KE_UNUSED(elapsedTime);
    }

    size_t RenderSystem::prepareCommands(ke::Scene * scene)
    {
        if (!scene) return 0; // ignore null scenes;

        if (::commandGenThreadCmdListQueue.size_approx() == 0)
        {
            ::currentCommandGenThreadCmdList = GraphicsCommandList();
        }
        else
        {
            GraphicsCommandList throwAway;
            while (::commandGenThreadCmdListQueue.size_approx() > 50)
            {
                ::commandGenThreadCmdListQueue.try_dequeue(throwAway);
            }

            if (!::commandGenThreadCmdListQueue.try_dequeue(::currentCommandGenThreadCmdList))
            {
                return 0;
            }
        }

        ::currentCommandGenThreadCmdList.clear();
        ::currentCommandGenThreadCmdList.reserve(8192);

        // Generate view command.
        auto cameraNode = scene->getCameraNode();
        assert(cameraNode);
        ::currentCommandGenThreadCmdList.push_back(cameraNode->getGraphicsCommand());

        // Generate command from scene tree.
        //
        // We use a recursive searcher to grab the graphics command minimise mem allocation
        // and keep most things on the stack to improve performance (especially in debug mode).
        std::function<void(ke::ISceneNode *)> graphicCmdGetter = [&](ke::ISceneNode * node)
        {
            ::currentCommandGenThreadCmdList.push_back(node->getGraphicsCommand());
            for (auto child : node->getChildren())
            {
                graphicCmdGetter(child.get());
            }
        };
        graphicCmdGetter(scene->getRootNode());

        return ::currentCommandGenThreadCmdList.size();
    }

    size_t RenderSystem::dispatchCommands()
    {
        auto commandCount = ::currentCommandGenThreadCmdList.size();
        ::renderThreadCmdListQueue.enqueue(std::move(::currentCommandGenThreadCmdList));
        return commandCount;
    }

    void RenderSystem::processCommands(ke::Time elapsedTime)
    {
        KE_UNUSED(elapsedTime);
        if (::renderThreadCmdListQueue.size_approx() == 0) return;

        GraphicsCommandList throwAway;
        while (::renderThreadCmdListQueue.size_approx() > 2)
        {
            ::renderThreadCmdListQueue.try_dequeue(throwAway);
            //throwAway.~vector();
        }

        if (::renderThreadCmdListQueue.size_approx() == 0) return;
        if (!::renderThreadCmdListQueue.try_dequeue(::currentRenderThreadCmdList))
        {
            return;
        }

        auto renderTarget        = static_cast<sf::RenderWindow*>(this->window.get()->get());
        auto lineRenderer        = static_cast<ke::LineRenderer*>(this->m_lineRenderer.get());
        auto circleShapeRenderer = static_cast<ke::CircleShapeRenderer*>(this->m_circleShapeRenderer.get());
        auto spriteRenderer      = static_cast<ke::SpriteRenderer*>(this->m_spriteRenderer.get());
        lineRenderer->setRenderTarget(renderTarget);
        circleShapeRenderer->setRenderTarget(renderTarget);
        spriteRenderer->setRenderTarget(renderTarget);

        // filter the commands to their respective processors.
        this->orderedRenderCommandList.clear();
        this->orderedRenderCommandList.reserve(8192);
        for (const auto & command : ::currentRenderThreadCmdList)
        {
            switch (command.type)
            {
            case GraphicsCommand::Types::SetViewContext:
            {
                using SfFloatType = float;
                auto view = renderTarget->getView();
                view.setCenter(static_cast<SfFloatType>(command.view.transform.x), static_cast<SfFloatType>(-command.view.transform.y));
                view.setSize(static_cast<SfFloatType>(command.view.dimension.width), static_cast<SfFloatType>(command.view.dimension.height));
                renderTarget->setView(view);
                break;
            }

            case GraphicsCommand::Types::RenderLine:
            case GraphicsCommand::Types::RenderCircleShape:
            case GraphicsCommand::Types::RenderSquareShape:
            case GraphicsCommand::Types::RenderConvexShape:
            case GraphicsCommand::Types::RenderSprite:
            {
                this->orderedRenderCommandList.push_back(command);
                break;
            }

            } // switch command type.
        } // for each command.

        std::sort(this->orderedRenderCommandList.begin(), this->orderedRenderCommandList.end(),
            ::graphicsRenderCommandRenderTypeComparer);
    }

    size_t RenderSystem::render()
    {
        auto renderTarget = static_cast<sf::RenderWindow*const>(this->window.get()->get());
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


        unsigned long currentDepthValue = this->orderedRenderCommandList[0].render.depth;
        auto lastDepthValue = currentDepthValue;
        for (const ke::GraphicsCommand & cmd : this->orderedRenderCommandList)
        {
            switch (cmd.type)
            {
            case ke::GraphicsCommand::Types::RenderLine:
            {
                this->m_lineRenderer->queueCommand(cmd);
                break;
            }

            case ke::GraphicsCommand::Types::RenderCircleShape:
            {
                this->m_circleShapeRenderer->queueCommand(cmd);
                break;
            }

            case ke::GraphicsCommand::Types::RenderSprite:
            {
                this->m_spriteRenderer->queueCommand(cmd);
                break;
            }

            default:
            {
                ke::Log::instance()->critical("Unsupported graphics command type: {0:#x}", cmd.type);
                assert(!cmd.type);
            }
            }

            currentDepthValue = cmd.render.depth;
            if (lastDepthValue != currentDepthValue)
            {
                renderAndFlushAll();
                lastDepthValue = currentDepthValue;
            }
        }
        renderAndFlushAll();

        renderTarget->display();
        ::commandGenThreadCmdListQueue.enqueue(std::move(::currentRenderThreadCmdList));

        return drawCallCount;
    }

    void RenderSystem::updateOnRenderLoop(ke::Time elapsedTime)
    {
        KE_UNUSED(elapsedTime);
        this->processEvents();
    }

    void RenderSystem::receiveEvent(ke::EventSptr event)
    {
        events.push(event);
    }

    void RenderSystem::processEvents()
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
                if (::TextureStore.find(textureId) == ::TextureStore.end())
                {
                    ke::Log::instance()->info("Loading texture {} from {}", textureLoadRequest->getTextureName(), textureLoadRequest->getTextureSourcePath());
                    auto texture = std::make_unique<sf::Texture>();
                    if (texture->loadFromFile(textureLoadRequest->getTextureSourcePath()))
                    {
                        ::TextureStore.insert_or_assign(textureId, std::move(texture));
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
                for (const auto & textureInfo : textureLoadRequest->getAllTextureInfo())
                {
                    const auto & [textureName, textureId, textureSrcPath] = textureInfo;
                    if (::TextureStore.find(textureId) == ::TextureStore.end())  // load if not in store.
                    {
                        ke::Log::instance()->info("Loading texture {}(id:{}) from {}", textureName, textureId, textureSrcPath);
                        auto texture = std::make_unique<sf::Texture>();
                        if (texture->loadFromFile(textureSrcPath))
                        {
                            ::TextureStore.insert_or_assign(textureId, std::move(texture));
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

}
