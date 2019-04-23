#include "KEngine/Graphics/SDL2/SDL2Window.hpp"

#include "KEngine/Events/AppEvents.hpp"
#include "KEngine/Core/EventManager.hpp"

namespace ke::sdl2
{

    SDL2Window::SDL2Window()
    {
        ke::EventManager::enqueue(ke::makeEvent<ke::WindowCreatedEvent>(this));
    }


    SDL2Window::~SDL2Window()
    {
        if (this->glContext)
        {
            SDL_GL_DeleteContext(this->glContext);
        }

        ke::EventManager::enqueue(ke::makeEvent<ke::WindowDestroyedEvent>(this));
    }

    void SDL2Window::display()
    {
        SDL_UpdateWindowSurface(window.get());
    }

    ke::String SDL2Window::getTitle() const
    {
        return SDL_GetWindowTitle(this->window.get());
    }

    std::uint32_t SDL2Window::getWidth() const
    {
        int width;
        SDL_GetWindowSize(this->window.get(), &width, nullptr);
        return width;
    }

    std::uint32_t SDL2Window::getHeight() const
    {
        int height;
        SDL_GetWindowSize(this->window.get(), nullptr, &height);
        return height;
    }

    void SDL2Window::setTitle(const ke::String & p_title)
    {
        SDL_SetWindowTitle(this->window.get(), p_title.c_str());
    }

    bool SDL2Window::setThreadCurrent(bool threadCurrent)
    {
        // reference: https://forums.libsdl.org/viewtopic.php?t=9087&sid=b555253507f22185464b22f5982a97a7
        return  threadCurrent ?
                SDL_GL_MakeCurrent(this->window.get(), nullptr) == 0 :
                SDL_GL_MakeCurrent(this->window.get(), this->glContext) == 0; /// < 0 when error.
    }

    void * SDL2Window::get()
    {
        return this->window.get();
    }
    
}
