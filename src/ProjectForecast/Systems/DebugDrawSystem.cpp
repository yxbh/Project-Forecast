#include "DebugDrawSystem.hpp"

#include "KEngine/App.hpp"
#include "../Events/RequestDrawDebugDotEvent.hpp"
#include "KEngine/Entity/Components/EntityRenderableComponents.hpp"
#include "KEngine/Entity/Components/EntityCameraComponent.hpp"
#include "KEngine/Common/Transform2D.hpp"
#include "KEngine/Core/Entity.hpp"
#include "KEngine/Core/EventManager.hpp"
#include "KEngine/Log/Log.hpp"

namespace pf
{

    bool DebugDrawSystem::initialise()
    {
        ke::EventManager::registerListener<pf::RequestDrawDebugDotEvent>(this, &DebugDrawSystem::handleDebugDrawRequest);


        auto entity = ke::makeEntity(ke::Entity::newId());
        auto cameraComponent = ke::makeEntityComponent<ke::EntityCameraComponent>(entity);
        cameraComponent->setCameraNode(ke::makeSceneNode<ke::CameraNode>(entity->getId()));
        entity->addComponent(cameraComponent);
        ke::App::instance()->getLogic()->getEntityManager()->addEntity(entity);
        ke::App::instance()->getLogic()->getCurrentHumanView()->getScene()->setCameraNode(cameraComponent->getCameraNode());
        ke::App::instance()->getLogic()->getCurrentHumanView()->attachEntity(entity->getId());




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
                "component_name" : "EntityRenderableCircleShapeComponent",
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

            auto & position = actualEvent->getPosition();
            ke::Log::instance()->info("creating dot at ({}, {})", position.x, position.y);
            for (auto i = 0; i < 1; ++i)
            {
                auto entity = ke::App::instance()->getLogic()->getEntityManager()->newEntity().lock();
                ke::Transform2D transform{ position.x, position.y };
                //auto renderComponent = ke::makeEntityComponent<ke::EntityRenderableCircleShapeComponent>(entity, transform, ke::Color::GREEN);
                //entity->addComponent(renderComponent);
                auto textureId = std::hash<ke::String>()("bTile16Dead");
                transform.scaleX = 1.0f;
                transform.scaleY = 1.0f;
                ke::Rect2DInt32 textureRect;
                textureRect.top    = 16;
                textureRect.left   = 48;
                textureRect.width  = 16;
                textureRect.height = 16;
                std::int16_t depth = 50;
                entity->addComponent(ke::makeEntityComponent<ke::SpriteDrawableComponent>(entity, transform, depth, textureId, textureRect));

                auto result = entity->initialise();
                assert(result);

                //ke::App::instance()->getLogic()->getCurrentHumanView()->getScene()->addNode(renderComponent->getSceneNode()); // TODO: potentially remove this if we are going to go with the approach where renderable components broadcast events when a scene node is created.

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
