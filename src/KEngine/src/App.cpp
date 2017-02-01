#include "KEngine/App.hpp"
#include "KEngine/Log/Log.hpp"
#include "KEngine/Common/macros.hpp"
#include "KEngine/Common/ScopeFunc.hpp"
#include "KEngine/Common/String.hpp"
#include "KEngine/Graphics/Window.hpp"

#include <SDL.h>

#include <memory>

namespace
{

}

namespace ke
{

    int App::exec()
    {
        this->initExec();
        KE_MAKE_SCOPEFUNC([this]() {
            this->cleanUpExec();
        });

        auto sdlInitResult = SDL_Init(SDL_INIT_VIDEO);
        if (sdlInitResult < 0)
        {
            Log::instance()->critical("SDL could not initialize! SDL_Error: {}", SDL_GetError());
            return sdlInitResult;
        }
        ke::Log::instance()->info("SDL initialisation successful.");

        auto window = ke::Window::create();
        if (nullptr == window)
        {
            Log::instance()->critical("SDL window could not be created. Error: {}", SDL_GetError());
            return -1;
        }

        window->display();
    
        SDL_Delay(2000);

        window.reset();
        SDL_Quit();
        
        return 0;
    }

    void App::initExec()
    {
        spdlog::set_async_mode(1048576); // magic number from spdlog repo.
    }

    void App::cleanUpExec()
    {
        spdlog::drop_all();
    }

}
