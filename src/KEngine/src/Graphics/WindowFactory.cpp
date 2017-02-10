#include "KEngine/Graphics/WindowFactory.hpp"

#include "KEngine/Graphics/SDL2/SDL2Window.hpp"
#include "KEngine/Graphics/SFML/SfmlWindow.hpp"
#include "KEngine/Log/Log.hpp"

namespace ke::priv
{
    static const auto DEFAULT_WINDOW_TITLE = "KEngine";
    static const int DEFAULT_WINDOW_WIDTH = 1600;
    static const int DEFAULT_WINDOW_HEIGHT = 900;
}

namespace ke
{

    ke::WindowSptr WindowFactory::newWindow()
    {
#if defined(USE_SDL)
        return createNewSdl2Window();
#elif defined(USE_SFML)
        return createNewSfmlWindow();
#else
        return nullptr;
#endif
    }

    ke::WindowSptr WindowFactory::createNewSdl2Window()
    {
#if defined(USE_SDL)
        auto window = std::shared_ptr<ke::sdl2::SDL2Window>(new ke::sdl2::SDL2Window);
        window->window = ke::sdl2::SDL2CWindowUptr(
            SDL_CreateWindow(
                priv::DEFAULT_WINDOW_TITLE,
                SDL_WINDOWPOS_UNDEFINED,
                SDL_WINDOWPOS_UNDEFINED,
                priv::DEFAULT_WINDOW_WIDTH,
                priv::DEFAULT_WINDOW_HEIGHT,
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
        return nullptr;
#endif
    }

    ke::WindowSptr WindowFactory::createNewSfmlWindow()
    {
#if defined(USE_SFML)
        auto window = std::shared_ptr<ke::sfml::SfmlWindow>(new ke::sfml::SfmlWindow);
        window->window = ke::sfml::SfWindowUptr(
            new sf::RenderWindow(
                sf::VideoMode(priv::DEFAULT_WINDOW_WIDTH, priv::DEFAULT_WINDOW_HEIGHT),
                priv::DEFAULT_WINDOW_TITLE,
                sf::Style::Default
            )
        );
        return window;
#else
        return nullptr;
#endif
    }

}