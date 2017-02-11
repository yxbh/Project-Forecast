#pragma once

#include "KEngine/Common/Libs/SRDelegate.hpp"

namespace ke
{

    template<typename FuncT>
    using Delegate = typename generic::delegate<FuncT>;

}