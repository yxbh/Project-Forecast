#pragma once

#include "KEngine/Interface/IWindow.hpp"
#include "KEngine/Interface/ISystem.hpp"
#include "KEngine/Graphics/Scene.hpp"
#include "KEngine/Graphics/GraphicsCommand.hpp"
#include "KEngine/Common/Time.hpp"
#include "KEngine/Common/Queues.hpp"

#include <array>
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
        size_t prepareCommands(ke::Scene * scene);

        /// <summary>
        /// Dispatch all commands generated/received so far for processing.
        /// </summary>
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
        size_t render();

        void setWindow(ke::WindowSptr p_window) { this->window = p_window; }

        virtual bool initialise() override;
        virtual void shutdown() override;

        virtual void update(ke::Time elapsedTime) override;

    private:
        ke::WindowSptr window;
    };

    using RenderSystemUptr = std::unique_ptr<RenderSystem>;
    using RenderSystemSptr = std::shared_ptr<RenderSystem>;
    using RenderSystemWptr = std::weak_ptr<RenderSystem>;
    
}
