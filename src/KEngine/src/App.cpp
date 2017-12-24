#include "KEngine/App.hpp"

#include "KEngine/Common/macros.hpp"
#include "KEngine/Common/HeartBeatGenerator.hpp"
#include "KEngine/Common/ScopeFunc.hpp"
#include "KEngine/Common/StopWatch.hpp"
#include "KEngine/Common/String.hpp"
#include "KEngine/Core/EventManager.hpp"
#include "KEngine/Core/FrameRateCounter.hpp"

#include "KEngine/Events/AppExitRequestedEvent.hpp"
#include "KEngine/Events/EventLoopFrameEvent.hpp"
#include "KEngine/Events/LogicLoopFrameEvent.hpp"
#include "KEngine/Events/GraphicsLoopFrameEvent.hpp"
#include "KEngine/Events/GraphicsLoopSetupFailureEvent.hpp"
#include "KEngine/Events/SDL2/SDL2Event.hpp"
#include "KEngine/Events/SFML/SfmlEvent.hpp"
#include "KEngine/Events/SFML/SfmlEventTranslator.hpp"

#include "KEngine/Log/Log.hpp"

#include "KEngine/Graphics/WindowFactory.hpp"

#include <SDL.h>
#include <SFML/Graphics/RenderWindow.hpp>

#include <atomic>
#include <cstdio>
#include <memory>

namespace
{

    static const ke::Time LOGIC_UPDATE_FIXED_TIMESPAN = ke::Time::milliseconds(20); // 50fps.

    static const auto EVENT_THREAD_SLEEP_DURATION = std::chrono::microseconds(100);
    static const auto LOGIC_THREAD_SLEEP_DURATION = std::chrono::milliseconds(2);
    static const auto RENDER_THREAD_SLEEP_DURATION = std::chrono::microseconds(10);

    static const ke::Time LOGIC_THREAD_TARGET_FRAMETIME = ke::Time::milliseconds(10);

    static std::atomic<double> logicLoopFrameTimeMs = { 0.0f };
    static std::atomic<double> renderLoopFrameTimeMs = { 0.0f };

    static std::atomic<float> eventLoopFps    = { 0.0f };
    static std::atomic<float> logicLoopFps    = { 0.0f };
    static std::atomic<float> graphicsLoopFps = { 0.0f };

    static std::atomic<size_t> graphicsCommandCount = { 0 };
    static std::atomic<size_t> graphicsDrawCallCount = { 0 };

    static std::atomic<size_t> eventManagerEventCount = { 0 };

    static char windowTitleTextBuffer[256] = {};

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
        ke::Log::instance()->info("Entering KEngine event loop ...");

        this->isEventLoopRunning = true;
        ke::StopWatch stopwatch;
        ke::HeartBeatGenerator heartBeat(ke::Time::milliseconds(5000));
        ke::FrameRateCounter fpsCounter(1000);

        while(this->isEventLoopRunning)
        {
            const auto frameTime = stopwatch.getElapsed();
            stopwatch.restart();

            fpsCounter.push(frameTime);
            ::eventLoopFps.store((float)fpsCounter.getAverageFps(), std::memory_order_relaxed);

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
            auto sfWindow = static_cast<sf::RenderWindow*>(this->mainWindow->get());
            assert(sfWindow);
            while (sfWindow->pollEvent(event))
            {
                ke::EventManager::enqueue(ke::makeEvent<ke::SfmlEvent>(event));
                switch (event.type)
                {
                case sf::Event::EventType::MouseButtonPressed:
                case sf::Event::EventType::MouseButtonReleased:
                case sf::Event::EventType::KeyPressed:
                case sf::Event::EventType::KeyReleased:
                case sf::Event::EventType::TextEntered:
                case sf::Event::EventType::Resized:
                {
                    auto keEvent = SfmlEventTranslator::translate(event, sfWindow);
                    if (keEvent)
                    {
                        ke::EventManager::enqueue(std::move(keEvent));
                    }
                    break;
                }

                case sf::Event::EventType::Closed:
                    Log::instance()->info("Normal exit requested.");
                    ke::EventManager::enqueue(ke::makeEvent<AppExitRequestedEvent>());
                    break;

                default: break;
                }
            }
#endif

            // set title with stats.
            const auto memO = std::memory_order_relaxed;
            using LLU = unsigned long long;
            std::snprintf(::windowTitleTextBuffer, sizeof(::windowTitleTextBuffer),
                "KEngine - FPS(%4.1f, %4.1f[%4.1f/ms], %4.1f[%4.1f/ms]), GraphicsCommands(%llu), DrawCalls(%llu), Events(%llu).",
                ::eventLoopFps.load(memO),
                ::logicLoopFps.load(memO), ::logicLoopFrameTimeMs.load(memO),
                ::graphicsLoopFps.load(memO), ::renderLoopFrameTimeMs.load(memO),
                (LLU)::graphicsCommandCount.load(memO), (LLU)::graphicsDrawCallCount.load(memO),
                (LLU)::eventManagerEventCount.load(memO));
            this->mainWindow->setTitle(::windowTitleTextBuffer);


            if (heartBeat)
            {
                ke::Log::instance()->info("Event loop heart beat");
            }

            std::this_thread::sleep_for(EVENT_THREAD_SLEEP_DURATION);
        }

