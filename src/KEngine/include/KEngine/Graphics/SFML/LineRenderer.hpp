#pragma once

#include "KEngine/Interfaces/IGraphicsCommandRenderer.hpp"

#include "KEngine/Graphics/SFML/SfmlHelper.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/VertexArray.hpp>

#include <algorithm>
#include <vector>
#include <cassert>

namespace ke
{

    class LineRenderer : public IGraphicsCommandRenderer
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
            this->drawCallCount = 0;

            if (this->commands.empty())
            {
                return;
            }

            this->vertexArray.clear();
            auto view = this->renderTarget->getView();
            auto viewCenter = view.getCenter();
            auto viewSize = view.getSize();
            auto viewTopLeft = sf::Vector2f(viewCenter.x - (viewSize.x) / 2, viewCenter.y - (viewSize.y) / 2);
            sf::FloatRect viewRect(viewTopLeft, viewSize);
            
            // do culling
            for (const auto & command : this->commands)
            {
                auto & lineInfo = std::get<ke::graphics::LineRenderInfo>(command.info);
                sf::Vector2f sfBegin{ lineInfo.begin.x, -lineInfo.begin.y };
                sf::Vector2f sfEnd{ lineInfo.end.x, -lineInfo.end.y };
                sf::Vector2f topLeft{ std::min(sfBegin.x, sfEnd.x), std::min(sfBegin.y, sfEnd.y) };
                const auto shapeWidth          = std::abs(std::max(sfBegin.x, sfEnd.x) - sfBegin.x);
                const auto shapeHeight         = std::abs(std::max(sfBegin.y, sfEnd.y) - sfBegin.y);
                const auto shapeMaxGlobalBound = sf::FloatRect(topLeft, { shapeWidth, shapeHeight });

                //if (!viewRect.intersects(shapeMaxGlobalBound))
                //    continue;
                
                this->vertexArray.emplace_back(sf::Vertex(sfBegin, SfmlHelper::convert(lineInfo.color)));
                this->vertexArray.emplace_back(sf::Vertex(sfEnd, SfmlHelper::convert(lineInfo.color)));
            }

            // do rendering.
            if (!this->vertexArray.empty())
            {
                renderTarget->draw(this->vertexArray.data(), this->vertexArray.size(), sf::PrimitiveType::Lines);
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
        std::vector<sf::Vertex> vertexArray;
        //sf::VertexArray vertexArray;

        mutable size_t drawCallCount = 0;

        std::vector<GraphicsCommand> commands;
    };

}