#pragma once

#include "KEngine/Common/Time.hpp"
#include "KEngine/Interface/IWindow.hpp"
#include "KEngine/Graphics/Scene.hpp"

#include <memory>

namespace ke
{

    /// <summary>
    /// 
    /// </summary>
    class RenderSystem
    {
    public:
        /// <summary>
        /// Generate render commands from the given scene when it is not null.
        /// </summary>
        /// <param name="scene"></param>
        void prepareRenderCommands(ke::Scene * scene);

        /// <summary>
        /// Prepare all the render commands generated so far for processing.
        /// </summary>
        void dispatchRenderCommands();

        /// <summary>
        /// Readies the render commands generated so far for rendering.
        /// Carry out any necessary interpolation/extrapolation for animations and effects.
        /// </summary>
        /// <param name="elapsedTime"></param>
        void processRenderCommands(ke::Time elapsedTime);

        /// <summary>
        /// 
        /// </summary>
        void render();

        void setWindow(ke::WindowSptr p_window) { this->window = p_window; }

    private:
        ke::WindowSptr window;
    };

    using RenderSystemUptr = std::unique_ptr<RenderSystem>;
    using RenderSystemSptr = std::shared_ptr<RenderSystem>;
    using RenderSystemWptr = std::weak_ptr<RenderSystem>;
    
}