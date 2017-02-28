#pragma once

#include "KEngine/Interface/IEventManagerImpl.hpp"

#include <deque>
#include <unordered_map>
#include <vector>

namespace ke::priv
{
    
    class EventManagerImpl : public ke::priv::IEventManagerImpl
    {
    public:
        virtual ~EventManagerImpl();

        virtual bool registerListener(const ke::EventType p_EventType, const ke::EventDelegate & p_Delegate) final;

        virtual bool deregisterAllListeners(const ke::EventType p_EventType) final;

        virtual bool deregisterListener(const ke::EventType p_EventType, const ke::EventDelegate & p_Delegate) final;

        virtual void enqueue(ke::EventSptr p_spNewEvent) final;

        virtual bool dispatchNow(ke::EventSptr p_Event) final;

        virtual bool removeEvent(const ke::EventType p_EventType, const bool p_RemoveAllSame = false) final;

        virtual ke::EventProcessResult update(const ke::Time p_DurationLimit = ke::Time::Zero) final;

        virtual size_t getEventCount() const final;

    public:
        using DelegateType      = ke::EventDelegate;
        using ListenerList      = std::vector<DelegateType>;
        using EventListenersMap = std::unordered_map<ke::EventType, ListenerList>;
        using EventQueue        = std::deque<ke::EventSptr>;

    private:
        ke::EventQueue              m_ThreadSafeEventQueue;
        EventQueue                  m_EventQueue;
        EventListenersMap           m_Listeners;

    };

}
