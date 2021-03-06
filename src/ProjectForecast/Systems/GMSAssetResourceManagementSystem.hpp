#pragma once

#include <KEngine/Interfaces/ISystem.hpp>

namespace pf
{

    /// <summary>
    /// A resource manager that manages GM:S specific resources such as those output by Altar.
    /// </summary>
    class GMSAssetResourceManagementSystem : public ke::ISystem
    {
        KE_DEFINE_SYSTEM_COMMON_PROPERTIES(GMSAssetResourceManagementSystem, 0xc6560e7e)

    public:

        virtual bool initialise(void) final;
        virtual void shutdown(void) final;

        virtual void update(ke::Time elapsedTime) final;

    private:
        void loadTexpageAssets(void);
        void loadTextureAssets(void);
        void loadBgAssets(void);
        void loadCodeAssets(void);
        void loadSpriteAssets(void);
        void loadRoomAssets(void);
        void loadObjectAssets(void);
    };

}