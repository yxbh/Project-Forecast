#pragma once

#include "KEngine/Interface/IEvent.hpp"
#include "KEngine/Common/Point2D.hpp"

namespace pf
{

    class RequestDrawDebugDotEvent : public ke::IEvent
    {
        KE_DEFINE_EVENT_COMMON_PROPERTIES(RequestDrawDebugDotEvent, 0x006B92AF)

    public:
        RequestDrawDebugDotEvent(ke::Point2D position = {}, float radius = 1.0f)
            : position(position), radius(radius)
        {

        }

        virtual ke::EventSptr makeCopy() const override
        {
            return ke::makeEvent<RequestDrawDebugDotEvent>(this->position, this->radius);
        }

        inline const ke::Point2D & getPosition() const { return this->position; }

    private:
        ke::Point2D position;
        float radius = 0.0f;

    };

}
