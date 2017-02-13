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

}