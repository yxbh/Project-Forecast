#pragma once

#include "KEngine/Interface/IEvent.hpp"

namespace pf
{

    class RequestDrawDebugDotEvent : public ke::IEvent
    {
        KE_DEFINE_EVENT_COMMON_PROPERTIES(RequestDrawDebugDotEvent, 0x006B92AF)
    public:
        virtual ke::EventSptr makeCopy() const override
        {
            return ke::makeEvent<RequestDrawDebugDotEvent>();
        }
    };

}
