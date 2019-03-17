#include "DebugDrawSystem.hpp"

#include "../AssetResources/GMSRoomResource.hpp"

#include "KEngine/App.hpp"
#include "../Events/RequestDrawDebugDotEvent.hpp"
#include "KEngine/Entity/Components/EntityRenderableComponents.hpp"
#include "KEngine/Common/Transform2D.hpp"
#include "KEngine/Core/Entity.hpp"
#include "KEngine/Core/EventManager.hpp"
#include "KEngine/Core/EntityFactory.hpp"
#include "KEngine/Log/Log.hpp"

#include <limits>

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
        KE_UNUSED(elapsedTime);
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
            ke::Log::instance()->debug("creating dot at ({}, {})", position.x, position.y);
            for (auto i = 0; i < 1; ++i)
            {
                //auto entity = ke::App::instance()->getLogic()->getEntityManager()->newEntity().lock();
                ke::Transform2D transform{ position.x, position.y };
                ////auto renderComponent = ke::makeEntityComponent<ke::EntityRenderableCircleShapeComponent>(entity, transform, ke::Color::GREEN);
                ////entity->addComponent(renderComponent);
                //auto textureId = std::hash<ke::String>()("bTile16Dead");
                //transform.scaleX = 1.0f;
                //transform.scaleY = 1.0f;
                //ke::Rect2DInt32 textureRect;
                //textureRect.top    = 16;
                //textureRect.left   = 48;
                //textureRect.width  = 16;
                //textureRect.height = 16;
                //ke::graphics::DepthType depth = std::numeric_limits<ke::graphics::DepthType>::max();
                //entity->addComponent(ke::makeEntityComponent<ke::SpriteDrawableComponent>(entity, transform, depth, textureId, textureRect));

                //auto result = entity->initialise();
                //assert(result);


                GMSRoomObjectInstance roomObject;
                roomObject.obj = "oWaterfall";
                roomObject.instanceid = ke::App::instance()->getLogic()->getEntityManager()->newId(); //100264;
                roomObject.pos = { static_cast<int>(position.x), static_cast<int>(position.y) };
                roomObject.scale = { 1.0f, 1.0f };
                roomObject.rotation = 0.0f;
                roomObject.colour = ke::Color::WHITE;
                auto entity = ke::App::instance()->getLogic()->getEntityFactory()->createNew("oWaterfall", roomObject);
                assert(entity);
            }
            

            break;
        }

        default:
            ke::Log::instance()->warn("Unexpected event {} received by {}.", event->getName(), this->getName());
            break;
        }
    }

}
