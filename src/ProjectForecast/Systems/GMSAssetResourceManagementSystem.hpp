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

        virtual bool initialise(void) final;
        virtual void shutdown(void) final;

        virtual void update(ke::Time elapsedTime) final;

    private:
        void loadTextureAssets(void);
        void loadRoomAssets(void);
        void loadObjectAssets(void);
    };

}