#pragma once

#include "KEngine/Interfaces/IWindow.hpp"
#include "KEngine/Interfaces/ISystem.hpp"
#include "KEngine/Graphics/Scene.hpp"
#include "KEngine/Common/Time.hpp"

#include <memory>

namespace ke
{
    class IRenderSystem;

    using RenderSystemUptr = std::unique_ptr<IRenderSystem>;
    using RenderSystemSptr = std::shared_ptr<IRenderSystem>;
    using RenderSystemWptr = std::weak_ptr<IRenderSystem>;


    /// <summary>
    /// Interface for implementing a render system.
    /// </summary>
    class IRenderSystem : public ke::ISystem
    {
    public:
        using ISystem::ISystem;

        /// <summary>
        /// Generate render commands from the given scene when it is not null.
        /// </summary>
        /// <param name="scene"></param>
        /// <returns>number of commands prepared.</returns>
        virtual size_t prepareCommands(ke::Scene * scene) = 0;

        /// <summary>
        /// Dispatch all commands generated/received so far for processing.
        /// </summary>
        /// <returns>number of commands dispatched.</returns>
        virtual size_t dispatchCommands() = 0;

        /// <summary>
        /// Readies the render commands generated so far for rendering.
        /// Carry out any necessary interpolation/extrapolation for animations and effects.
        /// </summary>
        /// <param name="elapsedTime"></param>
        virtual void processCommands(ke::Time elapsedTime) = 0;

        /// <summary>
        /// Draw the processed commands to screen.
        /// </summary>
        /// <returns>number of draw calls issued.</returns>
        virtual size_t render() = 0;

        virtual void setWindow(ke::WindowSptr p_window) = 0;

        virtual void updateOnRenderLoop(ke::Time elapsedTime) = 0;
    };

    /// <summary>
    /// Default render system.
    /// </summary>
    class RenderSystem : public ke::IRenderSystem
    {
        KE_DEFINE_SYSTEM_COMMON_PROPERTIES(RenderSystem, 0xED936B34)

    public:
        using GraphicsCommandList = std::vector<ke::GraphicsCommand>;

        RenderSystem();
        virtual ~RenderSystem();

        virtual size_t prepareCommands(ke::Scene* scene) override;

        virtual size_t dispatchCommands() override;

        virtual void processCommands(ke::Time elapsedTime) override;

        virtual size_t render() override;

        virtual void setWindow(ke::WindowSptr p_window) override;

        virtual void updateOnRenderLoop(ke::Time elapsedTime) override;

        virtual bool initialise() override;
        virtual void shutdown() override;

        virtual void update(ke::Time elapsedTime) override;

    private:
        RenderSystemUptr impl;
    };

}
