#pragma once

#include "KEngine/Interfaces/IGraphicsCommandRenderer.hpp"

#include "KEngine/Graphics/SFML/SfmlHelper.hpp"

#include "KEngine/Graphics/Imgui.hpp"
#if defined(USE_SFML)
#include "imgui-SFML.h"
#include <SFML/Graphics/RenderWindow.hpp>
#endif

#include <algorithm>
#include <vector>
#include <cassert>

namespace ke
{

    class DearImguiRenderer : public IGraphicsCommandRenderer
    {
    public:

#if defined(USE_SFML)
        inline void setSfRenderWindow(sf::RenderWindow * p_renderTarget)
        {
            assert(p_renderTarget);
            this->renderTarget = p_renderTarget;
            ImGui::SFML::Init(*this->renderTarget);
        }
#endif

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

            for (auto & cmd : this->commands)
            {
                auto & info = std::get<ke::graphics::ImguiRenderInfo>(cmd.info);
                info.imguiDrawer();
            }

#if defined(USE_SFML)
            ImGui::SFML::Render(*this->renderTarget);
#endif
            this->drawCallCount++;

            this->flush();
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
#if defined(USE_SFML)
        sf::RenderWindow * renderTarget = nullptr;
#endif

        mutable size_t drawCallCount = 0;

        std::vector<GraphicsCommand> commands;
    };

}