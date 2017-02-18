#pragma once

#include "KEngine/Common/Queues.hpp"
#include "KEngine/Common/Delegate.hpp"

#include <cstdint>
#include <memory>

namespace ke
{

    using EventType = std::uint32_t;


    /// <summary>
    /// Interface for creating command/event classes.
    /// Each derived class needs to define their own unique public static EventType variable named TYPE.
    /// </summary>
    /// 
    /// Assignment operators and copy constructors are disabled as user should only use smart pointers to an Event class type.
    /// Use the makeCopy() method to make copies of the same event.
    ///
    class IEvent
    {
    public:
        using Sptr = std::shared_ptr<IEvent>;
        using Uptr = std::unique_ptr<IEvent>;
        using Wptr = std::weak_ptr<IEvent>;

        static const EventType INVALID_EVENT = 0;
        static const EventType TYPE = INVALID_EVENT; /// each class inheriting from IEvent is required to redefine this.

        IEvent();
        IEvent(IEvent &) = delete;
        IEvent(IEvent &&) = delete;
        IEvent & operator=(IEvent &) = delete;
        IEvent & operator=(IEvent &&) = delete;

        virtual ~IEvent();

        virtual ke::EventType getType() const = 0;
        virtual const char * getName() const = 0;

        virtual Sptr makeCopy() const = 0;
    };

    inline IEvent::IEvent() {}
    inline IEvent::~IEvent() {}
    inline EventType IEvent::getType() const
    {
        return INVALID_EVENT;
    }


    using EventSptr = IEvent::Sptr;
    using EventUptr = IEvent::Uptr;
    using EventWptr = IEvent::Wptr;

    using EventQueue = ke::ThreadSafeQueue<EventSptr>;


    /// <summary>
    /// The event delegate type.
    /// </summary>
    using EventDelegate = ke::Delegate<void(::ke::EventSptr)>;


    /// <summary>
    /// Helper template function for constructing an Event object that's a subclass of ke::IEvent.
    /// </summary>
    /// <param name="...args"></param>
    /// <returns></returns>
    template <typename Event_T, typename ... Args_T>
    auto makeEvent(Args_T && ... args ) -> EventSptr
    {
        return std::make_shared<Event_T>(std::forward<Args_T>(args)...);
    }

}


/// <summary>
/// Define the common IEvent properties.
/// </summary>
#define KE_DEFINE_EVENT_COMMON_PROPERTIES(CLASS_NAME, EVENT_TYPE_VALUE) \
public: \
    static constexpr ke::EventType TYPE = EVENT_TYPE_VALUE; \
    static constexpr const char * const NAME = #CLASS_NAME; \
    virtual ke::EventType getType() const override { return CLASS_NAME::TYPE; } \
    virtual const char * getName() const override { return CLASS_NAME::NAME; } \
private:
