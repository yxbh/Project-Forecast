#include "KEngine/Graphics/SDL2/SDL2Window.hpp"

#include "KEngine/Core/EventManager.hpp"
#include "KEngine/Events/GraphicsLoopFrameEvent.hpp"

namespace ke::sdl2
{

    WindowSptr SDL2Window::create()
    {
        auto window = std::make_shared<SDL2Window>();
        window->window = SDL2WindowSptr(
                            SDL_CreateWindow(
                                window->title.c_str(),
                                SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED,
                                static_cast<int>(window->width),
                                static_cast<int>(window->height),
                                SDL_WINDOW_SHOWN
                            ),
                            SDL_DestroyWindow);

        if (nullptr == window->window)
        {
            Log::instance()->critical("SDL window could not be created. Error: {}", SDL_GetError());
            return nullptr;
        }
        else
        {
            ke::EventManager::registerListener<ke::GraphicsLoopFrameEvent>(
                window.get(), &ke::sdl2::SDL2Window::handleGraphicsLoopFrameEvent);
        }

        return window;
    }

    SDL2Window::~SDL2Window()
    {
        if (window)
        {
            ke::EventManager::deregisterListener<ke::GraphicsLoopFrameEvent>(
                window.get(), &ke::sdl2::SDL2Window::handleGraphicsLoopFrameEvent);
        }
    }

    void SDL2Window::display()
    {
        SDL_UpdateWindowSurface(window.get());
    }

    void SDL2Window::setTitle(const ke::String & p_title)
    {
        this->title = p_title;
        SDL_SetWindowTitle(this->window.get(), this->title.c_str());
    }

    void SDL2Window::handleGraphicsLoopFrameEvent(ke::EventSptr event)
    {
        std::shared_ptr<GraphicsLoopFrameEvent> frameEvent = std::static_pointer_cast<GraphicsLoopFrameEvent>(event);
        if (frameEvent)
        {
            this->setTitle("ProjectForecast - " + std::to_string(frameEvent->getFrameTimeSpan().asMilliseconds()) + "ms/frame");
        }
    }
    
}