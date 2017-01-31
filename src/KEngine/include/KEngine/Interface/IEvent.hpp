#pragma once

#include "KEngine/Common/Queues.hpp"
#include "KEngine/Common/Delegate.hpp"
#include "KEngine/Common/String.hpp"

#include <cstdint>
#include <memory>

namespace ke
{

    using EventType = std::uint16_t;


    /// <summary>
    /// Interface for creating command/event classes.
    /// Each derived class needs to define their own unique static EventType.
    /// </summary>
    /// 
    /// Assignment operators and copy constructors are disabled as user should only use smart pointers to an Event class type.
    /// Use the makeCopy() method to make copies of the same event.
    class IEvent
    {
    public:
        using Sptr = std::shared_ptr<IEvent>;
        using Uptr = std::unique_ptr<IEvent>;
        using Wptr = std::weak_ptr<IEvent>;

        static const EventType INVALID_EVENT = 0;
        static const EventType TYPE = INVALID_EVENT;

        IEvent();
        IEvent(IEvent &) = delete;
        IEvent(IEvent &&) = delete;
        IEvent & operator=(IEvent &) = delete;
        IEvent & operator=(IEvent &&) = delete;

        virtual ~IEvent();

        virtual ke::EventType getType() = 0;
        virtual ke::String getName() = 0;

        virtual Sptr makeCopy() = 0;
    };

    inline IEvent::IEvent() {}
    inline IEvent::~IEvent() {}
    inline EventType IEvent::getType()
    {
        return INVALID_EVENT;
    }


    using EventSptr = IEvent::Sptr;
    using EventUptr = IEvent::Uptr;
    using EventWptr = IEvent::Wptr;

    using EventQueue = ke::ThreadSafeQueue<EventSptr>;


    /*
    \class EventDelegate
    \brief The event delegate type.
    */
    using EventDelegate = ke::Delegate<void(::ke::EventSptr)>;

}
