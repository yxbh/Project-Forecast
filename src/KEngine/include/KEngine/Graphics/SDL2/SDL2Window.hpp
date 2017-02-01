#pragma once

#include "KEngine/Interface/IWindow.hpp"

#include <SDL.h>

namespace ke::sdl2
{

    class SDL2WindowDeleter
    {
    public:
        void operator()(SDL_Window * window)
        {
            SDL_DestroyWindow(window);
        }
    };

    using SDL2WindowUptr = std::unique_ptr<SDL_Window, SDL2WindowDeleter>;
    using SDL2WindowSptr = std::shared_ptr<SDL_Window>;
    using SDL2WindowWptr = std::weak_ptr<SDL_Window>;

    class SDL2Window : public ke::IWindow
    {
    public:
        static WindowSptr create()
        {
            auto window = std::make_shared<SDL2Window>();
            window->window = SDL2WindowSptr(SDL_CreateWindow(
                window->title.c_str(),
                SDL_WINDOWPOS_UNDEFINED,
                SDL_WINDOWPOS_UNDEFINED,
                window->width, window->height,
                SDL_WINDOW_SHOWN),
                SDL_DestroyWindow);

            if (nullptr == window)
            {
                Log::instance()->critical("SDL window could not be created. Error: {}", SDL_GetError());
            }

            return window;
        }

        virtual ~SDL2Window() = default;

        virtual void display() final
        {
            SDL_UpdateWindowSurface(window.get());
        }

        virtual void setTitle(ke::String title) final
        {
            this->title = title;
            SDL_SetWindowTitle(this->window.get(), this->title.c_str());
        }

        inline SDL_Window * get()
        {
            return window.get();
        }

        inline SDL_Surface * getSurface()
        {
            return SDL_GetWindowSurface(this->get());
        }

    private:
        SDL2WindowSptr window; /// SDL_Window instance managed by smart pointer.

        std::size_t width = 1920;
        std::size_t height = 1080;

        ke::String title = KE_TEXT("ProjectForecast");
    };

}