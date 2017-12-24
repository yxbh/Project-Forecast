#pragma once

#include "KEngine/Interfaces/IGraphicsCommandRenderer.hpp"
#include "KEngine/Graphics/SFML/SfmlHelper.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include <cassert>
#include <vector>

namespace ke
{

    class SpriteRenderer : public IGraphicsCommandRenderer
    {
    public:
        using TextureMapType = std::unordered_map<size_t, std::unique_ptr<sf::Texture>>;

        SpriteRenderer(TextureMapType* textureStore) : textureStore(textureStore) {}

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
            auto view = this->renderTarget->getView();
            auto viewCenter = view.getCenter();
            auto viewSize = view.getSize();
            auto viewTopLeft = sf::Vector2f(viewCenter.x - (viewSize.x) / 2, viewCenter.y - (viewSize.y) / 2);
            sf::FloatRect viewRect(viewTopLeft, viewSize);

            sf::Texture * currentTexture;
            size_t currentTextureId = 0;
            this->drawCallCount = 0;
            for (const auto & command : this->commands)
            {
                // do culling
                const auto shapeWidth  = static_cast<float>(command.sprite.textureRect.width);
                const auto shapeHeight = static_cast<float>(command.sprite.textureRect.height);
                const sf::Vector2f sfPosition{ command.sprite.globalTransform.x, -command.sprite.globalTransform.y };
                sf::Vector2f topLeft{ sfPosition };
                auto maxGlobalBound = sf::FloatRect(topLeft, { shapeWidth , shapeHeight });

                if (!viewRect.intersects(maxGlobalBound))
                    continue;

                // setup sprite for rendering.
                if (currentTextureId != command.sprite.textureId)
                {
                    currentTextureId = command.sprite.textureId;
                    if (this->textureStore->find(currentTextureId) != this->textureStore->end())
                    {
                        currentTexture = (*this->textureStore)[currentTextureId].get();
                        sprite.setTexture(*currentTexture);
                    }
                    else
                    {
                        continue;
                    }
                }
                sprite.setPosition(sfPosition);
                sprite.setScale(command.sprite.globalTransform.scaleX, command.sprite.globalTransform.scaleY);
                sf::IntRect textureRect;
                textureRect.top    = command.sprite.textureRect.bottom;
                textureRect.left   = command.sprite.textureRect.left;
                textureRect.width  = command.sprite.textureRect.width;
                textureRect.height = command.sprite.textureRect.height;
                sprite.setTextureRect(textureRect);
                const auto & color = command.sprite.color;
                sprite.setColor({ color.r, color.g, color.b, color.a });

                renderTarget->draw(sprite);
                ++this->drawCallCount;

                rectangleShape.setPosition(sfPosition);
                rectangleShape.setSize({ (float)command.sprite.textureRect.width, (float)command.sprite.textureRect.height });
                rectangleShape.setFillColor(sf::Color::Transparent);
                rectangleShape.setOutlineColor(sf::Color::Green);
                rectangleShape.setOutlineThickness(1.0f);
                renderTarget->draw(rectangleShape);
            }
        }

        virtual void flush() final
        {
            this->commands.clear();
        }

        virtual size_t getLastDrawCallCount() const final
        {
            return this->drawCallCount;
        }

    private:
        sf::RenderTarget * renderTarget;

        mutable size_t drawCallCount = 0;

        std::vector<GraphicsCommand> commands;

        TextureMapType * textureStore;
        sf::Sprite sprite;
        sf::RectangleShape rectangleShape;
    };

}