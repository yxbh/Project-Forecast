#pragma once

#include "KEngine/Interfaces/IGraphicsCommandRenderer.hpp"
#include "KEngine/Graphics/SFML/SfmlHelper.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/VertexArray.hpp>

#include <unordered_map>

namespace ke
{
    
    class CircleShapeRenderer : public IGraphicsCommandRenderer
    {
    public:

        inline void setRenderTarget(sf::RenderTarget * p_renderTarget)
        {
            assert(p_renderTarget);
            this->renderTarget = p_renderTarget;
        }

        virtual void queueCommand(const GraphicsCommand & command) final
        {
            this->commands.push_back(command);
        }

        virtual void render() final
        {
            auto view        = this->renderTarget->getView();
            auto viewCenter  = view.getCenter();
            auto viewSize    = view.getSize();
            auto viewTopLeft = sf::Vector2f(viewCenter.x - (viewSize.x) / 2, viewCenter.y - (viewSize.y) / 2);
            sf::FloatRect viewRect(viewTopLeft, viewSize);

            this->drawCallCount = 0;
            for (const auto & command : this->commands)
            {
                // do culling
                sf::Vector2f topLeft{ command.shape.globalTransform.x, -command.shape.globalTransform.y };
                const auto shapeWidth = command.shape.radius * 2;
                topLeft.x -= shapeWidth;
                topLeft.y -= shapeWidth;
                auto shapeMaxGlobalBound = sf::FloatRect(topLeft, { shapeWidth * 2 , shapeWidth * 2 });

                if (!viewRect.intersects(shapeMaxGlobalBound))
                    continue;

                const sf::Vector2f sfPosition{ command.shape.globalTransform.x, -command.shape.globalTransform.y };
                const auto sfOrigin = ke::SfmlHelper::convert(command.shape.origin);
                const auto sfFillColor = ke::SfmlHelper::convert(command.shape.fillColor);
                const auto sfOutlineColor = ke::SfmlHelper::convert(command.shape.outlineColor);

                // get the shape from cache.
                auto it = shapeMap.find(command.shape.id);
                sf::CircleShape * shapePtr = nullptr;
                if (it == shapeMap.end())
                {
                    shape.setPosition(sfPosition);
                    shape.setFillColor(sfFillColor);
                    shape.setRadius(command.shape.radius);
                    shape.setOrigin(sfOrigin);
                    shape.setOutlineColor(sfOutlineColor);
                    shape.setOutlineThickness(command.shape.outlineThickness);

                    shapeMap[command.shape.id] = shape;
                    shapePtr = &shape;
                }
                else
                {
                    shapePtr = &(it->second);
                }

                // update states
                if (sfPosition != shapePtr->getPosition())
                {
                    shapePtr->setPosition(sfPosition);
                }
                if (sfOrigin != shapePtr->getOrigin())
                {
                    shapePtr->setOrigin(sfOrigin);
                }
                if (command.shape.radius != shapePtr->getRadius())
                {
                    shapePtr->setRadius(command.shape.radius);
                }
                if (command.shape.outlineThickness != shapePtr->getOutlineThickness())
                {
                    shapePtr->setOutlineThickness(command.shape.outlineThickness);
                }
                if (sfFillColor != shapePtr->getFillColor())
                {
                    shapePtr->setFillColor(sfFillColor);
                }
                if (sfOutlineColor != shapePtr->getOutlineColor())
                {
                    shapePtr->setOutlineColor(sfOutlineColor);
                }

                // do rendering.
                renderTarget->draw(*shapePtr);
                ++this->drawCallCount;
            }            
        }

        virtual void flush() final
        {
            this->commands.clear();
            vertexArray.clear();
        }

        virtual size_t getLastDrawCallCount() const final
        {
            return this->drawCallCount;
        }

    private:
        static constexpr const unsigned NUM_POINTS = 25;

        sf::RenderTarget * renderTarget;
        sf::VertexArray vertexArray;
        sf::CircleShape shape;

        std::unordered_map<SceneNodeId, sf::CircleShape> shapeMap; // cache

        mutable size_t drawCallCount = 0;

        std::vector<GraphicsCommand> commands;
    };

}