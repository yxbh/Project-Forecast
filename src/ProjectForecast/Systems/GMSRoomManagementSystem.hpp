#pragma once

#include <KEngine/Interfaces/IEntity.hpp>
#include <KEngine/Interfaces/ISystem.hpp>
#include <KEngine/Interfaces/IEvent.hpp>

#include <atomic>
#include <memory>
#include <vector>

namespace ke
{
    // forward delcarations

    class CameraNode;
}

namespace pf
{
    // Forward declarations.
    class GMSRoomResource;


    /// <summary>
    /// 
    /// </summary>
    class GMSRoomManagementSystem : public ke::ISystem
    {
        KE_DEFINE_SYSTEM_COMMON_PROPERTIES(GMSRoomManagementSystem, 0x2F5C1FE6)

    public:

        virtual bool initialise() final;
        virtual void shutdown() final;

        virtual void update(ke::Time elapsedTime) final;

        void handleGMSRoomLoadRequest(ke::EventSptr event);

    private:
        void unloadCurrentEntities();

        void updateRorLevelBg_DesolateForest(ke::CameraNode * p_cameraNode);
        void updateRorLevelBg_DryLake(ke::CameraNode * p_cameraNode);

        std::shared_ptr<pf::GMSRoomResource> currentRoomResource;
        std::vector<ke::Entity*> currentRoomEntities;
        std::vector<ke::Entity*> currentRoomBgEntities;

        std::atomic_bool isLoadingRoom = false;

        ke::String currentLevelName = "Desolate Forest";

    };

}