#if defined(RUN_UNIT_TESTS)

#include "KEngine/Common/macros.hpp"
#include "KEngine/Core/EventManager.hpp"
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

    class TestEvent : public ke::IEvent
    {
        KE_DEFINE_EVENT_COMMON_PROPERTIES(TestEvent, 1)

    public:
        using IEvent::IEvent;

        virtual ke::EventSptr makeCopy() const final
        {
            return ke::makeEvent<TestEvent>();
        }
    };

    class TestEvent2 : public ke::IEvent
    {
        KE_DEFINE_EVENT_COMMON_PROPERTIES(TestEvent2, 2)

    public:
        virtual ke::EventSptr makeCopy() const final
        {
            return ke::makeEvent<TestEvent2>();
        }
    };


    class System // TODO: inherit from ISystem interface.
    {
    public:
        System()
        {
            ke::EventManager::instance()->registerListener(TestEvent::TYPE, ke::EventDelegate(this, &System::handleEvent));
            ke::EventManager::registerListener<TestEvent>(this, &System::handleEvent2);
            ke::EventManager::registerListener<TestEvent>(&::staticFunc);
        }

        ~System()
        {
            ke::EventManager::instance()->deregisterListener(TestEvent::TYPE, ke::EventDelegate(this, &System::handleEvent));
            ke::EventManager::deregisterListener<TestEvent>(this, &System::handleEvent2);
            ke::EventManager::deregisterListener<TestEvent>(&::staticFunc);
        }

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
}

TEST_CASE("Event Manager Unit Tests")
{
    SECTION("")
    {
        CHECK(counter == 0);

        System system;
        ke::EventManager::instance()->enqueue(ke::EventSptr(new TestEvent));
        ke::EventManager::instance()->update();
        CHECK(counter == 3);

        ke::EventManager::enqueue(ke::EventSptr(new TestEvent));
        ke::EventManager::update();
        CHECK(counter == 6);

        ke::EventManager::instance()->dispatchNow(ke::EventSptr(new TestEvent));

        CHECK(counter == 9);
        
        ke::EventManager::dispatchNow(ke::EventSptr(new TestEvent));
        CHECK(counter == 12);

        CHECK(ke::EventManager::registerListener<TestEvent2>(&::staticFunc));
        CHECK(!ke::EventManager::registerListener<TestEvent2>(&::staticFunc));
        CHECK(ke::EventManager::deregisterListener<TestEvent2>(&::staticFunc));
        CHECK(!ke::EventManager::deregisterListener<TestEvent2>(&::staticFunc));

        CHECK(ke::EventManager::registerListener<TestEvent2>(&system, &System::handleEvent2));
        CHECK(!ke::EventManager::registerListener<TestEvent2>(&system, &System::handleEvent2));
        CHECK(ke::EventManager::deregisterListener<TestEvent2>(&system, &System::handleEvent2));
        CHECK(!ke::EventManager::deregisterListener<TestEvent2>(&system, &System::handleEvent2));
    }
}

#endif
