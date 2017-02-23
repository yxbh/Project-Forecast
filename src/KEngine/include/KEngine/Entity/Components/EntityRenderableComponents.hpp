#pragma once

#include "KEngine/Graphics/SceneNodes.hpp"
#include "KEngine/Interface/IEntityComponent.hpp"
#include <cassert>

namespace ke
{

    class EntityRenderableComponent : public ke::IEntityComponent
    {
        KE_DEFINE_ENTITY_COMPONENT_COMMON_PROPERTIES(EntityRenderableComponent, 0xC2CFA5BB)

    public:
        using IEntityComponent::IEntityComponent;

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
        EntityRenderableCircleShapeComponent(ke::EntitySptr entity, const ke::Transform2D & localTransform, const ke::Color & fillColor,
            float radius = 5.0f, const ke::Color & outlineColor = ke::Color::TRANSPARENT, float outlineThickness = 0.0f)
            : EntityRenderableComponent(entity)
        {
            this->sceneNode = ke::CircleShapeNode::create(entity->getId(), localTransform,
                fillColor, radius, outlineColor, outlineThickness);
        }

        virtual bool initialise() final
        {
            // broadcast SceneNode created event.
            return true;
        }

    };

}