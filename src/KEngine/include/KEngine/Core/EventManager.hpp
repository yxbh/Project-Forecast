#pragma once

#include "KEngine/Core/EventManagerImpl.hpp"

namespace ke
{

    class EventManager
    {
    public:
        static inline ke::priv::IEventManagerImpl * instance()
        {
            using namespace ke::priv;

            static EventManagerUptr instance(new EventManagerImpl);
            return instance.get();
        }

        static inline void update(const ke::Time durationLimit = ke::Time::Zero)
        {
            ke::EventManager::instance()->update(durationLimit);
        }

        template <typename Event_T, typename Listener_T, typename ListenerMemFunc_T>
        static inline void registerListener(Listener_T * const instance, ListenerMemFunc_T memFunc)
        {
            ke::EventManager::instance()->registerListener(Event_T::EVENT_TYPE, ke::EventDelegate(instance, memFunc));
        }

        template <typename Event_T, typename ListenerStaticFunc_T>
        static inline void registerListener(ListenerStaticFunc_T staticFunc)
        {
            ke::EventManager::instance()->registerListener(Event_T::EVENT_TYPE, ke::EventDelegate(staticFunc));
        }

        template <typename Event_T, typename Listener_T, typename ListenerMemFunc_T>
        static inline void deregisterListener(Listener_T * const instance, ListenerMemFunc_T memFunc)
        {
            ke::EventManager::instance()->deregisterListener(Event_T::EVENT_TYPE, ke::EventDelegate(instance, memFunc));
        }

        template <typename Event_T, typename ListenerStaticFunc_T>
        static inline void deregisterListener(ListenerStaticFunc_T staticFunc)
        {
            ke::EventManager::instance()->deregisterListener(Event_T::EVENT_TYPE, ke::EventDelegate(staticFunc));
        }

        static inline void queue(ke::EventSptr && event)
        {
            ke::EventManager::instance()->queue(std::forward<ke::EventSptr>(event));
        }
    };

}
