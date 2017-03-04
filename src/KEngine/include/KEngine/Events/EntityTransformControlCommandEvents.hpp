#pragma once

#include "KEngine/Interface/IEvent.hpp"

#include "KEngine/Common/Transform2D.hpp"

namespace ke
{

    /// <summary>
    /// A command event for setting an entity's transform.
    /// </summary>
    class EntityTransformControlCommandEvent : public ke::IEvent
    {
        KE_DEFINE_EVENT_COMMON_PROPERTIES(EntityTransformControlCommandEvent, 0x33E359F3)

    public:
        EntityTransformControlCommandEvent(
            ke::EntityId p_entityId = ke::INVALID_ENTITY_ID,
            const ke::Transform2D & p_transform = ke::Transform2D())
            : entityId(p_entityId), transform(p_transform) {}

        inline void setEntityId(ke::EntityId p_entityId)
        {
            this->entityId = p_entityId;
        }
        
        inline ke::EntityId getEntityId() const
        {
            return this->entityId;
        }

        inline void setTransform(const ke::Transform2D & p_transform)
        {
            this->transform = p_transform;
        }

        inline const ke::Transform2D & getTransform() const
        {
            return this->transform;
        }

        inline ke::Transform2D & getTransform()
        {
            return this->transform;
        }

        virtual Sptr makeCopy() const override
        {
            return ke::makeEvent<EntityTransformControlCommandEvent>(this->entityId, this->transform);
        }

    protected:
        ke::EntityId entityId;
        ke::Transform2D transform;
    };


    /// <summary>
    /// A command event for setting an entity's delta transform.
    /// </summary>
    class EntityTransformDeltaControlCommandEvent : public ke::EntityTransformControlCommandEvent
    {
        KE_DEFINE_EVENT_COMMON_PROPERTIES(EntityTransformDeltaControlCommandEvent, 0x12BD7421)

    public:
        using ke::EntityTransformControlCommandEvent::EntityTransformControlCommandEvent;

        virtual Sptr makeCopy() const override
        {
            return ke::makeEvent<EntityTransformDeltaControlCommandEvent>(this->entityId, this->transform);
        }
    };

}
