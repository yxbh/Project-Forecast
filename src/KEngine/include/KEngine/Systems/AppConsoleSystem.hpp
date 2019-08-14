#pragma once

#include "KEngine/Interfaces/ISystem.hpp"
#include "KEngine/Common/String.hpp"

#include <memory>
#include <utility>


namespace ke
{
    /// <summary>
    /// Interface class for implementing custom command processors.
    /// </summary>
    class IAppConsoleCommandProcessor
    {
    public:
        // return true to indicate the command line has been processed.
        virtual std::pair<bool, ke::String> processCommandLine(const ke::String& p_commandLine) = 0;
    };

    using AppConsoleCommandProcessorUptr = std::unique_ptr<IAppConsoleCommandProcessor>;
    using AppConsoleCommandProcessorSptr = std::shared_ptr<IAppConsoleCommandProcessor>;
    using AppConsoleCommandProcessorWptr = std::weak_ptr<IAppConsoleCommandProcessor>;


    class IAppConsoleSystem : public ke::ISystem
    {
    public:
        using ISystem::ISystem;

        virtual bool registerCommandProcessor(AppConsoleCommandProcessorSptr) = 0;
        virtual void deregisterCommandProcessor(AppConsoleCommandProcessorSptr p_processor) = 0;
    };


    /// <summary>
    /// This system provides the dev console UI.
    /// </summary>
    class AppConsoleSystem final : public ke::ISystem
    {
        KE_DEFINE_SYSTEM_COMMON_PROPERTIES(AppConsoleSystem, 0xA1A2b715)

    public:
        AppConsoleSystem(void);

        virtual bool initialise(void) final;
        virtual void shutdown(void) final;

        virtual void update(ke::Time elapsedTime) final;

        
        virtual bool registerCommandProcessor(AppConsoleCommandProcessorSptr p_processor) final;
        virtual void deregisterCommandProcessor(AppConsoleCommandProcessorSptr p_processor) final;

    private:
        std::unique_ptr<IAppConsoleSystem> systemImpl;
    };

} // namespace ke