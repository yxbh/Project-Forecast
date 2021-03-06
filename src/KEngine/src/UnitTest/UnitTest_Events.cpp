#if defined(RUN_UNIT_TESTS)

#include "KEngine/Core/EventManager.hpp"
#include "KEngine/Interfaces/IEvent.hpp"

#include "KEngine/UnitTest/catch.hpp"


namespace
{
    class TestEvent : public ke::IEvent
    {
        KE_DEFINE_EVENT_COMMON_PROPERTIES(TestEvent, 1)

    public:
        virtual ke::EventSptr makeCopy() const final
        {
            return ke::makeEvent<TestEvent>();
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