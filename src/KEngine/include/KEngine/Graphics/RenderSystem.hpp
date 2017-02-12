#pragma once

#include "KEngine/Common/Time.hpp"
#include "KEngine/Interface/IWindow.hpp"

#include <memory>

namespace ke
{

    /// <summary>
    /// 
    /// </summary>
    class RenderSystem
    {
    public:
        void prepareRenderCommands();
        void dispatchRenderCommands();
        void processRenderCommands(ke::Time elapsedTime);
        void render();

        void setWindow(ke::WindowSptr p_window) { this->window = p_window; }

    private:
        ke::WindowSptr window;
    };

    using RenderSystemUptr = std::unique_ptr<RenderSystem>;
    using RenderSystemSptr = std::shared_ptr<RenderSystem>;
    using RenderSystemWptr = std::weak_ptr<RenderSystem>;
    
}