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
            virtual void onBeforeInitialisation() {};
            virtual void onPostInitialisation() {};
            virtual void onBeforeShutdown() {};
            virtual void onPostShutdown() {};
    };

    inline IApp::~IApp() {}

}