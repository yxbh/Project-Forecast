#pragma once

#include "Libs/json.hpp"

namespace ke
{

    // json library from https://github.com/nlohmann/json
    using namespace nlohmann; // alias the nlohmann namespace to make it our own.

    class Json : public nlohmann::json {};

}