#pragma once

#include "KEngine/Graphics/GraphicsCommand.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/VertexArray.hpp>

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
            auto view = this->renderTarget->getView();
            auto viewCenter = view.getCenter();
            auto viewSize = view.getSize();
            auto viewTopLeft = sf::Vector2f(viewCenter.x - (viewSize.x) / 2, viewCenter.y - (viewSize.y) / 2);
            sf::FloatRect viewRect(viewTopLeft, viewSize);
            if (!viewRect.intersects(shapeMaxGlobalBound))
                return;

            shape.setPosition(command.render.globalTransform.x, -command.render.globalTransform.y);
            sf::Color sfFillColor(
                command.render.fillColor.r,
                command.render.fillColor.g,
                command.render.fillColor.b,
                command.render.fillColor.a);
            shape.setFillColor(sfFillColor);
            shape.setRadius(command.render.radius);
            shape.setOrigin(command.render.origin.x, command.render.origin.y);
            sf::Color sfOutlineColor(
                command.render.outlineColor.r,
                command.render.outlineColor.g,
                command.render.outlineColor.b,
                command.render.outlineColor.a);
            shape.setOutlineColor(sfOutlineColor);
            shape.setOutlineThickness(command.render.outlineThickness);

            renderTarget->draw(shape);
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

        size_t drawCallCount = 0;
    };

}