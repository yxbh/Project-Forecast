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

        SpriteRenderer(TextureMapType* textureStore)
            : textureStore(textureStore)
        {
            this->rectangleShape.setFillColor(sf::Color::Transparent);
            this->rectangleShape.setOutlineColor(sf::Color::Green);
            this->rectangleShape.setOutlineThickness(1.0f);
        }

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
                const auto & textureRect = command.sprite.textureRect;
                const auto & transform   = command.sprite.globalTransform;
                const auto spriteWidth   = static_cast<float>(textureRect.width) * transform.scaleX;
                const auto spriteHeight  = static_cast<float>(textureRect.height) * transform.scaleY;
                const sf::Vector2f sfPosition
                {
                    static_cast<float>(transform.x),
                    static_cast<float>(-transform.y)
                };
                sf::Vector2f topLeft{ sfPosition };
                auto maxGlobalBound = sf::FloatRect(topLeft, { spriteWidth , spriteHeight });

                // We cull the command if its texture rect is trivial and it's outside our view.
                if (!viewRect.intersects(maxGlobalBound) &&
                    textureRect.height != 0 && textureRect.width != 0)
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
                sprite.setScale(transform.scaleX, transform.scaleY);
                sf::IntRect sfTextureRect;
                sfTextureRect.top    = textureRect.top;
                sfTextureRect.left   = textureRect.left;
                sfTextureRect.width  = textureRect.width == 0 ? currentTexture->getSize().x : textureRect.width;
                sfTextureRect.height = textureRect.height == 0 ? currentTexture->getSize().y : textureRect.height;
                sprite.setTextureRect(sfTextureRect);
                sprite.setColor(ke::SfmlHelper::convert(command.sprite.color));

                this->renderTarget->draw(sprite);
                ++this->drawCallCount;

                if (this->drawBoundingBox)
                {
                    this->rectangleShape.setPosition(sfPosition);
                    if (const auto & size = this->rectangleShape.getSize(); size.x != spriteWidth && size.y != spriteHeight)
                    {
                        this->rectangleShape.setSize({ spriteWidth, spriteHeight });
                    }
                    this->renderTarget->draw(rectangleShape);
                    ++this->drawCallCount;
                }
            }
        }

        virtual void flush() final
        {
            this->commands.clear();
            this->commands.reserve(8192);
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