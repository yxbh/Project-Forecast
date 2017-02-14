#include "ProjectForecastApp.hpp"

#include "Views/HumanView.hpp"

namespace pf
{

    void ProjectForecastApp::createLogicAndViews()
    {
        // create the logic
        this->setLogic(std::make_unique<pf::ProjectForecastAppLogic>());

        // create the views
        this->getLogic()->addView(std::make_shared<pf::HumanView>());
    }

}