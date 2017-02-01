#pragma once
#include "KEngine/Interface/IApp.hpp"

namespace ke
{
    /// <summary>
    /// 
    /// </summary>
    class App : public ke::priv::IApp
    {
    public:
        virtual int exec() override;

    private:
        void initExec();
        void cleanUpExec();
    };
}
