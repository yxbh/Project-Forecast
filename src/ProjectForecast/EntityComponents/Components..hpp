#pragma once

#include <KEngine/Entity/Components/EntityRenderableComponents.hpp>

#include <KEngine/Interfaces/IEntity.hpp>
#include <KEngine/Interfaces/IEntityComponent.hpp>

namespace pf
{
    
    class GMSRoomRenderableComponent : public ke::EntityRenderableComponent
    {
        KE_DEFINE_ENTITY_COMPONENT_COMMON_PROPERTIES(GMSRoomRenderableComponent, 0x3f672885)

    public:
        using ke::EntityRenderableComponent::EntityRenderableComponent;


    };

}