#pragma once

#include "KEngine/Graphics/GraphicsCommand.hpp"
#include "KEngine/Graphics/SFML/SfmlHelper.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/VertexArray.hpp>

#include <unordered_map>

namespace ke
{
    
    class CircleShapeRenderer
    {
    public:

        inline void setRenderTarget(sf::RenderTarget * p_renderTarget)
        {
            assert(p_renderTarget);
            this->renderTarget = p_renderTarget;
        }

        inline void render(const GraphicsCommand & command)
        {
            // do culling
            sf::Vector2f topLeft{ command.render.globalTransform.x, -command.render.globalTransform.y };
            const auto shapeWidth = command.render.radius * 2;
            topLeft.x -= shapeWidth;
            topLeft.y -= shapeWidth;
            auto shapeMaxGlobalBound = sf::FloatRect(topLeft, { shapeWidth * 2 , shapeWidth * 2 });
            auto view        = this->renderTarget->getView();
            auto viewCenter  = view.getCenter();
            auto viewSize    = view.getSize();
            auto viewTopLeft = sf::Vector2f(viewCenter.x - (viewSize.x) / 2, viewCenter.y - (viewSize.y) / 2);
            sf::FloatRect viewRect(viewTopLeft, viewSize);
            if (!viewRect.intersects(shapeMaxGlobalBound))
                return;

            const sf::Vector2f sfPosition{ command.render.globalTransform.x, -command.render.globalTransform.y };
            const auto sfOrigin       = ke::SfmlHelper::convert(command.render.origin);
            const auto sfFillColor    = ke::SfmlHelper::convert(command.render.fillColor);
            const auto sfOutlineColor = ke::SfmlHelper::convert(command.render.outlineColor);

            // get the shape from cache.
            auto it = shapeMap.find(command.render.id);
            sf::CircleShape * shapePtr = nullptr;
            if (it == shapeMap.end())
            {
                shape.setPosition(sfPosition);
                shape.setFillColor(sfFillColor);
                shape.setRadius(command.render.radius);
                shape.setOrigin(sfOrigin);
                shape.setOutlineColor(sfOutlineColor);
                shape.setOutlineThickness(command.render.outlineThickness);

                shapeMap[command.render.id] = shape;
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
            if (command.render.radius != shapePtr->getRadius())
            {
                shapePtr->setRadius(command.render.radius);
            }
            if (command.render.outlineThickness != shapePtr->getOutlineThickness())
            {
                shapePtr->setOutlineThickness(command.render.outlineThickness);
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

        inline size_t finaliseRender()
        {
            vertexArray.clear();
            auto tmpDrawCallCount = this->drawCallCount;
            this->drawCallCount = 0;
            return tmpDrawCallCount;
        }

    private:
        static constexpr const unsigned NUM_POINTS = 25;

        sf::RenderTarget * renderTarget;
        sf::VertexArray vertexArray;
        sf::CircleShape shape;

        std::unordered_map<SceneNodeId, sf::CircleShape> shapeMap; // cache

        size_t drawCallCount = 0;
    };

}