#include "KEngine/App.hpp"
#include "KEngine/Log/Log.hpp"
#include "KEngine/Common/macros.hpp"
#include "KEngine/Common/HeartBeatGenerator.hpp"
#include "KEngine/Common/ScopeFunc.hpp"
#include "KEngine/Common/StopWatch.hpp"
#include "KEngine/Common/String.hpp"
#include "KEngine/Core/EventManager.hpp"
#include "KEngine/Events/AppExitRequestedEvent.hpp"
#include "KEngine/Events/EventLoopFrameEvent.hpp"
#include "KEngine/Events/LogicLoopFrameEvent.hpp"
#include "KEngine/Events/GraphicsLoopFrameEvent.hpp"
#include "KEngine/Events/GraphicsLoopSetupFailureEvent.hpp"
#include "KEngine/Events/SDL2/SDL2Event.hpp"
#include "KEngine/Graphics/Window.hpp"

#include <SDL.h>

#include <memory>

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

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

        this->mainWindow = ke::Window::create();
        if (nullptr == mainWindow)
        {
            Log::instance()->critical("SDL window could not be created. Error: {}", SDL_GetError());
            return ke::ExitCodes::FAILURE_WINDOW_CREATION;
        }

        this->enterLogicLoop();
        this->enterGraphicsLoop();
        this->enterEventLoop();        

        this->onBeforeShutdown();
        ke::Log::instance()->info("Destroying main window ...");
        this->mainWindow.reset();
        ke::Log::instance()->info("Shutting down SDL ...");
        SDL_Quit();
        this->onPostShutdown();
        
        return ke::ExitCodes::SUCCESS;
    }

    void App::enterEventLoop()
    {
        ke::Log::instance()->info("Entering KEngine event loop.");

        this->isEventLoopRunning = true;
        ke::StopWatch stopwatch;
        ke::HeartBeatGenerator heartBeat(ke::Time::milliseconds(5000));
        SDL_Event event;
        while(this->isEventLoopRunning)
        {
            ke::Time frameTime = stopwatch.getElapsed();
            stopwatch.restart();

            ke::EventManager::queue(ke::makeEvent<EventLoopFrameEvent>(frameTime));

            while (SDL_PollEvent(&event) != 0)
            {
                ke::EventManager::queue(ke::makeEvent<SDL2Event>(event));
                if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE)
                {
                    Log::instance()->info("Normal exit requested.");
                    ke::EventManager::queue(ke::makeEvent<AppExitRequestedEvent>());
                }
            }

            if (heartBeat)
            {
                ke::Log::instance()->info("Event loop heart beat");
            }

            using namespace std::chrono_literals;
            std::this_thread::sleep_for(1ms);
        }

        ke::Log::instance()->info("KEngine event loop exited.");

        // join render loop thread
        if (this->graphicsLoopThread.joinable())
        {
            ke::Log::instance()->info("KEngine graphics loop thread joining ...");
            this->graphicsLoopThread.join();
            ke::Log::instance()->info("KEngine graphics loop thread joined");
        }
        else
        {
            ke::Log::instance()->warn("KEngine logic loop thread not joinable.");
        }

        // join logic loop thread.
        if (this->logicLoopThread.joinable())
        {
            ke::Log::instance()->info("KEngine logic loop thread joining ...");
            this->logicLoopThread.join();
            ke::Log::instance()->info("KEngine logic loop thread joined");
        }
        else
        {
            ke::Log::instance()->warn("KEngine logic loop thread not joinable.");
        }
    }

    void App::enterLogicLoop()
    {
        this->logicLoopThread = std::thread([this]()
        {

            ke::EventManager::registerListener<ke::AppExitRequestedEvent>(this, &ke::App::handleAppExitRequest);
            ke::EventManager::registerListener<ke::GraphicsLoopSetupFailureEvent>(this, &ke::App::handleGraphicsLoopSetupFailure);

            ke::Log::instance()->info("Entering KEngine logic loop.");

            ke::StopWatch stopwatch;
            ke::Time cumulativeLoopTime;
            ke::HeartBeatGenerator heartBeat(ke::Time::milliseconds(5000));
            this->isLogicLoopRunning = true;

            while(this->isLogicLoopRunning)
            {
                ke::Time frameTime = stopwatch.getElapsed();
                stopwatch.restart();
                cumulativeLoopTime += frameTime;

                ke::EventManager::queue(ke::makeEvent<LogicLoopFrameEvent>(frameTime));

                ke::EventManager::update();

                if (heartBeat)
                {
                    ke::Log::instance()->info("Logic loop heart beat");
                }


                using namespace std::chrono_literals;
                std::this_thread::sleep_for(1ms);
            }

            // we only flag event loop for termination here as some logics may still require the event loop to be alive.
            this->isEventLoopRunning = false;

            ke::Log::instance()->info("KEngine logic loop exited.");

        });       
    }

    void App::enterGraphicsLoop()
    {
        this->graphicsLoopThread = std::thread([this]() {

            if (!this->mainWindow->setThreadCurrent())
            {
                Log::instance()->critical("Failure enabling SDL2 window on thread {}. Cannot start graphics loop. SDL2 error: {}",
                    std::hash<std::thread::id>()(std::this_thread::get_id()),
                    SDL_GetError());
                ke::EventManager::dispatchNow(ke::makeEvent<ke::GraphicsLoopSetupFailureEvent>());
                return;
            }

            ke::Log::instance()->info("Entering KEngine render loop.");

            ke::StopWatch stopwatch;
            ke::Time cumulativeLoopTime;
            ke::HeartBeatGenerator heartBeat(ke::Time::milliseconds(5000));
            this->isGraphicsLoopRunning = true;

            while (this->isGraphicsLoopRunning)
            {
                ke::Time frameTime = stopwatch.getElapsed();
                stopwatch.restart();
                cumulativeLoopTime += frameTime;

                ke::EventManager::queue(ke::makeEvent<GraphicsLoopFrameEvent>(frameTime));

                if (heartBeat)
                {
                    ke::Log::instance()->info("render loop heart beat");
                }

                // process the oldest render command list from the render commands queue
                // e.g. culling, ordering, etc...
                // if queue is empty then interpolate before render.

                mainWindow->display();

                using namespace std::chrono_literals;
                std::this_thread::sleep_for(1ms);
            }

            ke::Log::instance()->info("KEngine render loop exited.");

        });
    }

    void App::initExec()
    {
        spdlog::set_async_mode(1048576); // magic number from spdlog repo.
    }

    void App::cleanUpExec()
    {
        spdlog::drop_all();
    }

    void App::handleAppExitRequest(ke::EventSptr)
    {
        ke::Log::instance()->info("Handling app exit request.");
        //this->isEventLoopRunning = false;
        this->isLogicLoopRunning = false;
        this->isGraphicsLoopRunning = false;
    }

    void App::handleGraphicsLoopSetupFailure(ke::EventSptr)
    {
        ke::Log::instance()->critical("Graphics loop setup failure encountered. Shuting down ...");
        this->isLogicLoopRunning = false;
        this->isGraphicsLoopRunning = false;
        this->isEventLoopRunning = false;
    }

}
