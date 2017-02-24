#include "DebugDrawSystem.hpp"

#include "KEngine/App.hpp"
#include "../Events/RequestDrawDebugDotEvent.hpp"
#include "KEngine/Entity/Components/EntityRenderableComponents.hpp"
#include "KEngine/Common/Transform2D.hpp"
#include "KEngine/Core/Entity.hpp"
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
            auto actualEvent = std::dynamic_pointer_cast<pf::RequestDrawDebugDotEvent>(event);
            assert(actualEvent);
            static const auto debugDrawEntityJson =
R"(
    {
        "type" : "entity",
        "name" : "debug dot",
        "components" :
        [
            {
                "type" : "entity_component",
                "component_name" : "EntityRenderComponent",
                "scene_node" :
                {
                    "type" : "scene_node",
                    ""
                    "children" : []
                }
            }
        ]
    }
)";

            for (auto i = 0; i < 50; ++i)
            {
                auto entity = ke::makeEntity(ke::Entity::newId());
                auto & position = actualEvent->getPosition();
                ke::Transform2D transform{ position.x, position.y };
                auto renderComponent = ke::makeEntityComponent<ke::EntityRenderableCircleShapeComponent>(entity, transform, ke::Color::GREEN);
                entity->addComponent(renderComponent);

                auto result = entity->initialise();
                assert(result);

                ke::App::instance()->getLogic()->getEntityManager()->addEntity(entity);
                ke::App::instance()->getLogic()->getCurrentHumanView()->getScene()->addNode(renderComponent->getSceneNode()); // TODO: potentially remove this if we are going to go with the approach where renderable components broadcast events when a scene node is created.

                //ke::Log::instance()->info("entity with scene node created.");
            }
            

            break;
        }

        default:
            ke::Log::instance()->warn("Unexpected event {} received by {}.", event->getName(), this->getName());
            break;
        }
    }

}