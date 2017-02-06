#include "KEngine/Common/StopWatch.hpp"
#include "KEngine/Core/EventManagerImpl.hpp"
#include "KEngine/Log/Log.hpp"

#include <cassert>

namespace ke::priv
{

    EventManagerImpl::~EventManagerImpl(void)
    {
        m_Listeners.clear();
        m_EventQueue.clear();
    }

    bool EventManagerImpl::registerListener(const ke::EventType p_EventType, const ke::EventDelegate & p_Delegate)
    {
        ListenerList & listeners(m_Listeners[p_EventType]);
        for (const DelegateType & listener : listeners)
            if (listener == p_Delegate) // check if already in the list for specified EventType.
            {
                Log::instance()->warn("listener already registered to event type: {}", p_EventType);
                return false;
            }

        listeners.push_back(p_Delegate);
        return true;
    }

    bool EventManagerImpl::deregisterListener(const ke::EventType p_EventType, const ke::EventDelegate & p_Delegate)
    {
        if (m_Listeners.find(p_EventType) == m_Listeners.end())
        {
            Log::instance()->warn("no listener is registered to listen to event type: {}", p_EventType);
            return false;
        }
        ListenerList & listeners(m_Listeners[p_EventType]);
        for (ListenerList::const_iterator cit(listeners.begin()); cit != listeners.end(); ++cit)
        {
            if (*cit == p_Delegate)
            {
                listeners.erase(cit);
                return true;
            }
        }
        Log::instance()->warn("the listener is not registered to listen to event type: {}", p_EventType);
        return false;
    }

    bool EventManagerImpl::deregisterAllListeners(const ke::EventType p_EventType)
    {
        if (m_Listeners.find(p_EventType) == m_Listeners.end())
        {
            Log::instance()->warn("no listeners are registered to listen to event type: {}", p_EventType);
            return false;
        }
        m_Listeners[p_EventType].clear();
        return true;
    }

    bool EventManagerImpl::dispatchNow(ke::EventSptr p_Event)
    {
        assert(p_Event->getType() != ke::IEvent::INVALID_EVENT);
        EventListenersMap::iterator it = m_Listeners.find(p_Event->getType());

        if (it == m_Listeners.end())
            return false;

        const ListenerList & listeners = it->second;
        bool handled_event(false);
        for (auto & listener : listeners)
        {
            listener(p_Event);
            handled_event = true;
        }
        return handled_event;
    }

    void EventManagerImpl::queue(ke::EventSptr p_spNewEvent)
    {
        m_ThreadSafeEventQueue.push(p_spNewEvent);
    }

    bool EventManagerImpl::removeEvent(const ke::EventType p_EventType, const bool p_RemoveAllSame)
    {
        bool remove_success(false);
        EventListenersMap::iterator listeners_it(m_Listeners.find(p_EventType));
        if (listeners_it == m_Listeners.end())
            return false;

        EventQueue::iterator it = m_EventQueue.begin();
        while (it != m_EventQueue.end())
        {
            auto it_for_delete = it; ++it; // erase() invalidates iterator. Make a copy and iterate original first.

                                           // different type, move on to next.
            if (p_EventType != (*it_for_delete)->getType()) continue;

            m_EventQueue.erase(it_for_delete); // type match, so remove.
            remove_success = true;

            if (!p_RemoveAllSame) break;
        }
        return remove_success;
    }

    EventProcessResult EventManagerImpl::update(const ke::Time p_ExcutionDurationLimit)
    {
        bool has_duraton_limit = p_ExcutionDurationLimit == ke::Time::Zero ? false : true;

        ke::StopWatch stopwatch; stopwatch.restart();

        ke::EventSptr moving_event_ptr;
        while (m_ThreadSafeEventQueue.poll(moving_event_ptr))
            m_EventQueue.push_back(moving_event_ptr);

        ke::EventSptr event_ptr;
        while (!m_EventQueue.empty())
        {
            if (has_duraton_limit && stopwatch.getElapsed() >= p_ExcutionDurationLimit) // if elapsed time if duration limit setted.
                break;

            event_ptr = m_EventQueue.front(); m_EventQueue.pop_front();

            auto listeners_it = m_Listeners.find(event_ptr->getType());
            if (listeners_it == m_Listeners.end()) // no listeners registered for this type.
                continue;

            ListenerList & list(listeners_it->second);
            for (auto & listener : list)
                listener(event_ptr);    // call delegate
        }

        if (m_ThreadSafeEventQueue.isEmpty())
            return ke::EventProcessResult::ALL_EVENTS_PROCESSED;
        return ke::EventProcessResult::SOME_EVENTS_PROCESSED;
    }

}
