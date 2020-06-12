#include "EntityTransformControlSystem.hpp"

#include "KEngine/App.hpp"
#include "KEngine/Core/EntityManager.hpp"
#include "KEngine/Core/EventManager.hpp"
#include "KEngine/Entity/Components/EntityCameraComponent.hpp"
#include "KEngine/Events/EntityTransformControlCommandEvents.hpp"
#include "KEngine/Log/Log.hpp"

namespace pf
{
    static auto logger = ke::Log::createDefaultLogger("pf::EntityTransformControlSystem");

    bool EntityTransformControlSystem::initialise()
    {
        ke::EventManager::registerListener<ke::EntityTransformControlCommandEvent>(
            this, &EntityTransformControlSystem::handleEntityTransformControlCommand);
        ke::EventManager::registerListener<ke::EntityTransformDeltaControlCommandEvent>(
            this, &EntityTransformControlSystem::handleEntityTransformDeltaControlCommand);

        return true;
    }
    
    void EntityTransformControlSystem::shutdown()
    {
        ke::EventManager::deregisterListener<ke::EntityTransformControlCommandEvent>(
            this, &EntityTransformControlSystem::handleEntityTransformControlCommand);
        ke::EventManager::deregisterListener<ke::EntityTransformDeltaControlCommandEvent>(
            this, &EntityTransformControlSystem::handleEntityTransformDeltaControlCommand);
    }

    void EntityTransformControlSystem::update(ke::Time elapsedTime)
    {
        KE_UNUSED(elapsedTime);
    }

    void EntityTransformControlSystem::handleEntityTransformControlCommand(ke::EventSptr event)
    {
        auto command = std::dynamic_pointer_cast<ke::EntityTransformControlCommandEvent>(event);
        const auto entityId = command->getEntityId();
        assert(command);
        assert(entityId != ke::INVALID_ENTITY_ID);

        auto entityWptr = ke::App::instance()->getLogic()->getEntityManager()->findEntityById(entityId);
        auto entity = entityWptr.lock();
        if (!entity)
        {
            logger->error("Could not find entity({}) to execute transform control command on.", entityId);
            return;
        }

        auto componentWptr = entity->getComponent<ke::EntityCameraComponent>(ke::EntityCameraComponent::TYPE);
        auto component = componentWptr.lock();
        assert(component);
        component->getCameraNode()->setLocalTransform(command->getTransform());
    }

    void EntityTransformControlSystem::handleEntityTransformDeltaControlCommand(ke::EventSptr event)
    {
        auto command = std::dynamic_pointer_cast<ke::EntityTransformDeltaControlCommandEvent>(event);
        const auto entityId = command->getEntityId();
        assert(command);
        assert(entityId != ke::INVALID_ENTITY_ID);

        auto entityWptr = ke::App::instance()->getLogic()->getEntityManager()->findEntityById(entityId);
        auto entity = entityWptr.lock();
        if (!entity)
        {
            logger->error("Could not find entity({}) to execute transform delta control command on.", entityId);
            return;
        }

        auto componentWptr = entity->getComponent<ke::EntityCameraComponent>();
        auto component = componentWptr.lock();
        assert(component);
        component->getCameraNode()->setLocalTransform(
            component->getCameraNode()->getLocalTransform() + command->getTransform());
    }

}
