#if defined(RUN_UNIT_TESTS)

#include "KEngine/Core/SystemManager.hpp"

#include "KEngine/UnitTest/catch.hpp"


namespace
{
    class MockSystemA : public ke::ISystem
    {
        inline virtual bool initialise() final { return true; }
        inline virtual void shutdown() {}

        inline virtual void update(ke::Time elapsedTime) { KE_UNUSED(elapsedTime); };

        inline virtual ke::SystemType getType() const { return 9999; };
        inline virtual const char * getName() const { return "MockSystemA"; }
    };

    class MockSystemB : public ke::ISystem
    {
        inline virtual bool initialise() final { return true; }
        inline virtual void shutdown() {}

        inline virtual void update(ke::Time elapsedTime) { KE_UNUSED(elapsedTime); };

        inline virtual ke::SystemType getType() const { return 9998; };
        inline virtual const char * getName() const { return "MockSystemB"; }
    };
}


TEST_CASE("ke::SystemManager Unit Tests")
{

    SECTION("Add & Remove")
    {
        ke::SystemManager sm;
        CHECK(0 == sm.getSystemCount());

        sm.addSystem(std::make_unique<::MockSystemA>());
        CHECK(1 == sm.getSystemCount());

        sm.addSystem(std::make_unique<::MockSystemB>());
        CHECK(2 == sm.getSystemCount());

        sm.clear();
        CHECK(0 == sm.getSystemCount());

        // try adding again to make sure it still works correctly after a clear() call.
        sm.addSystem(std::make_unique<::MockSystemA>());
        CHECK(1 == sm.getSystemCount());

        sm.addSystem(std::make_unique<::MockSystemB>());
        CHECK(2 == sm.getSystemCount());

        sm.clear();
        CHECK(0 == sm.getSystemCount());
    }

}

#endif