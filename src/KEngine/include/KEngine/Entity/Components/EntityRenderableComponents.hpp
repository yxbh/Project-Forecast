#pragma once

#include "KEngine/Events/OtherGraphicsEvents.hpp"
#include "KEngine/Core/EventManager.hpp"
#include "KEngine/Core/Entity.hpp"
#include "KEngine/Graphics/SceneNodes.hpp"
#include "KEngine/Interfaces/IEntityComponent.hpp"

#include "KEngine/Common/Point2D.hpp"

#include <cassert>

namespace ke
{

    class EntityRenderableComponent : public ke::IEntityComponent
    {
        KE_DEFINE_ENTITY_COMPONENT_COMMON_PROPERTIES(EntityRenderableComponent, 0xC2CFA5BB)

    public:
        using IEntityComponent::IEntityComponent;

        virtual ~EntityRenderableComponent()
        {
            ke::EventManager::enqueue(ke::makeEvent<ke::SceneNodeDestroyRequestEvent>(this->sceneNode));
        }

        virtual bool initialise()
        {
            ke::EventManager::enqueue(ke::makeEvent<ke::SceneNodeCreatedEvent>(this->sceneNode));
            this->setInitialised();
            return true;
        }

        inline void setSceneNode(ke::SceneNodeSptr p_sceneNode)
        {
            assert(p_sceneNode);
            this->sceneNode = p_sceneNode;
        }

        inline ke::SceneNodeSptr getSceneNode()
        {
            return this->sceneNode;
        }

    protected:
        ke::SceneNodeSptr sceneNode;
    };


    class EntityRenderableCircleShapeComponent : public ke::EntityRenderableComponent
    {
        KE_DEFINE_ENTITY_COMPONENT_COMMON_PROPERTIES(EntityRenderableCircleShapeComponent, 0x8617353F)

    public:
        EntityRenderableCircleShapeComponent(ke::EntitySptr entity, const ke::Transform2D & localTransform, std::int16_t depth,
            const ke::Color & fillColor, float radius = 5.0f, const ke::Color & outlineColor = ke::Color::TRANSPARENT,
            float outlineThickness = 0.0f)
            : EntityRenderableComponent(entity)
        {
            this->sceneNode = ke::CircleShapeNode::create(entity->getId(), localTransform,
                depth, fillColor, radius, outlineColor, outlineThickness);
        }

    };

    class SpriteDrawableComponent : public ke::EntityRenderableComponent
    {
        KE_DEFINE_ENTITY_COMPONENT_COMMON_PROPERTIES(SpriteDrawableComponent, 0xdc84005a)

    public:
        SpriteDrawableComponent(ke::EntitySptr entity, const ke::Transform2D & localTransform, std::int16_t depth, size_t textureId,
            const ke::Rect2DInt32 & textureRect, const ke::Color & color = ke::Color::WHITE)
            : EntityRenderableComponent(entity)
        {
            this->sceneNode = ke::SpriteNode::create(entity->getId(), localTransform, depth, textureId, textureRect, color);
        }

    };

    class LineDrawableComponent : public ke::EntityRenderableComponent
    {
        KE_DEFINE_ENTITY_COMPONENT_COMMON_PROPERTIES(LineDrawableComponent, 0x63917d41)

    public:
        LineDrawableComponent(ke::EntitySptr entity, const Point2DFloat & begin, const Point2DFloat & end,
            std::int16_t depth, const ke::Color & color = ke::Color::WHITE)
            : EntityRenderableComponent(entity)
        {
            this->sceneNode = ke::LineNode::create(entity->getId(), begin, end, depth, color);
        }

    };

}