#pragma once

namespace ke::priv
{
    class IApp
    {
        public:
            virtual ~IApp();

            virtual int exec() = 0;

            virtual void onBeforeInitialisation() {};
            virtual void onPostInitialisation() {};
            virtual void onBeforeShutdown() {};
            virtual void onPostShutdown() {};
    };

    inline IApp::~IApp() {}
}