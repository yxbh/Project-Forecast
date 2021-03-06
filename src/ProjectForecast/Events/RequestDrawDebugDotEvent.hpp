#pragma once

#include "KEngine/Interfaces/IEvent.hpp"
#include "KEngine/Common/Point2D.hpp"
#include "KEngine/Common/Color.hpp"

namespace pf
{

    class RequestDrawDebugDotEvent : public ke::IEvent
    {
        KE_DEFINE_EVENT_COMMON_PROPERTIES(RequestDrawDebugDotEvent, 0x006B92AF)

    public:
        RequestDrawDebugDotEvent(ke::Point2DFloat position = {}, float radius = 1.0f, ke::Color color = ke::Color::GREEN)
            : position(position), radius(radius), color(color)
        {
        }

        virtual ke::EventSptr makeCopy() const override
        {
            return ke::makeEvent<RequestDrawDebugDotEvent>(this->position, this->radius, this->color);
        }

        inline const ke::Point2DFloat & getPosition() const { return this->position; }

    private:
        ke::Point2DFloat position;
        float radius = 0.0f;
        ke::Color color{0,0,0, 255};

    };

}
