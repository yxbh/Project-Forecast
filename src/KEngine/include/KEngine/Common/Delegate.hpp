#pragma once

#include "SRDelegate.hpp"

namespace ke
{

    template<typename FuncT>
    using Delegate = typename generic::delegate<FuncT>;

}