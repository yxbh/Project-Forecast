#pragma once

#include "KEngine/Interface/IWindow.hpp"

namespace ke
{
    
    class WindowFactory
    {
    public:
        static ke::WindowSptr newWindow();

    private:
        static ke::WindowSptr createNewSdl2Window();
        static ke::WindowSptr createNewSfmlWindow();

    };

}