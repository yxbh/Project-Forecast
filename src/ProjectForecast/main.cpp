#if !defined(RUN_UNIT_TESTS)

#include "KEngine\App.hpp"

#include <chrono>
#include <iostream>
#include <thread>

int main(int argc, char ** argv)
{
    using namespace std::chrono_literals;
    std::cout << u8"hello world" << std::endl;

    ke::App app;
    const int result = app.exec();
    std::this_thread::sleep_for(2s);
    return result;
}

#else

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "KEngine/UnitTest/catch.hpp"

#endif
