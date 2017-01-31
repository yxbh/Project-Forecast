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
        ++counter;
        return;
    }

    static void staticFunc2(ke::EventSptr event)
    {
        ++counter;
        return;
    }

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


    class System // TODO: inherit from ISystem interface.
    {
    public:
        System()
        {
            ke::EventManager::instance()->registerListener(TestEvent::EVENT_TYPE, ke::EventDelegate(this, &System::handleEvent));
            ke::EventManager::registerListener<TestEvent>(this, &System::handleEvent2);
            ke::EventManager::registerListener<TestEvent>(&::staticFunc);
        }

        ~System()
        {
            ke::EventManager::instance()->deregisterListener(TestEvent::EVENT_TYPE, ke::EventDelegate(this, &System::handleEvent));
            ke::EventManager::deregisterListener<TestEvent>(this, &System::handleEvent2);
            ke::EventManager::deregisterListener<TestEvent>(&::staticFunc);
        }

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
}

TEST_CASE("Event Manager Unit Tests")
{
    SECTION("")
    {
        CHECK(counter == 0);

        System system;
        ke::EventManager::instance()->queue(ke::EventSptr(new TestEvent));
        ke::EventManager::instance()->update();

        CHECK(counter == 3);

        ke::EventManager::queue(ke::EventSptr(new TestEvent));
        ke::EventManager::update();

        CHECK(counter == 6);

        ke::EventManager::instance()->dispatchNow(ke::EventSptr(new TestEvent));

        CHECK(counter == 9);
    }
}
