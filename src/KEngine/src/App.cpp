#include "KEngine/App.hpp"

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
#include "KEngine/Events/SFML/SfmlEvent.hpp"

#include "KEngine/Log/Log.hpp"

#include "KEngine/Graphics/WindowFactory.hpp"

#include <SDL.h>
#include <SFML/Window.hpp>

#include <memory>

namespace
{

    static const ke::Time LOGIC_UPDATE_FIXED_TIMESPAN = ke::Time::milliseconds(20); // 50fps.

    using namespace std::chrono_literals;
    static const auto EVENT_THREAD_SLEEP_DURATION = 1ms;
    static const auto LOGIC_THREAD_SLEEP_DURATION = 1ms;
    static const auto RENDER_THREAD_SLEEP_DURATION = 1ms;

    static std::shared_ptr<ke::GraphicsLoopFrameEvent> graphicsLoopEventHolder;

    static void graphicsLoopEventHandler(ke::EventSptr event)
    {
        std::shared_ptr<ke::GraphicsLoopFrameEvent> frameEvent
                = std::static_pointer_cast<ke::GraphicsLoopFrameEvent>(event);
        if (frameEvent)
        {
            std::atomic_store(&graphicsLoopEventHolder, frameEvent);
        }
    }

}

namespace ke
{
    ke::App * App::sGlobalAppInstance = nullptr;

    App::App()
    {
        assert(!App::sGlobalAppInstance);
        App::sGlobalAppInstance = this;
    }

    App::~App()
    {
        assert(App::sGlobalAppInstance);
        App::sGlobalAppInstance = nullptr;
    }

    int App::exec()
    {
        this->onBeforeInitialisation();

        this->initExec();
        KE_MAKE_SCOPEFUNC([this]() {
            this->cleanUpExec();
        });

        this->onPostInitialisation();

#if defined(USE_SDL)
        auto sdlInitResult = SDL_Init(SDL_INIT_VIDEO);
        if (sdlInitResult < 0)
        {
            Log::instance()->critical("SDL could not initialize! SDL_Error: {}", SDL_GetError());
            return sdlInitResult;
        }
        ke::Log::instance()->info("SDL initialisation successful.");
#endif

        Log::instance()->info("Creating application main window ...");
        this->mainWindow = ke::WindowFactory::newWindow();
        if (nullptr == mainWindow)
        {
#if defined(USE_SDL)
            Log::instance()->critical("SDL window could not be created. Error: {}", SDL_GetError());
#elif defined(USE_SFML)
            Log::instance()->critical("SFML window could not be created.");
#endif
            return ke::ExitCodes::FAILURE_WINDOW_CREATION;
        }
        this->mainWindow->setThreadCurrent(false);// disable window on this thread so can be made thread current on render thread.

        this->renderSystem = std::make_unique<ke::RenderSystem>();
        this->renderSystem->setWindow(this->mainWindow);

        // enter all the engine loops.
        this->enterLogicLoop();
        this->enterGraphicsLoop();
        this->enterEventLoop();        

        this->onBeforeShutdown();

        ke::Log::instance()->info("Destroying main window ...");
        this->mainWindow.reset();

#if defined(USE_SDL)
        ke::Log::instance()->info("Shutting down SDL ...");
        SDL_Quit();
#endif

        this->onPostShutdown();
        
        return ke::ExitCodes::SUCCESS;
    }

    void App::enterEventLoop()
    {
        ke::EventManager::registerListener<ke::GraphicsLoopFrameEvent>(&::graphicsLoopEventHandler);
        KE_MAKE_SCOPEFUNC([](){ ke::EventManager::deregisterListener<ke::GraphicsLoopFrameEvent>(&::graphicsLoopEventHandler); });

        ke::Log::instance()->info("Entering KEngine event loop ...");

        this->isEventLoopRunning = true;
        ke::StopWatch stopwatch;
        ke::HeartBeatGenerator heartBeat(ke::Time::milliseconds(5000));
        while(this->isEventLoopRunning)
        {
            ke::Time frameTime = stopwatch.getElapsed();
            stopwatch.restart();

            ke::EventManager::enqueue(ke::makeEvent<EventLoopFrameEvent>(frameTime));

#if defined(USE_SDL)
            SDL_Event event;
            while (SDL_PollEvent(&event) != 0)
            {
                ke::EventManager::queue(ke::makeEvent<SDL2Event>(event));
                if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE)
                {
                    Log::instance()->info("Normal exit requested.");
                    ke::EventManager::queue(ke::makeEvent<AppExitRequestedEvent>());
                }
            }
#elif defined(USE_SFML)
            sf::Event event;
            auto sfWindow = static_cast<sf::Window*>(this->mainWindow->get());
            assert(sfWindow);
            while (sfWindow->pollEvent(event))
            {
                ke::EventManager::enqueue(ke::makeEvent<ke::SfmlEvent>(event));
                switch (event.type)
                {
                case sf::Event::EventType::Closed:
                    Log::instance()->info("Normal exit requested.");
                    ke::EventManager::enqueue(ke::makeEvent<AppExitRequestedEvent>());
                    break;
                }
            }
#endif

            auto graphicsLoopEvent = std::atomic_load_explicit(&graphicsLoopEventHolder, std::memory_order_relaxed);
            if (graphicsLoopEvent)
            {
                this->mainWindow->setTitle("KEngine - " + std::to_string(graphicsLoopEvent->getFrameTimeSpan().asMilliseconds()) + "ms/frame");
            }

            if (heartBeat)
            {
                ke::Log::instance()->info("Event loop heart beat");
            }

            std::this_thread::sleep_for(EVENT_THREAD_SLEEP_DURATION);
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

        this->mainWindow->setThreadCurrent(true);
        this->mainWindow.reset();
    }

