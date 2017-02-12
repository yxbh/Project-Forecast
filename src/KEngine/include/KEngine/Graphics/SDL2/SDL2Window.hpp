#pragma once

#include "KEngine/Interface/IWindow.hpp"
#include "KEngine/Interface/IEvent.hpp"

#include <SDL_video.h>

namespace ke
{
    class WindowFactory;
}

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

    using SDL2CWindowUptr = std::unique_ptr<SDL_Window, SDL2WindowDeleter>;
    using SDL2CWindowSptr = std::shared_ptr<SDL_Window>;
    using SDL2CWindowWptr = std::weak_ptr<SDL_Window>;

    class SDL2Window : public ke::IWindow
    {
        friend class ke::WindowFactory;

    public:
        virtual ~SDL2Window();

        virtual void display() final;

        virtual ke::String getTitle() const final;

        virtual std::uint32_t getWidth() const final;
        virtual std::uint32_t getHeight() const final;

        virtual void setTitle(const ke::String & title) final;

        virtual bool setThreadCurrent(bool threadCurrent = true) final;

        virtual void * get() final;

    private:
        SDL2Window();

        SDL2CWindowUptr window; /// SDL_Window instance managed by smart pointer.
        SDL_GLContext glContext;
        
    };

}
