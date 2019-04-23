#pragma once

#include "KEngine/Interfaces/IEvent.hpp"

namespace ke
{

    class RequestShowAppConsoleEvent final : public ke::IEvent
    {
        KE_DEFINE_EVENT_COMMON_PROPERTIES(RequestShowAppConsoleEvent, 0xE9996809)

    public:
        using IEvent::IEvent;

        virtual ke::EventSptr makeCopy() const final
        {
            return ke::makeEvent<RequestShowAppConsoleEvent>();
        }
    };

    class RequestHideAppConsoleEvent final : public ke::IEvent
    {
        KE_DEFINE_EVENT_COMMON_PROPERTIES(RequestHideAppConsoleEvent, 0xF7074ACC)

    public:
        using IEvent::IEvent;

        virtual ke::EventSptr makeCopy() const final
        {
            return ke::makeEvent<RequestHideAppConsoleEvent>();
        }
    };

    class RequestToggleAppConsoleDisplayEvent final : public ke::IEvent
    {
        KE_DEFINE_EVENT_COMMON_PROPERTIES(RequestToggleAppConsoleDisplayEvent, 0xDCF11AB4)

    public:
        using IEvent::IEvent;

        virtual ke::EventSptr makeCopy() const final
        {
            return ke::makeEvent<RequestToggleAppConsoleDisplayEvent>();
        }
    };

}  // ns ke