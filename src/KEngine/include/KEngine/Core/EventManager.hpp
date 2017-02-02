#pragma once

#include "KEngine/Core/EventManagerImpl.hpp"

#include <cassert>

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

        /// <summary>
        /// Deliver all events to their registered listeners.
        /// </summary>
        /// <param name="durationLimit"></param>
        /// <returns>EventProcessResult indicating status of event processing.</returns>
        static inline void update(const ke::Time durationLimit = ke::Time::Zero)
        {
            ke::EventManager::instance()->update(durationLimit);
        }

        /// <summary>
        /// Add an object as an event listener with the given member function for the specified event type.
        /// </summary>
        /// <param name="instance">A pointer to the listener.</param>
        /// <param name="memFunc">A member function pointer that matches the signature of ke::EventDelegate.</param>
        /// <returns>false if the delegate is already listening to the specified event type.</returns>
        template <typename Event_T, typename Listener_T, typename ListenerMemFunc_T>
        static inline bool registerListener(Listener_T * const instance, ListenerMemFunc_T memFunc)
        {
            assert(instance);
            return ke::EventManager::instance()->registerListener(Event_T::TYPE, ke::EventDelegate(instance, memFunc));
        }

        template <typename Event_T, typename ListenerStaticFunc_T>
        /// <summary>
        /// Add a static function (can be a static class function) as an event listener for the specified event type.
        /// </summary>
        /// <param name="staticFunc">A static function pointer.</param>
        /// <returns>false if the delegate is already listening to the specified event type.</returns>
        static inline bool registerListener(ListenerStaticFunc_T staticFunc)
        {
            return ke::EventManager::instance()->registerListener(Event_T::TYPE, ke::EventDelegate(staticFunc));
        }

        /// <summary>
        /// Deregister a listener/delegate from the specified event type.
        /// </summary>
        /// <param name="instance">A pointer to the listener.</param>
        /// <param name="memFunc">A member function pointer that matches the signature of ke::EventDelegate.</param>
        /// <returns>true if the function was successful.</returns>
        template <typename Event_T, typename Listener_T, typename ListenerMemFunc_T>
        static inline bool deregisterListener(Listener_T * const instance, ListenerMemFunc_T memFunc)
        {
            return ke::EventManager::instance()->deregisterListener(Event_T::TYPE, ke::EventDelegate(instance, memFunc));
        }

        /// <summary>
        /// Deregister a listener/delegate from the specified event type.
        /// </summary>
        /// <param name="staticFunc">A static function pointer.</param>
        /// <returns>true if the function was successful.</returns>
        template <typename Event_T, typename ListenerStaticFunc_T>
        static inline bool deregisterListener(ListenerStaticFunc_T staticFunc)
        {
            return ke::EventManager::instance()->deregisterListener(Event_T::TYPE, ke::EventDelegate(staticFunc));
        }

        /// <summary>
        /// Queue an event dispatching when update() is called.
        /// </summary>
        /// <param name="p_spNewEvent"></param>
        static inline void queue(ke::EventSptr && event)
        {
            ke::EventManager::instance()->queue(std::forward<ke::EventSptr>(event));
        }

        /// <summary>
        /// Dispatch the given event to all delegates listening for the given event's type now.
        /// </summary>
        /// <param name="p_Event"></param>
        /// <returns>false if Event is not handled by a listener.</returns>
        static inline void dispatchNow(ke::EventSptr && event)
        {
            ke::EventManager::instance()->dispatchNow(std::forward<ke::EventSptr>(event));
        }
    };

}
