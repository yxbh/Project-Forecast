#pragma once

#include "KEngine/Interfaces/IGraphicsCommandRenderer.hpp"
#include "KEngine/Graphics/SFML/SfmlHelper.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Texture.hpp>

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

            sf::Texture * currentTexture = nullptr;
            size_t currentTextureId = 0;
            this->drawCallCount = 0;
            for (const auto & command : this->commands)
            {
                // do culling
                const auto spriteWidth  = static_cast<float>(command.sprite.textureRect.width) * command.sprite.globalTransform.scaleX;
                const auto spriteHeight = static_cast<float>(command.sprite.textureRect.height) * command.sprite.globalTransform.scaleY;
                const sf::Vector2f sfPosition{ command.sprite.globalTransform.x, -command.sprite.globalTransform.y };
                sf::Vector2f topLeft{ sfPosition };
                auto maxGlobalBound = sf::FloatRect(topLeft, { spriteWidth , spriteHeight });

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
                        // if texture doesn't exist we don't draw it obviously.
                        continue;
                    }
                }
                sprite.setPosition(sfPosition);
                sprite.setScale(command.sprite.globalTransform.scaleX, command.sprite.globalTransform.scaleY);
                sf::IntRect textureRect;
                textureRect.top    = command.sprite.textureRect.top;
                textureRect.left   = command.sprite.textureRect.left;
                textureRect.width  = command.sprite.textureRect.width == 0 ? currentTexture->getSize().x : command.sprite.textureRect.width;
                textureRect.height = command.sprite.textureRect.height == 0 ? currentTexture->getSize().y : command.sprite.textureRect.height;
                sprite.setTextureRect(textureRect);
                sprite.setColor(ke::SfmlHelper::convert(command.sprite.color));

                renderTarget->draw(sprite);
                ++this->drawCallCount;

                if (this->drawBoundingBox)
                {
                    rectangleShape.setPosition(sfPosition);
                    rectangleShape.setSize({ spriteWidth, spriteHeight });
                    rectangleShape.setFillColor(sf::Color::Transparent);
                    rectangleShape.setOutlineColor(sf::Color::Green);
                    rectangleShape.setOutlineThickness(1.0f);
                    renderTarget->draw(rectangleShape);
                    ++this->drawCallCount;
                }
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

        bool drawBoundingBox = true;

        mutable size_t drawCallCount = 0;

        std::vector<GraphicsCommand> commands;

        TextureMapType * textureStore;
        sf::Sprite sprite;
        sf::RectangleShape rectangleShape;
    };

}