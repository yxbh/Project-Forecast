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

    SECTION("Conversion Tests: nanoseconds")
    {
        CHECK(ke::Time::nanoseconds(1).asNanoseconds() == 1);
        CHECK(ke::Time::nanoseconds(10).asNanoseconds() == 10);
        CHECK(ke::Time::nanoseconds(100).asNanoseconds() == 100);
        CHECK(ke::Time::nanoseconds(1000).asNanoseconds() == 1000);

        CHECK(ke::Time::nanoseconds(1000).asMicroseconds() == 1);
        CHECK(ke::Time::nanoseconds(10000).asMicroseconds() == 10);
        CHECK(ke::Time::nanoseconds(100000).asMicroseconds() == 100);
        CHECK(ke::Time::nanoseconds(1000000).asMicroseconds() == 1000);

        CHECK(ke::Time::nanoseconds(1).asMilliseconds() == 0); // precision is lost.
        CHECK(ke::Time::nanoseconds(1000000).asMilliseconds() == 1);
    }
    
    SECTION("Conversion Tests: microseconds")
    {
        CHECK(ke::Time::microseconds(1).asNanoseconds() == 1000);
        CHECK(ke::Time::microseconds(10).asNanoseconds() == 10000);
        CHECK(ke::Time::microseconds(100).asNanoseconds() == 100000);
        CHECK(ke::Time::microseconds(1000).asNanoseconds() == 1000000);

        CHECK(ke::Time::microseconds(1).asMicroseconds() == 1);
        CHECK(ke::Time::microseconds(10).asMicroseconds() == 10);
        CHECK(ke::Time::microseconds(100).asMicroseconds() == 100);
        CHECK(ke::Time::microseconds(1000).asMicroseconds() == 1000);

        CHECK(ke::Time::microseconds(1).asMilliseconds() == 0);
        CHECK(ke::Time::microseconds(10).asMilliseconds() == 0);
        CHECK(ke::Time::microseconds(100).asMilliseconds() == 0);
        CHECK(ke::Time::microseconds(500).asMilliseconds() == 0); // precision is lost.
        CHECK(ke::Time::microseconds(1000).asMilliseconds() == 1);
    }

    SECTION("Conversion Tests: milliseconds")
    {
        CHECK(ke::Time::milliseconds(1).asNanoseconds() == 1000000);
        CHECK(ke::Time::milliseconds(10).asNanoseconds() == 10000000);
        CHECK(ke::Time::milliseconds(100).asNanoseconds() == 100000000);
        CHECK(ke::Time::milliseconds(1000).asNanoseconds() == 1000000000);

        CHECK(ke::Time::milliseconds(1).asMicroseconds() == 1000);
        CHECK(ke::Time::milliseconds(10).asMicroseconds() == 10000);
        CHECK(ke::Time::milliseconds(100).asMicroseconds() == 100000);
        CHECK(ke::Time::milliseconds(1000).asMicroseconds() == 1000000);

        CHECK(ke::Time::milliseconds(1).asMilliseconds() == 1);
        CHECK(ke::Time::milliseconds(10).asMilliseconds() == 10);
        CHECK(ke::Time::milliseconds(100).asMilliseconds() == 100);
        CHECK(ke::Time::milliseconds(500).asMilliseconds() == 500);
        CHECK(ke::Time::milliseconds(1000).asMilliseconds() == 1000);

        CHECK(ke::Time::milliseconds(-1).asMilliseconds() == -1);
        CHECK(ke::Time::milliseconds(-10).asMilliseconds() == -10);
        CHECK(ke::Time::milliseconds(-100).asMilliseconds() == -100);
        CHECK(ke::Time::milliseconds(-500).asMilliseconds() == -500);
        CHECK(ke::Time::milliseconds(-1000).asMilliseconds() == -1000);
    }

    SECTION("Conversion Tests: seconds")
    {
        CHECK(ke::Time::seconds(1.0).asSeconds() == 1.0);
        CHECK(ke::Time::seconds(1.5).asSeconds() == 1.5);

        CHECK(ke::Time::seconds(-1.0).asSeconds() == -1.0);
        CHECK(ke::Time::seconds(-1.5).asSeconds() == -1.5);

        CHECK(ke::Time::seconds(1).asNanoseconds() == 1000000000);
        CHECK(ke::Time::seconds(1.5).asNanoseconds() == 1500000000);

        CHECK(ke::Time::seconds(1).asMicroseconds() == 1000000);
        CHECK(ke::Time::seconds(1.5).asMicroseconds() == 1500000);

        CHECK(ke::Time::seconds(1).asMilliseconds() == 1000);
        CHECK(ke::Time::seconds(1.5).asMilliseconds() == 1500);
    }

    SECTION("Addition")
    {
        CHECK((ke::Time::milliseconds(10) + ke::Time::milliseconds(0)) == ke::Time::milliseconds(10));
        CHECK((ke::Time::milliseconds(0) + ke::Time::milliseconds(10)) == ke::Time::milliseconds(10));
        CHECK((ke::Time::milliseconds(10) + ke::Time::milliseconds(10)) == ke::Time::milliseconds(20));
    }

    SECTION("Subtraction")
    {
        CHECK((ke::Time::milliseconds(10) - ke::Time::milliseconds(10)) == ke::Time::Zero);
        CHECK((ke::Time::milliseconds(10) - ke::Time::milliseconds(10)) == ke::Time::milliseconds(0));

        CHECK((ke::Time::milliseconds(0) - ke::Time::milliseconds(10)) == ke::Time::milliseconds(-10));
        CHECK((ke::Time::milliseconds(10) - ke::Time::milliseconds(0)) == ke::Time::milliseconds(10));

        CHECK((ke::Time::milliseconds(1) - ke::Time::microseconds(100)).asMicroseconds() == 900);
    }
}

#endif