#include "KEngine/Graphics/RenderSystem.hpp"

#include "KEngine/Graphics/RenderLayer.hpp"

#include "KEngine/Common/Libs/concurrentqueue.h"
#include "KEngine/Common/Libs/readerwriterqueue.h"
#include "KEngine/Log/Log.hpp"

#include <SFML/Graphics.hpp>

#include <cassert>
#include <memory>
#include <queue>

namespace
{

    static ke::RenderSystem * instance = nullptr;

    std::unique_ptr<sf::Drawable> circleShape;

    static uint32_t drawCallCount = 0;

    template<typename Type>
    //using ConcurrentQueue = moodycamel::ConcurrentQueue<Type>;
    using ConcurrentQueue = moodycamel::ReaderWriterQueue<Type>;
    //using ConcurrentQueue = ke::ThreadSafeQueue<Type>;

    using GraphicsCommandList = std::vector<ke::GraphicsCommand>;

    static GraphicsCommandList currentCommandGenThreadCmdList;
    static GraphicsCommandList currentRenderThreadCmdList;
    static ConcurrentQueue<GraphicsCommandList> commandGenThreadCmdListQueue;
    static ConcurrentQueue<GraphicsCommandList> renderThreadCmdListQueue;

    static std::vector<ke::RenderLayer> renderLayers;
}

namespace ke
{

    RenderSystem::RenderSystem()
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
        return true;
    }

    void RenderSystem::shutdown()
    {
        circleShape.reset();
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
            ::currentCommandGenThreadCmdList.reserve(4096);
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

        // generate view command.
        assert(scene->getCameraNode());
        ::currentCommandGenThreadCmdList.push_back(scene->getCameraNode()->getGraphicsCommand());

        // generate command from scene tree.
        std::queue<ke::ISceneNode*> nodes;
        nodes.push(scene->getRootNode());
        while (nodes.size())
        {
            auto node = nodes.front();
            nodes.pop();
            ::currentCommandGenThreadCmdList.push_back(node->getGraphicsCommand());
            for (auto child : node->getChildren())
            {
                nodes.push(child.get());
            }
        }

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
        ::renderLayers.clear();
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

        // filter the commands to their respective processors.
        for (const auto & command : ::currentRenderThreadCmdList)
        {
            switch (command.type)
            {
            case GraphicsCommand::Types::SetViewContext:
            {
                sf::RenderWindow * renderTarget = static_cast<sf::RenderWindow*>(this->window.get()->get());
                auto view = renderTarget->getView();
                view.setCenter(static_cast<float>(command.view.transform.x), static_cast<float>(-command.view.transform.y));
                renderTarget->setView(view);
                break;
            }

            case GraphicsCommand::Types::RenderCircleShape:
            case GraphicsCommand::Types::RenderSquareShape:
            case GraphicsCommand::Types::RenderConvexShape:
            case GraphicsCommand::Types::RenderSprite:
            {
                while (command.render.depth >= ::renderLayers.size())
                {
                    ::renderLayers.push_back(ke::RenderLayer());
                }

                ::renderLayers[command.render.depth].graphicsCommands.push_back(command);
                break;
            }

            } // switch command type.
        } // for each command.
    }

    size_t RenderSystem::render()
    {
        if (::renderLayers.size() == 0)
        {
            return 0;
        }

        sf::RenderWindow * renderTarget = static_cast<sf::RenderWindow*>(this->window.get()->get());
        ::drawCallCount = 0;

        renderTarget->clear();
        for (const auto & layer : ::renderLayers)
        {
            for (const ke::GraphicsCommand & cmd : layer.graphicsCommands)
            {
                switch (cmd.type)
                {
                case ke::GraphicsCommand::Types::RenderCircleShape:
                {
                    if (!circleShape)
                    {
                        circleShape.reset(new sf::CircleShape);
                    }

                    auto * shape = static_cast<sf::CircleShape*>(circleShape.get());
                    shape->setPosition(cmd.render.globalTransform.x, cmd.render.globalTransform.y);
                    sf::Color sfFillColor(
                        cmd.render.fillColor.r,
                        cmd.render.fillColor.g,
                        cmd.render.fillColor.b,
                        cmd.render.fillColor.a);
                    shape->setFillColor(sfFillColor);
                    shape->setRadius(cmd.render.radius);
                    sf::Color sfOutlineColor(
                        cmd.render.outlineColor.r,
                        cmd.render.outlineColor.g,
                        cmd.render.outlineColor.b,
                        cmd.render.outlineColor.a);
                    shape->setOutlineColor(sfOutlineColor);
                    shape->setOutlineThickness(cmd.render.outlineThickness);
                    renderTarget->draw(*shape);
                    ++::drawCallCount;
                    break;
                }

                }
            }
        }

        renderTarget->display();
        ::commandGenThreadCmdListQueue.enqueue(std::move(::currentRenderThreadCmdList));

        return ::drawCallCount;
    }

}
