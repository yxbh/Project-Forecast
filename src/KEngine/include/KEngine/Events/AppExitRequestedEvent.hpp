#pragma once

#include "KEngine/Interface/IEvent.hpp"

namespace ke
{

    /// <summary>
    /// An event that signifies that application exit has been requested.
    /// </summary>
    class AppExitRequestedEvent : public ke::IEvent
    {
    public:
        static const EventType TYPE = 0xE63141C0;

        virtual ke::EventType getType() const override
        {
            return TYPE;
        }

        virtual ke::String getName() const final
        {
            return KE_TEXT("AppExitRequestedEvent");
        }

        virtual ke::EventSptr makeCopy() const final
        {
            return ke::makeEvent<AppExitRequestedEvent>();
        }

    private:
    };

}
