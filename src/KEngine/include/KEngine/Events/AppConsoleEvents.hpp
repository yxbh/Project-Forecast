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

    class AppConsoleCommandLineEvent final : public ke::IEvent
    {
        KE_DEFINE_EVENT_COMMON_PROPERTIES(AppConsoleCommandLineEvent, 0xEFFE7687)

    public:
        AppConsoleCommandLineEvent(const ke::String & p_cmdLine)
            : commandLine(p_cmdLine) {}

        virtual ke::EventSptr makeCopy() const final
        {
            return ke::makeEvent<AppConsoleCommandLineEvent>(this->commandLine);
        }

        inline const ke::String & getCommandLine() const
        {
            return this->commandLine;
        }

    private:
        ke::String commandLine;
    };

    class AppConsoleCommandLineResponseEvent final : public ke::IEvent
    {
        KE_DEFINE_EVENT_COMMON_PROPERTIES(AppConsoleCommandLineResponseEvent, 0xEFFE7687)

    public:
        AppConsoleCommandLineResponseEvent(const ke::String & p_response)
            : response(p_response) {}

        virtual ke::EventSptr makeCopy() const final
        {
            return ke::makeEvent<AppConsoleCommandLineResponseEvent>(this->response);
        }

        inline const ke::String & getResponse() const
        {
            return this->response;
        }

    private:
        ke::String response;
    };

}  // ns ke