#pragma once

#include "KEngine/Interfaces/IEvent.hpp"

namespace ke
{

    /// <summary>
    /// An event that signifies that application exit has been requested.
    /// </summary>
    class AppExitRequestedEvent : public ke::IEvent
    {
        KE_DEFINE_EVENT_COMMON_PROPERTIES(AppExitRequestedEvent, 0xE63141C0)

    public:
        virtual ke::EventSptr makeCopy() const final
        {
            return ke::makeEvent<AppExitRequestedEvent>();
        }

    };

}
