#include "KEngine\Core\EventManager.hpp"
#include "KEngine\Interface\IEvent.hpp"

#include "KEngine\UnitTest\catch.hpp"


namespace
{
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
}

TEST_CASE("Events Unit Tests") // TODO: what am i testing here?
{
    ke::EventManager::instance();
    REQUIRE(ke::EventManager::instance() != nullptr);

    TestEvent event;
}