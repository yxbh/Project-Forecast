#pragma once
#include "KEngine\Interface\IApp.hpp"

namespace ke
{
    class App : public ke::priv::IApp
    {
    public:
        virtual int exec() override
        {
            return 0;
        }
    };
}