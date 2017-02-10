#pragma once

#include "KEngine/Graphics/SDL2/SDL2Window.hpp"
#include "KEngine/Graphics/SFML/SfmlWindow.hpp"

namespace ke
{

#if defined(USE_SDL)

    class Window : public ke::sdl2::SDL2Window
    {
    public:
    };

#elif defined(USE_SFML)

    class Window : public ke::sfml::SfmlWindow
    {
    public:
    };

#endif

}