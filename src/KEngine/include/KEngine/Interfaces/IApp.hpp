#pragma once

namespace ke::priv
{

    /// <summary>
    /// Interface for implementing an Application class that represents a KEngine application.
    /// Calling exec() begins the execution of the application and the member function does not return until the application exits.
    /// This interface is internal. Do not inherit if you are a KEngine user.
    /// </summary>
    class IApp
    {
    public:
            virtual ~IApp() = 0;

            /// <summary>
            /// Begin KEngine application execution. This begins the lifetime of the application.
            /// </summary>
            /// <returns>Application exit code.</returns>
            virtual int exec() = 0;

    protected:
            /// <summary>
            /// Event function called before engine initialisation begins.
            /// Override this to do any work before engine initialises.
            /// </summary>
            virtual void onBeforeInitialisation() {};
            /// <summary>
            /// Event function called after engine initialisation.
            /// Override this to do any work after engine is initialised.
            /// </summary>
            virtual void onPostInitialisation() {};
            /// <summary>
            /// Event function called before engine shutdown begins.
            /// OVerride this to do any work before engine begins shutdown.
            /// </summary>
            virtual void onBeforeShutdown() {};
            /// <summary>
            /// Event function called after engine shutdown.
            /// OVerride this to do any work after engine has performed shutdown.
            /// </summary>
            virtual void onPostShutdown() {};
    };

    inline IApp::~IApp() {}

}