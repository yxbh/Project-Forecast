#pragma once

#include <string>

namespace ke
{

    using String = std::wstring;

}

#define KE_TEXT(Arg) L##Arg; 