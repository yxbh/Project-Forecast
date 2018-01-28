#include "PlayerCameraControlSystem.hpp"

#include "KEngine/App.hpp"
#include "KEngine/Core/EventManager.hpp"
#include "KEngine/Events/AppEvents.hpp"
#include "KEngine/Entity/Components/EntityCameraComponent.hpp"

namespace pf
{

    bool PlayerCameraControlSystem::initialise()
    {
        ke::EventManager::registerListener<ke::WindowResizedEvent>(this, &PlayerCameraControlSystem::handleWindowSizeChangedRequest);

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
                currentCameraNode->setDimension(resizedEvent->getNewSize());
            }
        }
    }

}