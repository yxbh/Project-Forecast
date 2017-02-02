#pragma once
#include "KEngine/Interface/IApp.hpp"

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
        void initExec();
        void cleanUpExec();
    };
}
