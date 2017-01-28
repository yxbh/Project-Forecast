#include "KEngine\Core\EventManager.hpp"
#include "KEngine\Interface\IEvent.hpp"

#include "KEngine\UnitTest\catch.hpp"

namespace
{
    static unsigned counter = 0;

    static void resetCounter()
    {
        counter = 0;
    }

    static void staticFunc(ke::EventSptr event)
    {
        ++counter;
        return;
    }

    static void staticFunc2(ke::EventSptr event)
    {
        ++counter;
        return;
    }

    class System // TODO: inherit from ISystem interface.
    {
    public:
        void handleEvent(ke::EventSptr event)
        {
            ++counter;
            return;
        }

        void handleEvent2(ke::EventSptr event)
        {
            ++counter;
            return;
        }
    };

    class TestEvent : public ke::IEvent
    {
    public:
        static const ke::EventType EVENT_TYPE = 1;

        virtual ke::EventType  getType() final
        {
            return EVENT_TYPE;
        }

        virtual ke::String getName() final
        {
            return KE_TEXT("TestEvent");
        }

        virtual ke::EventSptr makeCopy() final
        {
            return std::make_shared<TestEvent>();
        }
    };

    class TestEvent2 : public ke::IEvent
    {
    public:
        static const ke::EventType EVENT_TYPE = 2;

        virtual ke::EventType  getType() final
        {
            return EVENT_TYPE;
        }

        virtual ke::String getName() final
        {
            return KE_TEXT("TestEvent2");
        }

        virtual ke::EventSptr makeCopy() final
        {
            return std::make_shared<TestEvent2>();
        }
    };
}

TEST_CASE("Event Manager Unit Tests")
{
    SECTION("")
    {

    }
}