    void App::enterLogicLoop()
    {
        this->logicLoopThread = std::thread([this]()
        {

            ke::EventManager::registerListener<ke::AppExitRequestedEvent>(this, &ke::App::handleAppExitRequest);
            ke::EventManager::registerListener<ke::GraphicsLoopSetupFailureEvent>(this, &ke::App::handleGraphicsLoopSetupFailure);

            ke::Log::instance()->info("Entering KEngine logic loop ...");

            ke::StopWatch stopwatch;
            ke::Time cumulativeLoopTime;
            ke::HeartBeatGenerator heartBeat(ke::Time::milliseconds(5000));
            this->isLogicLoopRunning = true;

            while(this->isLogicLoopRunning)
            {
                cumulativeLoopTime += stopwatch.getElapsed();
                stopwatch.restart();

                //
                // update logic
                //
                while (cumulativeLoopTime >= LOGIC_UPDATE_FIXED_TIMESPAN)
                {
                    cumulativeLoopTime -= LOGIC_UPDATE_FIXED_TIMESPAN;

                    ke::EventManager::enqueue(ke::makeEvent<LogicLoopFrameEvent>(LOGIC_UPDATE_FIXED_TIMESPAN));
                    ke::EventManager::update();

                    //
                    this->resourceManager->update();
                    
                    ke::EventManager::update();

                    // update logic.
                    this->appLogic->update(LOGIC_UPDATE_FIXED_TIMESPAN);
                }

                //
                // prepare and dispatch render commands
                //
                //for (auto view : this->appLogic->getViews())
                //    this->renderSystem->prepareRenderCommands(view.get()->getScene());
                this->renderSystem->prepareCommands(this->appLogic->getCurrentHumanView()->getScene());
                this->renderSystem->dispatchCommands();

                //
                // debug/diagnostic stuff
                //
                if (heartBeat)
                {
                    ke::Log::instance()->info("Logic loop heart beat");
                }

                std::this_thread::sleep_for(LOGIC_THREAD_SLEEP_DURATION);
            }

            // we only flag event loop for termination here as some logics may still require the event loop to be alive.
            this->isEventLoopRunning = false;

            ke::Log::instance()->info("KEngine logic loop exited.");

            //
            // clean up.
            //
            this->getLogic()->getSystemManager()->clear();
        });       
    }

    void App::enterGraphicsLoop()
    {
        this->graphicsLoopThread = std::thread([this]() {

            if (!this->mainWindow->setThreadCurrent(true))
            {
#if defined(USE_SDL)
                Log::instance()->critical("Failure enabling SDL2 window on thread {}. Cannot start graphics loop. SDL2 error: {}",
                    std::hash<std::thread::id>()(std::this_thread::get_id()),
                    SDL_GetError());
#elif defined(USE_SFML)
                Log::instance()->critical("Failure enabling SFML window on thread {}. Cannot start graphics loop.",
                    std::hash<std::thread::id>()(std::this_thread::get_id()));
#endif
                ke::EventManager::dispatchNow(ke::makeEvent<ke::GraphicsLoopSetupFailureEvent>());
                return;
            }

            mainWindow->display();

            ke::Log::instance()->info("Entering KEngine render loop ...");

            ke::StopWatch stopwatch;
            ke::Time cumulativeLoopTime;
            ke::HeartBeatGenerator heartBeat(ke::Time::milliseconds(5000));
            this->isGraphicsLoopRunning = true;

            while (this->isGraphicsLoopRunning)
            {
                ke::Time frameTime = stopwatch.getElapsed();
                stopwatch.restart();
                cumulativeLoopTime += frameTime;

                ke::EventManager::enqueue(ke::makeEvent<GraphicsLoopFrameEvent>(frameTime));

                if (heartBeat)
                {
                    ke::Log::instance()->info("render loop heart beat");
                }

                // process the oldest render command list from the render commands queue
                // e.g. culling, ordering, etc...
                // if queue is empty then interpolate before render.
                this->renderSystem->processCommands(frameTime);
                this->renderSystem->render();
                                
                std::this_thread::sleep_for(RENDER_THREAD_SLEEP_DURATION);
            }

            this->mainWindow->setThreadCurrent(false);

            ke::Log::instance()->info("KEngine render loop exited.");

        });
    }

    void App::setLogic(ke::AppLogicUptr && p_appLogic)
    {
        this->appLogic = std::move(p_appLogic);
    }

    void App::initExec()
    {
        spdlog::set_async_mode(1048576); // magic number from spdlog repo.

        ke::Log::instance()->info("Creating logic and views ...");
        this->createLogicAndViews();

        assert(this->getLogic());
        assert(this->getLogic()->getCurrentHumanView());

        ke::Log::instance()->info("Creating resource manager ...");
        this->resourceManager = std::make_unique<ResourceManager>();
    }

    void App::cleanUpExec()
    {
        spdlog::drop_all();
    }

    void App::handleAppExitRequest(ke::EventSptr)
    {
        if (isNormalExitRequested)
        {
            ke::Log::instance()->info("Already handling app exit request.");
            return;
        }
        isNormalExitRequested = true;
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
