#pragma once

#include "KEngine/Interface/IApp.hpp"
#include "KEngine/Interface/IEvent.hpp"
#include "KEngine/Interface/IWindow.hpp"
#include "KEngine/Core/ResourceManager.hpp"

#include <atomic>
#include <utility>
#include <thread>

namespace ke
{

    /// <summary>
    /// Application class that represents a KEngine application.
    /// Calling exec() begins the execution of the application and the member function does not return until the application exits.
    /// Only ever one instance of an application object should be created and it must be done on the main thread where the main() function is called.
    /// </summary>
    class App : public ke::priv::IApp
    {
    public:

        /// <summary>
        /// Begin KEngine application execution. This begins the lifetime of the application.
        /// NOTE: Do not override this member function!
        /// </summary>
        /// <returns>Application exit code.</returns>
        virtual int exec() override;

    private:
        void enterEventLoop();
        void enterLogicLoop();
        void enterGraphicsLoop();

        void initExec();
        void cleanUpExec();

        void handleAppExitRequest(ke::EventSptr);
        void handleGraphicsLoopSetupFailure(ke::EventSptr);

        
        ResourceManagerUptr resourceManager;

        std::atomic_bool isEventLoopRunning { false };
        std::atomic_bool isLogicLoopRunning { false };
        std::atomic_bool isGraphicsLoopRunning { false };

        std::thread logicLoopThread;
        std::thread graphicsLoopThread;

        ke::WindowSptr mainWindow;
    };

    /// <summary>
    /// KEngine exit codes.
    /// </summary>
    enum ExitCodes : std::int32_t
    {
        SUCCESS = 0,
        FAILURE_WINDOW_CREATION = 1,
        UNKNOWN = std::numeric_limits<std::int32_t>::max()
    };

}
