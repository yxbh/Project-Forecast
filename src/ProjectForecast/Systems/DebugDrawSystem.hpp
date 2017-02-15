#pragma once

#include "KEngine/Interface/ISystem.hpp"

namespace pf
{

    /// <summary>
    /// 
    /// </summary>
    class DebugDrawSystem : public ke::ISystem
    {
        KE_DEFINE_SYSTEM_COMMON_PROPERTIES(DebugDrawSystem, 0xB32C5615)

    public:

        virtual bool initialise() final;
        virtual void shutdown() final;

        virtual void update(ke::Time elapsedTime) final;
        
    };


}