#include "KEngine/Graphics/RenderSystem.hpp"

#include "KEngine/Common/Libs/concurrentqueue.h"
#include "KEngine/Common/Libs/readerwriterqueue.h"

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
    static GraphicsCommandList currentRenderThreadmdList;
    static ConcurrentQueue<GraphicsCommandList> commandGenThreadCmdListQueue;
    static ConcurrentQueue<GraphicsCommandList> renderThreadCmdListQueue;
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
        GraphicsCommandList throwAway;
        while (::renderThreadCmdListQueue.size_approx() > 2)
        {
            ::renderThreadCmdListQueue.try_dequeue(throwAway);
            //throwAway.~vector();
        }
    }

    size_t RenderSystem::render()
    {
        if (::renderThreadCmdListQueue.size_approx() == 0) return 0;
        if (!::renderThreadCmdListQueue.try_dequeue(::currentRenderThreadmdList))
        {
            return 0;
        }

        sf::RenderWindow * renderTarget = static_cast<sf::RenderWindow*>(this->window.get()->get());
        ::drawCallCount = 0;

        renderTarget->clear();
        for (ke::GraphicsCommand & cmd : ::currentRenderThreadmdList)
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
        renderTarget->display();
        ::commandGenThreadCmdListQueue.enqueue(std::move(::currentRenderThreadmdList));

        return ::drawCallCount;
    }

}