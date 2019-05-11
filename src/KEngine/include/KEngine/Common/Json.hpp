#pragma once

#pragma warning(push)
#pragma warning(disable:26444)  // Avoid unnamed objects with custom construction and destruction (es.84).
#pragma warning(disable:26451)  // Arithmetic overflow
#pragma warning(disable:28020)
#include "Libs/json.hpp"
#pragma warning(pop)

namespace ke
{

    // json library from https://github.com/nlohmann/json
    using namespace nlohmann; // alias the nlohmann namespace to make it our own.

    using Json = nlohmann::json;

}
