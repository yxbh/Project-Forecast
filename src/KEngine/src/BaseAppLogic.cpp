#include "KEngine/BaseAppLogic.hpp"

#include "KEngine/Entity/Components/EntityRenderableComponents.hpp"
#include "KEngine/Entity/ComponentBuilders/EntityRenderableComponentBuilder.hpp"

#include "KEngine/Systems/AppConsoleSystem.hpp"
#include "KEngine/Systems/ImguiSystem.hpp"

#include <cassert>

namespace ke
{

    BaseAppLogic::BaseAppLogic()
    {
        this->entityFactory.registerComponentBuilder<ke::EntityRenderableComponentBuilder>(ke::EntityRenderableComponent::NAME);

        auto ret = this->systemManager.addSystem(std::make_unique<AppConsoleSystem>());
        assert(ret);
        ret = this->systemManager.addSystem(std::make_unique<ImguiSystem>());
        assert(ret);
    }

    void BaseAppLogic::onUpdate(ke::Time elapsedTime)
    {
        // update all the systems
        this->systemManager.update(elapsedTime);

        // update all the entities
        this->entityManager.update(elapsedTime);

        // update all the views
        for (auto view : this->appViews) view->update(elapsedTime);
    }

    void BaseAppLogic::addView(ke::AppViewSptr view, bool setAsCurrent)
    {
        assert(view);

        this->appViews.push_back(view);

        if (setAsCurrent && view->isHumanView())
        {
            this->currentHumanView = view.get();
            // TODO : broadcast current view changed event.
        }
        // if there's no current view, set it as current view anyway.
        else if (!this->currentHumanView && view->isHumanView())
        {
            this->currentHumanView = view.get();
        }
    }

}