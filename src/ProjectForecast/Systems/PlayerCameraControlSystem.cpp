#include "PlayerCameraControlSystem.hpp"

#include "KEngine/App.hpp"
#include "KEngine/Core/EventManager.hpp"
#include "KEngine/Events/AppEvents.hpp"
#include "KEngine/Events/OtherGraphicsEvents.hpp"
#include "KEngine/Entity/Components/EntityCameraComponent.hpp"
#include "KEngine/Log/Log.hpp"

namespace pf
{

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
        auto currentHumanView = ke::App::instance()->getLogic()->getCurrentHumanView();
        assert(currentHumanView);
        if (currentHumanView)
        {
            const auto & newSize = resizedEvent->getNewSize();
            auto currentCameraNode = dynamic_cast<ke::CameraNode*>(currentHumanView->getScene()->getCameraNode());
            if (currentCameraNode)
            {
                currentCameraNode->setDimension(newSize);
            }
        }
    }

    void PlayerCameraControlSystem::handleCameraViewZoomRequest(ke::EventSptr event)
    {
        auto zoomEvent = dynamic_cast<ke::CameraViewZoomDeltaRequestEvent*>(event.get());
        assert(zoomEvent);
        auto currentHumanView = ke::App::instance()->getLogic()->getCurrentHumanView();
        assert(currentHumanView);
        if (currentHumanView)
        {
            auto currentCameraNode = dynamic_cast<ke::CameraNode*>(currentHumanView->getScene()->getCameraNode());
            if (currentCameraNode)
            {
                const auto delta = zoomEvent->getDeltaZoom();
                auto newZoomVal = currentCameraNode->getViewZoom();
                if (delta < 0.0f) // zoom out
                {
                    if (newZoomVal <= 1.0f)
                    {
                        newZoomVal *= 2;
                    }
                    else
                    {
                        newZoomVal += 1;
                    }
                }
                else if (delta > 0.0f) // zoom in
                {
                    if (newZoomVal <= 1.0f)
                    {
                        newZoomVal /= 2;
                    }
                    else
                    {
                        newZoomVal -= 1;
                    }
                }
                else
                {
                    ke::Log::instance()->warn("Unexpected 0 zoom delta.");
                }
                currentCameraNode->setViewZoom(newZoomVal);
                ke::Log::instance()->info("New camera zoom: {}", newZoomVal);
            }
        }
    }

}