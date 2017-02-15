#include "ProjectForecastApp.hpp"

#include "Systems/DebugDrawSystem.hpp"
#include "Views/HumanView.hpp"

#include <cassert>

namespace pf
{

    void ProjectForecastApp::createLogicAndViews()
    {
        // create the logic
        this->setLogic(std::make_unique<pf::ProjectForecastAppLogic>());

        // create and attach the systems
        auto systemManager = this->getLogic()->getSystemManager();
        assert(systemManager);
        auto ret = systemManager->addSystem(std::make_unique<pf::DebugDrawSystem>());
        assert(ret);

        // create the views
        this->getLogic()->addView(std::make_shared<pf::HumanView>());
    }

}