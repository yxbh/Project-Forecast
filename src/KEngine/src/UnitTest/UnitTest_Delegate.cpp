#if defined(RUN_UNIT_TESTS)

#include "KEngine/Common/Delegate.hpp"

#include "KEngine/UnitTest/catch.hpp"

namespace
{
    static unsigned counter = 0;

    static void resetCounter()
    {
        counter = 0;
    }

    static void staticFunc()
    {
        ++counter;
        return;
    }

    static void staticFunc2()
    {
        ++counter;
        return;
    }

    class Object
    {
    public:
        void memFunc()
        {
            ++counter;
            return;
        }

        void memFunc2()
        {
            ++counter;
            return;
        }
    };
}

TEST_CASE("ke::Delegate Unit Tests")
{
    resetCounter();

    SECTION("static functions")
    {
        ke::Delegate<void(void)> delegate1(staticFunc);
        ke::Delegate<void(void)> delegate2(staticFunc);
        ke::Delegate<void(void)> delegate3(staticFunc2);

        CHECK(delegate1 == delegate2);
        CHECK(delegate2 == delegate1);

        CHECK(delegate1 != delegate3);
        CHECK(delegate3 != delegate1);

        delegate1();
        delegate2();
        delegate3();

        CHECK(counter == 3);
    }

    resetCounter();
    
    SECTION("member functions")
    {

        Object object, object2;

        ke::Delegate<void(void)> delegate1(object, &Object::memFunc);
        ke::Delegate<void(void)> delegate2(object, &Object::memFunc);
        ke::Delegate<void(void)> delegate3(object, &Object::memFunc2);
        ke::Delegate<void(void)> delegate4(object2, &Object::memFunc);

        CHECK(delegate1 == delegate2);
        CHECK(delegate2 == delegate1);

        CHECK(delegate1 != delegate3);
        CHECK(delegate3 != delegate1);

        CHECK(delegate1 != delegate4);
        CHECK(delegate4 != delegate1);

        REQUIRE(counter == 0);

        delegate1();
        delegate2();
        delegate3();
        delegate4();

        CHECK(counter == 4);
    }

    SECTION("mixed functions")
    {
        ke::Delegate<void(void)> delegate1(staticFunc);
        ke::Delegate<void(void)> delegate2(staticFunc2);

        Object object, object2;
        ke::Delegate<void(void)> delegate3(object, &Object::memFunc);
        ke::Delegate<void(void)> delegate4(object2, &Object::memFunc);

        CHECK(delegate1 != delegate3);
        CHECK(delegate2 != delegate4);
    }
}

#endif
