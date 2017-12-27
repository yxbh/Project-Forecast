#pragma once

#include "KEngine/Interfaces/IEvent.hpp"
#include "KEngine/Interfaces/IWindow.hpp"
#include "KEngine/Interfaces/ISystem.hpp"
#include "KEngine/Interfaces/IGraphicsCommandRenderer.hpp"
#include "KEngine/Graphics/Scene.hpp"
#include "KEngine/Graphics/GraphicsCommand.hpp"
#include "KEngine/Common/Time.hpp"
#include "KEngine/Common/Queues.hpp"

#include <cstdint>
#include <memory>

namespace ke
{

    /// <summary>
    /// 
    /// </summary>
    class RenderSystem : ke::ISystem
    {
        KE_DEFINE_SYSTEM_COMMON_PROPERTIES(RenderSystem, 0xED936B34)

    public:
        using GraphicsCommandList = std::vector<ke::GraphicsCommand>;

        RenderSystem();
        virtual ~RenderSystem();

        /// <summary>
        /// Generate render commands from the given scene when it is not null.
        /// </summary>
        /// <param name="scene"></param>
        /// <returns>number of commands prepared.</returns>
        size_t prepareCommands(ke::Scene * scene);

        /// <summary>
        /// Dispatch all commands generated/received so far for processing.
        /// </summary>
        /// <returns>number of commands dispatched.</returns>
        size_t dispatchCommands();

        /// <summary>
        /// Readies the render commands generated so far for rendering.
        /// Carry out any necessary interpolation/extrapolation for animations and effects.
        /// </summary>
        /// <param name="elapsedTime"></param>
        void processCommands(ke::Time elapsedTime);

        /// <summary>
        /// 
        /// </summary>
        /// <returns>number of draw calls issued.</returns>
        size_t render();

        void setWindow(ke::WindowSptr p_window) { this->window = p_window; }

        void receiveEvent(ke::EventSptr event);
        void processEvents();

        void updateOnRenderLoop(ke::Time elapsedTime);

        virtual bool initialise() override;
        virtual void shutdown() override;

        virtual void update(ke::Time elapsedTime) override;

    private:
        ke::WindowSptr window;

        ke::ThreadSafeQueue<ke::EventSptr> events;

        GraphicsCommandRendererUptr m_lineRenderer;
        GraphicsCommandRendererUptr m_circleShapeRenderer;
        GraphicsCommandRendererUptr m_spriteRenderer;
    };

    using RenderSystemUptr = std::unique_ptr<RenderSystem>;
    using RenderSystemSptr = std::shared_ptr<RenderSystem>;
    using RenderSystemWptr = std::weak_ptr<RenderSystem>;
    
}
