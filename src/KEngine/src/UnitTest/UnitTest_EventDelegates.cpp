#if defined(RUN_UNIT_TESTS)

#include "KEngine/Common/macros.hpp"
#include "KEngine/Common/Delegate.hpp"
#include "KEngine/Interface/IEvent.hpp"

#include "KEngine/UnitTest/catch.hpp"

namespace
{
    static unsigned counter = 0;

    static void resetCounter()
    {
        counter = 0;
    }

    static void staticFunc(ke::EventSptr event)
    {
        KE_UNUSED(event);
        ++counter;
        return;
    }

    static void staticFunc2(ke::EventSptr event)
    {
        KE_UNUSED(event);
        ++counter;
        return;
    }

    class System // TODO: inherit from ISystem interface.
    {
    public:
        void handleEvent(ke::EventSptr event)
        {
            KE_UNUSED(event);
            ++counter;
            return;
        }

        void handleEvent2(ke::EventSptr event)
        {
            KE_UNUSED(event);
            ++counter;
            return;
        }
    };

    class TestEvent : public ke::IEvent
    {
    public:
        static const ke::EventType EVENT_TYPE = 1;

        virtual ke::EventType getType() const final
        {
            return EVENT_TYPE;
        }

        virtual ke::String getName() const final
        {
            return KE_TEXT("TestEvent");
        }

        virtual ke::EventSptr makeCopy() const final
        {
            return ke::makeEvent<TestEvent>();
        }
    };
}

TEST_CASE("Event Delegates Unit Tests")
{
    resetCounter();

    ke::EventSptr event = ke::makeEvent<TestEvent>();

    SECTION("static functions")
    {
        ke::Delegate<void(ke::EventSptr)> delegate1(&staticFunc);
        ke::Delegate<void(ke::EventSptr)> delegate2(&staticFunc);
        ke::Delegate<void(ke::EventSptr)> delegate3(&staticFunc2);

        CHECK(delegate1 == delegate2);
        CHECK(delegate2 == delegate1);

        CHECK(delegate1 != delegate3);
        CHECK(delegate3 != delegate1);

        delegate1(event);
        delegate2(event);
        delegate3(event);

        CHECK(counter == 3);
    }

    resetCounter();

    SECTION("member functions")
    {

        System system, system2;

        ke::Delegate<void(ke::EventSptr)> delegate1(system, &System::handleEvent);
        ke::Delegate<void(ke::EventSptr)> delegate2(system, &System::handleEvent);
        ke::Delegate<void(ke::EventSptr)> delegate3(system, &System::handleEvent2);
        ke::Delegate<void(ke::EventSptr)> delegate4(system2, &System::handleEvent);

        CHECK(delegate1 == delegate2);
        CHECK(delegate2 == delegate1);

        CHECK(delegate1 != delegate3);
        CHECK(delegate3 != delegate1);

        CHECK(delegate1 != delegate4);
        CHECK(delegate4 != delegate1);

        REQUIRE(counter == 0);

        delegate1(event);
        delegate2(event);
        delegate3(event);
        delegate4(event);

        CHECK(counter == 4);
    }

    SECTION("mixed functions")
    {
        ke::Delegate<void(ke::EventSptr)> delegate1(staticFunc);
        ke::Delegate<void(ke::EventSptr)> delegate2(staticFunc2);

        System system, system2;
        ke::Delegate<void(ke::EventSptr)> delegate3(system, &System::handleEvent);
        ke::Delegate<void(ke::EventSptr)> delegate4(system2, &System::handleEvent);

        CHECK(delegate1 != delegate3);
        CHECK(delegate2 != delegate4);
    }
}

#endif
