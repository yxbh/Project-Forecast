#include "ProjectForecastApp.hpp"

#include "Systems/DebugDrawSystem.hpp"
#include "Systems/DebugSceneGridSystem.hpp"
#include "Systems/EntityTransformControlSystem.hpp"
#include "Systems/GMSRoomManagementSystem.hpp"
#include "Systems/GMSAssetResourceManagementSystem.hpp"
#include "Systems/PlayerCameraControlSystem.hpp"

#include "Views/HumanView.hpp"
#include "CommandLineOptions.hpp"

#include <KEngine/Events/OtherGraphicsEvents.hpp>

#include <KEngine/Core/EventManager.hpp>
#include <KEngine/Log/Log.hpp>

#include <cassert>

namespace pf
{
    ProjectForecastApp::ProjectForecastApp(const int p_argc, char ** const p_argv)
        : ke::App(p_argc, p_argv)
    {
        this->cmdOptions.add_options("ProjectForecast")
            (pf::cli::ExecAssetsPath, "Set asset folder path.", cxxopts::value<ke::String>()->default_value("./assets/"));
    }

    void ProjectForecastApp::createLogicAndViews()
    {
        // create the logic
        this->setLogic(std::make_unique<pf::ProjectForecastAppLogic>());

        // create the views
        this->getLogic()->addView(std::make_shared<pf::HumanView>());

        // create and attach the systems
        auto systemManager = this->getLogic()->getSystemManager();
        assert(systemManager);
        bool ret = false;
        ret = systemManager->addSystem(std::make_unique<pf::EntityTransformControlSystem>());
        assert(ret);
        ret = systemManager->addSystem(std::make_unique<pf::PlayerCameraControlSystem>());
        assert(ret);
        ret = systemManager->addSystem(std::make_unique<pf::DebugDrawSystem>());
        assert(ret);
        ret = systemManager->addSystem(std::make_unique<pf::DebugSceneGridSystem>());
        assert(ret);
        ret = systemManager->addSystem(std::make_unique<pf::GMSRoomManagementSystem>());
        assert(ret);
        ret = systemManager->addSystem(std::make_unique<pf::GMSAssetResourceManagementSystem>());
        assert(ret);
    }

    ProjectForecastAppLogic::ProjectForecastAppLogic()
    {
        ke::EventManager::registerListener<ke::SceneNodeCreatedEvent>(this, &ProjectForecastAppLogic::autoAddNewSceneNodeToCurrentViewSceneOnNodeCreate);
    }

    ProjectForecastAppLogic::~ProjectForecastAppLogic()
    {
        ke::EventManager::deregisterListener<ke::SceneNodeCreatedEvent>(this, &ProjectForecastAppLogic::autoAddNewSceneNodeToCurrentViewSceneOnNodeCreate);
    }

    void ProjectForecastAppLogic::autoAddNewSceneNodeToCurrentViewSceneOnNodeCreate(ke::EventSptr event)
    {
        auto newSceneNodeEvent = dynamic_cast<ke::SceneNodeCreatedEvent*>(event.get());
        if (newSceneNodeEvent)
        {
            if (auto currentView = this->getCurrentHumanView(); currentView)
            {
                if (auto scene = currentView->getScene(); scene)
                {
                    scene->addNode(newSceneNodeEvent->getSceneNode());
                }
            }
        }
        else
        {
            ke::Log::instance()->error("ProjectForecastAppLogic::autoAddNewSceneNodeToCurrentViewSceneOnNodeCreate received unexpected: {}", event->getName());
        }
    }

}