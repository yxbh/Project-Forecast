#include "KEngine/App.hpp"
#include "KEngine/Log/Log.hpp"
#include "KEngine/Common/macros.hpp"
#include "KEngine/Common/ScopeFunc.hpp"
#include "KEngine/Common/StopWatch.hpp"
#include "KEngine/Common/String.hpp"
#include "KEngine/Core/EventManager.hpp"
#include "KEngine/Graphics/Window.hpp"
#include "KEngine/Events/EventLoopFrameEvent.hpp"
#include "KEngine/Events/LogicLoopFrameEvent.hpp"
#include "KEngine/Events/GraphicsLoopFrameEvent.hpp"
#include "KEngine/Events/SDL2/SDL2Event.hpp"

#include <SDL.h>

#include <memory>
#include <thread>

namespace ke::priv
{
    class SDL2GlContextManager
    {
    public:
        static SDL2GlContextManager * instance()
        {
            static std::unique_ptr<SDL2GlContextManager> instance = std::make_unique<SDL2GlContextManager>();
            return instance.get();
        }
    };
}

namespace ke
{

    int App::exec()
    {
        this->onBeforeInitialisation();
        this->initExec();
        KE_MAKE_SCOPEFUNC([this]() {
            this->cleanUpExec();
        });
        this->onPostInitialisation();

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

        ke::StopWatch stopwatch;
        SDL_Event event;
        bool running = true;
        for (; running;)
        {
            ke::Time frameTime = stopwatch.getElapsed();
            stopwatch.restart();

            ke::EventManager::queue(std::make_shared<EventLoopFrameEvent>(frameTime));
            ke::EventManager::queue(std::make_shared<LogicLoopFrameEvent>(frameTime));
            ke::EventManager::queue(std::make_shared<GraphicsLoopFrameEvent>(frameTime));

            while (SDL_PollEvent(&event) != 0)
            {
                ke::EventManager::queue(std::make_shared<SDL2Event>(event));
                if (event.type == SDL_QUIT)
                {
                    running = false;
                    Log::instance()->info("Exit requested.");
                }
            }

            ke::EventManager::update();

            

            window->display();

            using namespace std::chrono_literals;
            std::this_thread::sleep_for(1ms);
        }

        this->onBeforeShutdown();
        window.reset();
        SDL_Quit();
        this->onPostShutdown();
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
