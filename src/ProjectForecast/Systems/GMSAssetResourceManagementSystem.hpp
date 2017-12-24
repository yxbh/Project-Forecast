#pragma once

#include <KEngine/Interfaces/ISystem.hpp>

namespace pf
{

    /// <summary>
    /// 
    /// </summary>
    class GMSAssetResourceManagementSystem : public ke::ISystem
    {
        KE_DEFINE_SYSTEM_COMMON_PROPERTIES(GMSAssetResourceManagementSystem, 0xc6560e7e)

    public:

        virtual bool initialise() final;
        virtual void shutdown() final;

        virtual void update(ke::Time elapsedTime) final;

    };

}