#include "DebugDrawSystem.hpp"

#include "../Events/RequestDrawDebugDotEvent.hpp"

#include "KEngine/Core/EventManager.hpp"
#include "KEngine/Log/Log.hpp"

namespace pf
{

    bool DebugDrawSystem::initialise()
    {
        ke::EventManager::registerListener<pf::RequestDrawDebugDotEvent>(this, &DebugDrawSystem::handleDebugDrawRequest);
        return true;
    }

    void DebugDrawSystem::shutdown()
    {
        ke::EventManager::deregisterListener<pf::RequestDrawDebugDotEvent>(this, &DebugDrawSystem::handleDebugDrawRequest);
    }

    void DebugDrawSystem::update(ke::Time elapsedTime)
    {

    }

    void DebugDrawSystem::handleDebugDrawRequest(ke::EventSptr event)
    {
        switch (event->getType())
        {
        case pf::RequestDrawDebugDotEvent::TYPE:
        {
            // TODO generate and broadcast entity with render component that contains a SceneNode with a dot.
            break;
        }

        default:
            ke::Log::instance()->warn("Unexpected event {} received by {}.", event->getName(), this->getName());
            break;
        }
    }

}