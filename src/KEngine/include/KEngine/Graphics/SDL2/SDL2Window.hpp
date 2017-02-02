#pragma once

#include "KEngine/Interface/IWindow.hpp"
#include "KEngine/Interface/IEvent.hpp"
#include "KEngine/Log/Log.hpp"

#include <SDL_video.h>

namespace ke::sdl2
{

    class SDL2WindowDeleter
    {
    public:
        inline void operator()(SDL_Window * window)
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
        static WindowSptr create();

        inline SDL2Window()
        {

        }
        virtual ~SDL2Window();

        virtual void display() final;

        virtual void setTitle(const ke::String & title) final;

        virtual void setThreadCurrent() final
        {
            //SDL_GL_MakeCurrent
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
        void handleGraphicsLoopFrameEvent(ke::EventSptr event);

        SDL2WindowSptr window; /// SDL_Window instance managed by smart pointer.

        std::size_t width = 1920;
        std::size_t height = 1080;

        ke::String title = KE_TEXT("ProjectForecast");
    };

}