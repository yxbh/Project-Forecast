#if !defined(RUN_UNIT_TESTS)

#include "KEngine/Common/macros.hpp"
#include "KEngine/App.hpp"
#include "KEngine/BaseAppLogic.hpp"
#include "KEngine/Log/Log.hpp"

#include "ProjectForecastApp.hpp"

#include <chrono>
#include <iostream>
#include <thread>

int main(int argc, char ** argv)
{
    KE_UNUSED(argc);
    KE_UNUSED(argv);

    std::cout << u8"hello world" << std::endl;

    pf::ProjectForecastApp app;
    const int result = app.exec();
    ke::Log::instance()->info("Exit code: {}", result);
    return result;
}

#else

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "KEngine/UnitTest/catch.hpp"

#endif
