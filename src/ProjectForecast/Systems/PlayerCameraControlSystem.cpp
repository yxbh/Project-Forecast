#include "PlayerCameraControlSystem.hpp"

#include "KEngine/App.hpp"
#include "KEngine/Core/EventManager.hpp"
#include "KEngine/Events/AppEvents.hpp"
#include "KEngine/Events/OtherGraphicsEvents.hpp"
#include "KEngine/Entity/Components/EntityCameraComponent.hpp"
#include "KEngine/Log/Log.hpp"

#include <algorithm>
#include <cmath>


namespace pf
{
    static constexpr float g_zoomFactors[] = { 0.3f, 0.4f, 0.5f, 0.6f, 0.7f, 0.8f, 0.9f, 1.0f, 2.0f, 3.0f, 4.0f };
    static constexpr std::size_t g_zoomFactorsCount = sizeof(g_zoomFactors) / sizeof(g_zoomFactors[0]);


    bool PlayerCameraControlSystem::initialise()
    {
        ke::EventManager::registerListener<ke::WindowResizedEvent>(this, &PlayerCameraControlSystem::handleWindowSizeChangedRequest);
        ke::EventManager::registerListener<ke::CameraViewZoomDeltaRequestEvent>(this, &PlayerCameraControlSystem::handleCameraViewZoomRequest);

        auto entity = ke::App::instance()->getLogic()->getEntityManager()->newEntity().lock();
        auto cameraComponent = ke::makeEntityComponent<ke::EntityCameraComponent>(entity);
        cameraComponent->setCameraNode(ke::CameraNode::create(entity->getId()));
        entity->addComponent(cameraComponent);
        ke::App::instance()->getLogic()->getCurrentHumanView()->getScene()->setCameraNode(cameraComponent->getCameraNode());
        ke::App::instance()->getLogic()->getCurrentHumanView()->attachEntity(entity->getId());

        this->currentZoomFactorIdx = 7;

        return true;
    }

    void PlayerCameraControlSystem::shutdown()
    {
        ke::EventManager::deregisterListener<ke::WindowResizedEvent>(this, &PlayerCameraControlSystem::handleWindowSizeChangedRequest);
        ke::EventManager::deregisterListener<ke::CameraViewZoomDeltaRequestEvent>(this, &PlayerCameraControlSystem::handleCameraViewZoomRequest);
    }

    void PlayerCameraControlSystem::update(ke::Time elapsedTime)
    {
        KE_UNUSED(elapsedTime);
    }

    void PlayerCameraControlSystem::handleWindowSizeChangedRequest(ke::EventSptr event)
    {
        auto resizedEvent = dynamic_cast<ke::WindowResizedEvent*>(event.get());
        assert(resizedEvent);
        this->windowSizeCache = resizedEvent->getNewSize();
        auto currentHumanView = ke::App::instance()->getLogic()->getCurrentHumanView();
        assert(currentHumanView);
        if (currentHumanView)
        {
            auto currentCameraNode = dynamic_cast<ke::CameraNode*>(currentHumanView->getScene()->getCameraNode());
            if (currentCameraNode)
            {
                const auto zoomFactor = g_zoomFactors[static_cast<unsigned>(this->currentZoomFactorIdx)];
                const auto newZoomVal = 1.0f / std::sqrtf(zoomFactor);
                currentCameraNode->setViewDimension(
                    {
                        static_cast<std::uint32_t>(this->windowSizeCache.width * newZoomVal),
                        static_cast<std::uint32_t>(this->windowSizeCache.height * newZoomVal)
                    }
                );
            }
        }
    }

    void PlayerCameraControlSystem::handleCameraViewZoomRequest(ke::EventSptr event)
    {
        auto zoomEvent = dynamic_cast<ke::CameraViewZoomDeltaRequestEvent*>(event.get());
        assert(zoomEvent);

        const auto delta = zoomEvent->getDeltaZoom();
        if (delta < 0.0f) // zoom out
        {
            ke::Log::instance()->debug("Player camera zooming out!");
            --this->currentZoomFactorIdx;
        }
        else if (delta > 0.0f) // zoom in
        {
            ke::Log::instance()->debug("Player camera zooming in!");
            ++this->currentZoomFactorIdx;
        }
        else
        {
            ke::Log::instance()->warn("Unexpected 0 zoom delta.");
        }
        this->currentZoomFactorIdx = std::clamp(this->currentZoomFactorIdx, 0, static_cast<int>(g_zoomFactorsCount - 1));

        auto currentHumanView = ke::App::instance()->getLogic()->getCurrentHumanView();
        assert(currentHumanView);
        if (currentHumanView)
        {
            auto currentCameraNode = dynamic_cast<ke::CameraNode*>(currentHumanView->getScene()->getCameraNode());
            if (currentCameraNode)
            {
                const auto zoomFactor = g_zoomFactors[static_cast<unsigned>(this->currentZoomFactorIdx)];
                const auto newZoomVal = 1.0f / std::sqrtf(zoomFactor);
                currentCameraNode->setViewDimension(
                    {
                        static_cast<std::uint32_t>(this->windowSizeCache.width * newZoomVal),
                        static_cast<std::uint32_t>(this->windowSizeCache.height * newZoomVal)
                    }
                );
                ke::Log::instance()->info("New camera zoom: {}, Zoom factor: {}", newZoomVal, zoomFactor);
            }
        }
    }

}