#pragma once

#include "KEngine/Interfaces/ISystem.hpp"


namespace pf
{

    /// <summary>
    /// This is a simple system for registering RoR specific entity and entity component builders.
    /// </summary>
    class EntityFactorySystem : public ke::ISystem
    {
        KE_DEFINE_SYSTEM_COMMON_PROPERTIES(EntityFactorySystem, 0x40b34b07)

    public:

        virtual bool initialise(void) final;
        virtual void shutdown(void) final;

        virtual void update(ke::Time p_elapsedTime) final;
    };

}