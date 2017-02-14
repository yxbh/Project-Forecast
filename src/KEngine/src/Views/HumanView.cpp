#include "KEngine/Views/HumanView.hpp"

namespace ke
{

    void HumanView::attachEntity(ke::EntityId entityId)
    {
        this->currentEntityId = entityId;
    }

}