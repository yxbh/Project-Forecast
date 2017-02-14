#include "KEngine/BaseAppLogic.hpp"

namespace ke
{

    void BaseAppLogic::update(ke::Time elapsedTime)
    {
        // update all the systems

        // update all the entities
        this->entityManager.update(elapsedTime);

        // update all the views
        for (auto view : this->appViews) view->update(elapsedTime);
    }

    void BaseAppLogic::addView(ke::AppViewSptr view, bool setAsCurrent)
    {
        this->appViews.push_back(view);

        if (setAsCurrent)
        {
            currentAppView = view.get();
            // TODO : broadcast current view changed event.
        }
        // if there's no current view, set it as current view anyway.
        else if (!this->currentAppView)
        {
            currentAppView = view.get();
        }
    }

}