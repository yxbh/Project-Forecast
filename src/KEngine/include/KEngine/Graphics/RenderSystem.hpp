#pragma once

#include "KEngine/Interface/IWindow.hpp"
#include "KEngine/Graphics/Scene.hpp"
#include "KEngine/Graphics/GraphicsCommand.hpp"
#include "KEngine/Common/Time.hpp"
#include "KEngine/Common/Queues.hpp"

#include <memory>

namespace ke
{

    /// <summary>
    /// 
    /// </summary>
    class RenderSystem
    {
    public:
        using GraphicsCommandList = std::vector<ke::GraphicsCommand>;

        /// <summary>
        /// Generate render commands from the given scene when it is not null.
        /// </summary>
        /// <param name="scene"></param>
        void prepareCommands(ke::Scene * scene);

        /// <summary>
        /// Prepare all the render commands generated so far for processing.
        /// </summary>
        void dispatchCommands();

        /// <summary>
        /// Readies the render commands generated so far for rendering.
        /// Carry out any necessary interpolation/extrapolation for animations and effects.
        /// </summary>
        /// <param name="elapsedTime"></param>
        void processCommands(ke::Time elapsedTime);

        /// <summary>
        /// 
        /// </summary>
        void render();

        void setWindow(ke::WindowSptr p_window) { this->window = p_window; }

    private:
        ke::WindowSptr window;

        GraphicsCommandList logicThreadRenderCommandQueue;
        ke::ThreadSafeQueue<GraphicsCommandList> renderThreadCommandLists;
    };

    using RenderSystemUptr = std::unique_ptr<RenderSystem>;
    using RenderSystemSptr = std::shared_ptr<RenderSystem>;
    using RenderSystemWptr = std::weak_ptr<RenderSystem>;
    
}