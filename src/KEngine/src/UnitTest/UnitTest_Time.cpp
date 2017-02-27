#if defined(RUN_UNIT_TESTS)

#include "KEngine/Common/Time.hpp"

#include "KEngine/UnitTest/catch.hpp"


TEST_CASE("ke::Time Unit Tests")
{
    
    WHEN("A Time object is default constructed")
    {
        ke::Time time;
        ke::Time time2(ke::Time::Zero);

        THEN("Internal counter should be 0")
        {
            CHECK(time.asNanoseconds() == 0);
            CHECK(time.asMicroseconds() == 0);
            CHECK(time.asMilliseconds() == 0);
            CHECK(time.asSeconds() == 0.0);
            CHECK(time2.asNanoseconds() == 0);
            CHECK(time2.asMicroseconds() == 0);
            CHECK(time2.asMilliseconds() == 0);
            CHECK(time2.asSeconds() == 0.0);
        }        
    }

    SECTION("Conversion Tests")
    {
        CHECK(ke::Time::microseconds(1).asMicroseconds() == 1);
        CHECK(ke::Time::microseconds(10).asMicroseconds() == 10);
        CHECK(ke::Time::microseconds(100).asMicroseconds() == 100);
        CHECK(ke::Time::microseconds(1000).asMicroseconds() == 1000);

        CHECK(ke::Time::microseconds(1).asMilliseconds() == 0);
        CHECK(ke::Time::microseconds(10).asMilliseconds() == 0);
        CHECK(ke::Time::microseconds(100).asMilliseconds() == 0);
        CHECK(ke::Time::microseconds(500).asMilliseconds() == 0); // precision is lost.
        CHECK(ke::Time::microseconds(1000).asMilliseconds() == 1);

        CHECK(ke::Time::seconds(1).asMilliseconds() == 1000);
        CHECK(ke::Time::seconds(1.5).asMilliseconds() == 1500);

        CHECK(ke::Time::seconds(1).asMicroseconds() == 1000000);
        CHECK(ke::Time::seconds(1.5).asMicroseconds() == 1500000);
    }
    
}

#endif