        ke::Log::instance()->info("KEngine event loop exited.");

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

            // 
            // Setup window and render system.
            // 
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

            if (!this->renderSystem->initialise())
            {
                Log::instance()->critical("Failuring initialising render system on thread {}.",
                    std::hash<std::thread::id>()(std::this_thread::get_id()));
                return;
            }

            mainWindow->display();


            ke::Log::instance()->info("Entering KEngine logic loop ...");

            ke::StopWatch stopwatch;
            ke::Time cumulativeLoopTime;
            ke::HeartBeatGenerator heartBeat(ke::Time::milliseconds(5000));
            ke::FrameRateCounter logicFpsCounter(1000);
            ke::FrameRateCounter renderFpsCounter(500);
            this->isGraphicsLoopRunning = true;
            this->isLogicLoopRunning = true;

            while(this->isLogicLoopRunning)
            {
                const auto frameTime = stopwatch.getElapsed();
                stopwatch.restart();
                cumulativeLoopTime += frameTime;

                logicFpsCounter.push(frameTime);
                ::logicLoopFps.store((float)logicFpsCounter.getAverageFps(), std::memory_order_relaxed);
                ::logicLoopFrameTimeMs.store(frameTime.asNanoseconds()/1000000.0, std::memory_order_relaxed);
                ::eventManagerEventCount.store(ke::EventManager::instance()->getEventCount(), std::memory_order_relaxed);

//                if (frameTime < ::LOGIC_THREAD_TARGET_FRAMETIME)
//                {
//                    const auto difference = ::LOGIC_THREAD_TARGET_FRAMETIME - frameTime;
//                    std::this_thread::sleep_for(std::chrono::nanoseconds(difference.asNanoseconds()));
//                }
//                std::this_thread::sleep_for(std::chrono::milliseconds(::LOGIC_THREAD_TARGET_FRAMETIME.asMilliseconds()));

                //
                // update logic
                //
                while (cumulativeLoopTime >= ::LOGIC_UPDATE_FIXED_TIMESPAN)
                {
                    cumulativeLoopTime -= ::LOGIC_UPDATE_FIXED_TIMESPAN;

                    ke::EventManager::enqueue(ke::makeEvent<LogicLoopFrameEvent>(::LOGIC_UPDATE_FIXED_TIMESPAN));
                    ke::EventManager::update();

                    //
                    this->resourceManager->update();
                    
                    ke::EventManager::update();

                    // update logic.
                    this->appLogic->update(::LOGIC_UPDATE_FIXED_TIMESPAN);
                }

                //
                //
                //
                renderFpsCounter.push(frameTime);
                ::graphicsLoopFps.store((float)renderFpsCounter.getAverageFps(), std::memory_order_relaxed);
                ::renderLoopFrameTimeMs.store(frameTime.asMicroseconds() / 1000.0, std::memory_order_relaxed);
                ke::EventManager::enqueue(ke::makeEvent<GraphicsLoopFrameEvent>(frameTime));

                //
                // prepare and dispatch render commands
                //
                this->renderSystem->prepareCommands(this->appLogic->getCurrentHumanView()->getScene());
                auto cmdCount = this->renderSystem->dispatchCommands();
                ::graphicsCommandCount.store(cmdCount, std::memory_order_relaxed);

                this->renderSystem->updateOnRenderLoop(frameTime);

                // process the oldest render command list from the render commands queue
                // e.g. culling, ordering, etc...
                // if queue is empty then interpolate before render.
                this->renderSystem->processCommands(frameTime);
                const auto drawCallCount = this->renderSystem->render();
                ::graphicsDrawCallCount.store(drawCallCount > 0 ? drawCallCount : ::graphicsDrawCallCount.load(), std::memory_order_relaxed);

                //
                // debug/diagnostic stuff
                //
                if (heartBeat)
                {
                    ke::Log::instance()->info("Logic loop heart beat");
                }

                if (frameTime < ke::Time::milliseconds(1))
                    std::this_thread::sleep_for(LOGIC_THREAD_SLEEP_DURATION);
            }

            this->renderSystem->shutdown();
            this->mainWindow->setThreadCurrent(false);

            // we only flag event loop for termination here as some logics may still require the event loop to be alive.
            this->isEventLoopRunning = false;

            ke::Log::instance()->info("KEngine logic loop exited.");

            //
            // clean up.
            //
            this->getLogic()->getSystemManager()->clear();
        });       
    }

    void App::setLogic(ke::AppLogicUptr && p_appLogic)
    {
        this->appLogic = std::move(p_appLogic);
    }

    void App::initExec()
    {
        spdlog::set_async_mode(1048576); // magic number from spdlog repo.

        ke::Log::instance()->info("Creating resource manager ...");
        this->resourceManager = std::make_unique<ResourceManager>();
        ke::Log::instance()->info("Creating resource manager ... DONE");

        ke::Log::instance()->info("Creating logic and views ...");
        this->createLogicAndViews();
        ke::Log::instance()->info("Creating logic and views ... DONE");

        assert(this->getLogic());
        assert(this->getLogic()->getCurrentHumanView());
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
