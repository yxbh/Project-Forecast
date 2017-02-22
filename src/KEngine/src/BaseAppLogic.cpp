#include "KEngine/BaseAppLogic.hpp"

#include "KEngine/Entity/Components/EntityRenderableComponents.hpp"
#include "KEngine/Entity/ComponentLoaders/EntityRenderableComponentLoader.hpp"

namespace ke
{

    BaseAppLogic::BaseAppLogic()
    {
        this->entityFactory.registerComponentJsonLoader<ke::EntityRenderableComponentLoader>(ke::EntityRenderableComponent::NAME);
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