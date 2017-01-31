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

        /// <summary>
        /// Add an event delegate for the specified event type.
        /// </summary>
        /// <param name="p_EventType"></param>
        /// <param name="p_Delegate">The event delegate that will be called when an event of the specified is received.</param>
        /// <returns>false if the delegate is already listening to the specified event type.</returns>
        bool registerListener(const ke::EventType p_EventType, const ke::EventDelegate & p_Delegate);
        
        /// <summary>
        /// Remove all listeners for the specified EventType.
        /// </summary>
        /// <param name="p_EventType"></param>
        /// <returns></returns>
        virtual bool deregisterAllListeners(const ke::EventType p_EventType) = 0;
        
        /// <summary>
        /// Deregister a listener/delegate with the given EntityID from a specific event type.
        /// </summary>
        /// <param name="p_EventType"></param>
        /// <param name="p_Delegate"></param>
        /// <returns></returns>
        virtual bool deregisterListener(const ke::EventType p_EventType, const ke::EventDelegate & p_Delegate) = 0;
        
        /// <summary>
        /// Add an event into the Event queue.
        /// </summary>
        /// <param name="p_spNewEvent"></param>
        virtual void queue(ke::EventSptr p_spNewEvent) = 0;
        
        /// <summary>
        /// Call all delegates listening to that event now.
        /// </summary>
        /// <param name="p_Event"></param>
        /// <returns>false if Event is not handled for whatever reason.</returns>
        virtual bool dispatchNow(ke::EventSptr p_Event) = 0;

        /// <summary>
        /// Remove first instance of an event of a particular type.
        /// </summary>
        /// <param name="p_EventType"></param>
        /// <param name="p_RemoveAllSame">If true all events of the specified type will be removed. Does not remove event from the thread safe queue.</param>
        /// <returns></returns>
        virtual bool removeEvent(const EventType p_EventType, const bool p_RemoveAllSame = false) = 0;

        /// <summary>
        /// Deliver all events to their registered listeners.
        /// </summary>
        /// <param name="p_DurationLimit"></param>
        /// <returns>EventProcessResult indicating status of event processing.</returns>
        virtual EventProcessResult update(const ke::Time p_DurationLimit = ke::Time::Zero) = 0;
        //virtual EventProcessResult update(const ke::Time p_Duration, const ke::Time p_DurationLimit = Time::Zero) = 0;
    };

    inline IEventManagerImpl::~IEventManagerImpl() {}

}

namespace ke
{

    using EventManagerSptr = std::shared_ptr<ke::priv::IEventManagerImpl>;
    using EventManagerUptr = std::unique_ptr<ke::priv::IEventManagerImpl>;
    using EventManagerWptr = std::weak_ptr<ke::priv::IEventManagerImpl>;

}
