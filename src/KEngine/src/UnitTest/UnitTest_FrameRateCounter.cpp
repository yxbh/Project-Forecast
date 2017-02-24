#if defined(RUN_UNIT_TESTS)

#include "KEngine/Core/FrameRateCounter.hpp"

#include "KEngine/UnitTest/catch.hpp"


TEST_CASE("ke::FrameRateCounter Unit Tests")
{

    SECTION("zero.")
    {
        ke::FrameRateCounter fpsCounter(1000);
        fpsCounter.getAverageFps();
    }

}

#endif
