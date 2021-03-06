#if !defined(RUN_UNIT_TESTS)

#include "KEngine/Common/macros.hpp"
#include "KEngine/App.hpp"
#include "KEngine/BaseAppLogic.hpp"
#include "KEngine/Log/Log.hpp"

#include "ProjectForecastApp.hpp"

#include <iostream>

int main(int argc, char ** argv)
{
    std::cout << u8"hello world" << std::endl;

    ke::Log::instance()->set_level(ke::log::level::debug);
    pf::ProjectForecastApp app(argc, argv);
    const int result = app.exec();
    ke::Log::instance()->info("Exit code: {}", result);
    return result;
}

#else

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "KEngine/UnitTest/catch.hpp"

#endif
