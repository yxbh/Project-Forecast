#include "KEngine/Views/HumanView.hpp"

#include "KEngine/Graphics/Scene.hpp"


namespace ke
{

    void HumanView::attachEntity(ke::EntityId entityId)
    {
        this->currentEntityId = entityId;
    }

}