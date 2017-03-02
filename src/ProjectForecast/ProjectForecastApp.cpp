#include "ProjectForecastApp.hpp"

#include "Systems/DebugDrawSystem.hpp"
#include "Systems/EntityTransformControlSystem.hpp"
#include "Views/HumanView.hpp"

#include <cassert>

namespace pf
{

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
        ret = systemManager->addSystem(std::make_unique<pf::DebugDrawSystem>());
        assert(ret);
    }

}