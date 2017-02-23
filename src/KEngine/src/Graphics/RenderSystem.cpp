#include "KEngine/Graphics/RenderSystem.hpp"

#include <SFML/Graphics.hpp>

#include <queue>

namespace
{
    ke::RenderSystem::GraphicsCommandList allCommands;
}

namespace ke
{

    void RenderSystem::prepareCommands(ke::Scene * scene)
    {
        if (!scene) return; // ignore null scenes;

        GraphicsCommandList & commands = this->logicThreadRenderCommandQueue;
        commands.reserve(4096);
        std::queue<ke::ISceneNode*> nodes;
        nodes.push(scene->getRootNode());
        while (nodes.size())
        {
            auto node = nodes.front();
            nodes.pop();
            commands.emplace_back(std::move(node->getGraphicsCommand()));
            for (auto child : node->getChildren())
            {
                nodes.push(child.get());
            }
        }

    }

    void RenderSystem::dispatchCommands()
    {
        GraphicsCommandList & commands = this->logicThreadRenderCommandQueue;
        this->renderThreadCommandLists.emplace(std::move(commands));
        commands.clear(); // ref: http://stackoverflow.com/questions/9168823/reusing-a-moved-container
    }

    void RenderSystem::processCommands(ke::Time elapsedTime)
    {
        auto allCommands = this->renderThreadCommandLists.pop();

        sf::RenderWindow * renderTarget = static_cast<sf::RenderWindow*>(this->window.get()->get());

        for (ke::GraphicsCommand & cmd : allCommands)
        {
            switch (cmd.type)
            {
            case ke::GraphicsCommand::Types::RenderCircleShape:
            {
                sf::CircleShape shape;
                shape.setPosition(cmd.render.globalTransform.x, cmd.render.globalTransform.y);
                sf::Color sfFillColor(
                    cmd.render.fillColor.r,
                    cmd.render.fillColor.g,
                    cmd.render.fillColor.b,
                    cmd.render.fillColor.a);
                shape.setFillColor(sfFillColor);
                shape.setRadius(cmd.render.radius);
                sf::Color sfOutlineColor(
                    cmd.render.outlineColor.r,
                    cmd.render.outlineColor.g,
                    cmd.render.outlineColor.b,
                    cmd.render.outlineColor.a);
                shape.setOutlineColor(sfOutlineColor);
                shape.setOutlineThickness(cmd.render.outlineThickness);
                renderTarget->draw(shape);
                break;
            }

            }
        }

    }

    void RenderSystem::render()
    {
        sf::RenderWindow * renderTarget = static_cast<sf::RenderWindow*>(this->window.get()->get());
        renderTarget->display();
    }

}