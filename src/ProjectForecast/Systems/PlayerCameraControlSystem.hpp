#pragma once

#include <KEngine/Interfaces/IEntity.hpp>
#include <KEngine/Interfaces/ISystem.hpp>
#include <KEngine/Interfaces/IEvent.hpp>
#include <KEngine/Common/Dimension2D.hpp>


namespace ke
{
    // forward delcarations

    class CameraNode;
}

namespace pf
{

    /// <summary>
    /// This class is responsible for maintaining the player camera.
    /// </summary>
    class PlayerCameraControlSystem : public ke::ISystem
    {
        KE_DEFINE_SYSTEM_COMMON_PROPERTIES(PlayerCameraControlSystem, 0x1671252a)

    public:

        virtual bool initialise() final;
        virtual void shutdown() final;

        virtual void update(ke::Time elapsedTime) final;

        void handleWindowSizeChangedRequest(ke::EventSptr event);
        void handleCameraViewZoomRequest(ke::EventSptr event);

    private:
        ke::Dimension2DUInt32 windowSizeCache;
        int currentZoomFactorIdx = 0;

    };

}