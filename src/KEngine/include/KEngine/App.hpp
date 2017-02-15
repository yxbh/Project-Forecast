#pragma once

#include "KEngine/Interface/IApp.hpp"
#include "KEngine/Interface/IEvent.hpp"
#include "KEngine/Interface/IWindow.hpp"
#include "KEngine/Core/ResourceManager.hpp"
#include "KEngine/Graphics/RenderSystem.hpp"
#include "KEngine/BaseAppLogic.hpp"

#include <atomic>
#include <utility>
#include <thread>

namespace ke
{

    /// <summary>
    /// Application class that represents a KEngine application.
    /// Inherit this class to create a custom ke::AppLogic and views to implement a custom application.
    /// Calling exec() begins the execution of the application and the member function does not return until the application exits.
    /// Call setLogic() to set the logic before calling exec().
    /// Only ever one instance of an application object should be created and it must be done on the main thread where the main() function is called.
    /// </summary>
    class App : public ke::priv::IApp
    {
    public:
        App();
        virtual ~App();

        /// <summary>
        /// Begin KEngine application execution. This begins the lifetime of the application.
        /// NOTE: Do not override this member function!
        /// </summary>
        /// <returns>Application exit code.</returns>
        virtual int exec() override;

        virtual void setLogic(ke::AppLogicUptr && appLogic);
        inline ke::BaseAppLogic * getLogic() const { return this->appLogic.get(); };

        inline static ke::App * instance() { return App::sGlobalAppInstance; }

    protected:
        static ke::App * sGlobalAppInstance;

        /// <summary>
        /// Implement this to create the logic and views for the application.
        /// This member function is called during the initilisation process after exec() is called.
        /// Do not call this yourself.
        /// </summary>
        virtual void createLogicAndViews() = 0;

    private:
        void enterEventLoop();
        void enterLogicLoop();
        void enterGraphicsLoop();

        void initExec();
        void cleanUpExec();

        void handleAppExitRequest(ke::EventSptr);
        void handleGraphicsLoopSetupFailure(ke::EventSptr);

        ke::AppLogicUptr appLogic;
        ke::RenderSystemUptr renderSystem;

        ke::ResourceManagerUptr resourceManager;

        std::atomic_bool isEventLoopRunning { false };
        std::atomic_bool isLogicLoopRunning { false };
        std::atomic_bool isGraphicsLoopRunning { false };

        std::atomic_bool isNormalExitRequested { false };

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
