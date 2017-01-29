#pragma once

#include "KEngine/Common/Delegate.hpp"
#include "KEngine/Common/Time.hpp"
#include "KEngine/Interface/IEvent.hpp"

namespace ke
{
    enum EventProcessResult
    {
        ALL_EVENTS_PROCESSED,
        SOME_EVENTS_PROCESSED
    };

}

namespace ke::priv
{

    class IEventManagerImpl
    {
    public:
        virtual ~IEventManagerImpl() = 0;

        /**
        Add an event delegate for the specified event type.
        @return false if the delegate is already listening to the specified event type.
        */
        bool registerListener(const ke::EventType p_EventType, const ke::EventDelegate & p_Delegate);
        /** Remove all listeners for the specified EventType. */
        virtual bool deregisterAllListeners(const ke::EventType p_EventType) = 0;
        /** Deregister a listener/delegate with the given EntityID from a specific event type. */
        virtual bool deregisterListener(const ke::EventDelegate & p_Handle, const ke::EventType p_CmdType) = 0;
        /** Add an event into the Event queue. */
        virtual void queue(ke::EventSptr p_spNewEvent) = 0;
        /** Call all delegates listening to that event now. Return false if Event is not handled for whatever reason. */
        virtual bool dispatchNow(ke::EventSptr p_Event) = 0;
        /** Remove first instance of an event of a particular type. If true all event belonging to the type will be removed. Does not remove event from the thread safe queue.*/
        virtual bool removeEvent(const EventType p_EventType, const bool p_RemoveAllSame = false) = 0;

        /** Fire off all Commands (With in limit, if one is set), returns EventProcessResult indicating status of event processing. */
        //virtual EventProcessResult update(const ke::Time p_Duration, const ke::Time p_DurationLimit = Time::Zero) = 0;
        virtual EventProcessResult update(const ke::Time p_DurationLimit = ke::Time::Zero) = 0;
    };

    inline IEventManagerImpl::~IEventManagerImpl() {}

}

namespace ke
{

    using EventManagerSptr = std::shared_ptr<ke::priv::IEventManagerImpl>;
    using EventManagerUptr = std::unique_ptr<ke::priv::IEventManagerImpl>;
    using EventManagerWptr = std::weak_ptr<ke::priv::IEventManagerImpl>;

}
