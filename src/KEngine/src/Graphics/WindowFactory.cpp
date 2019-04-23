#include "KEngine/Graphics/WindowFactory.hpp"

#if defined(USE_SDL)
#include "KEngine/Graphics/SDL2/SDL2Window.hpp"
#elif defined(USE_SFML)
#include "KEngine/Graphics/SFML/SfmlWindow.hpp"
#endif

#include "KEngine/Log/Log.hpp"

#include "KEngine/Common/macros.hpp"

namespace ke
{

    ke::WindowSptr WindowFactory::newWindow(
        unsigned p_width, unsigned p_height,
        int p_posX, int p_posY,
        const ke::String & p_title)
    {
#if defined(USE_SDL)
        return createNewSdl2Window(p_width, p_height, p_posX, p_posY, p_title);
#elif defined(USE_SFML)
        return createNewSfmlWindow(p_width, p_height, p_posX, p_posY, p_title);
#else
        return nullptr;
#endif
    }

    ke::WindowSptr WindowFactory::createNewSdl2Window(
        unsigned p_width, unsigned p_height,
        int p_posX, int p_posY,
        const ke::String & p_title)
    {
#if defined(USE_SDL)
        auto window = std::shared_ptr<ke::sdl2::SDL2Window>(new ke::sdl2::SDL2Window);
        window->window = ke::sdl2::SDL2CWindowUptr(
            SDL_CreateWindow(
                p_title.c_str(),
                p_posX,
                p_posY,
                static_cast<int>(p_width),
                static_cast<int>(p_height),
                SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL
            )
        );
        window->glContext = SDL_GL_CreateContext(window->window.get());

        if (nullptr == window->window)
        {
            Log::instance()->critical("SDL window could not be created. Error: {}", SDL_GetError());
            return nullptr;
        }

        if (nullptr == window->glContext)
        {
            Log::instance()->critical("SDL window context could not be created. Error: {}", SDL_GetError());
            return nullptr;
        }

        return window;
#else
        KE_UNUSED(p_width); KE_UNUSED(p_height);
        KE_UNUSED(p_posX); KE_UNUSED(p_posY);
        KE_UNUSED(p_title);
        return nullptr;
#endif
    }

    ke::WindowSptr WindowFactory::createNewSfmlWindow(
        unsigned p_width, unsigned p_height,
        int p_posX, int p_posY,
        const ke::String & p_title)
    {
#if defined(USE_SFML)
        auto window = std::shared_ptr<ke::sfml::SfmlWindow>(new ke::sfml::SfmlWindow);
        assert(window);
        window->window = ke::sfml::SfWindowUptr(
            new sf::RenderWindow(
                sf::VideoMode(p_width, p_height),
                p_title.c_str(),
                sf::Style::Default
            )
        );
        if (p_posX != 0 && p_posY != 0)  // TODO: temporarily ignore (0,0) until I need to figure out if that is a sensible coordinate to set.
        {
            window->window->setPosition({ p_posX, p_posY });
        }
        return window;
#else
        KE_UNUSED(p_width); KE_UNUSED(p_height);
        KE_UNUSED(p_posX); KE_UNUSED(p_posY);
        KE_UNUSED(p_title);
        return nullptr;
#endif
    }

}