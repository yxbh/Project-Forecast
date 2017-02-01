#pragma once

#include <string>

//
// Reference: http://utf8everywhere.org/
//

namespace ke
{

    using String = std::string;

}

//#define KE_TEXT(Arg) L##Arg
#define KE_TEXT(Arg) Arg