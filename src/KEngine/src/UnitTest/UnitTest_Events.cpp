#if defined(RUN_UNIT_TESTS)

#include "KEngine/Core/EventManager.hpp"
#include "KEngine/Interface/IEvent.hpp"

#include "KEngine/UnitTest/catch.hpp"


namespace
{
    class TestEvent : public ke::IEvent
    {
    public:
        static const ke::EventType TYPE = 1;
        
        virtual ke::EventType getType() const final
        {
            return TYPE;
        }

        virtual ke::String getName() const final
        {
            return KE_TEXT("TestEvent");
        }

        virtual ke::EventSptr makeCopy() const final
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

#endif