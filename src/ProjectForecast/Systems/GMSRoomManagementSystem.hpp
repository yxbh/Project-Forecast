#pragma once

#include "../AssetResources/GMSRoomResource.hpp"

#include <KEngine/Interfaces/IEntity.hpp>
#include <KEngine/Interfaces/ISystem.hpp>
#include <KEngine/Interfaces/IEvent.hpp>

#include <atomic>
#include <vector>

namespace pf
{

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

        pf::GMSRoomResource * currentRoomResource;
        std::vector<ke::Entity*> currentRoomEntities;
        std::vector<ke::Entity*> currentRoomBgEntities;

        std::atomic_bool isLoadingRoom = false;

    };

}