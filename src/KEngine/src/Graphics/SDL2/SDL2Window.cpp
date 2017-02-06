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
                                KE_TEXT("KEngine"),
                                SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED,
                                1600, // width
                                900,  // height
                                SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL
                            ),
                            SDL_DestroyWindow);
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

        ke::EventManager::registerListener<ke::GraphicsLoopFrameEvent>(
            window.get(), &ke::sdl2::SDL2Window::handleGraphicsLoopFrameEvent);
        
        return window;
    }

    SDL2Window::~SDL2Window()
    {
        if (this->glContext)
        {
            SDL_GL_DeleteContext(this->glContext);
        }

        if (window)
        {
            ke::EventManager::deregisterListener<ke::GraphicsLoopFrameEvent>(
                this, &ke::sdl2::SDL2Window::handleGraphicsLoopFrameEvent);
        }
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

    bool SDL2Window::setThreadCurrent()
    {
        SDL_GL_DeleteContext(this->glContext);
        this->glContext = SDL_GL_CreateContext(this->window.get());
        return SDL_GL_MakeCurrent(this->window.get(), this->glContext) == 0; // < 0 when error.
    }

    void SDL2Window::handleGraphicsLoopFrameEvent(ke::EventSptr event)
    {
        std::shared_ptr<GraphicsLoopFrameEvent> frameEvent = std::static_pointer_cast<GraphicsLoopFrameEvent>(event);
        if (frameEvent)
        {
            this->setTitle("KEngine - " + std::to_string(frameEvent->getFrameTimeSpan().asMilliseconds()) + "ms/frame");
        }
    }
    
}