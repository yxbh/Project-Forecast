#pragma once

#include "KEngine/Interfaces/IEntity.hpp"
#include "KEngine/Interfaces/ISceneNode.hpp"
#include "KEngine/Interfaces/IEntityComponent.hpp"

#include "KEngine/Common/Point2D.hpp"

#include <cassert>
#include <cstdint>

namespace ke
{

    class EntityRenderableComponent : public ke::IEntityComponent
    {
        KE_DEFINE_ENTITY_COMPONENT_COMMON_PROPERTIES(EntityRenderableComponent, 0xC2CFA5BB)

    public:
        using IEntityComponent::IEntityComponent;

        virtual ~EntityRenderableComponent(void);

        virtual bool initialise(void);

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
        EntityRenderableCircleShapeComponent(ke::EntitySptr entity, const ke::Transform2D & localTransform, std::int32_t depth,
            const ke::Color & fillColor, float radius = 5.0f, const ke::Color & outlineColor = ke::Color::TRANSPARENT,
            float outlineThickness = 0.0f);

    };

    class SpriteDrawableComponent : public ke::EntityRenderableComponent
    {
        KE_DEFINE_ENTITY_COMPONENT_COMMON_PROPERTIES(SpriteDrawableComponent, 0xdc84005a)

    public:
        SpriteDrawableComponent(ke::EntitySptr entity, const ke::Transform2D & localTransform, std::int32_t depth, size_t textureId,
            const ke::Rect2DInt32 & textureRect, const ke::Color & color = ke::Color::WHITE);

    };

    class LineDrawableComponent : public ke::EntityRenderableComponent
    {
        KE_DEFINE_ENTITY_COMPONENT_COMMON_PROPERTIES(LineDrawableComponent, 0x63917d41)

    public:
        LineDrawableComponent(ke::EntitySptr entity, const Point2DFloat & begin, const Point2DFloat & end,
            std::int32_t depth, const ke::Color & color = ke::Color::WHITE);

    };

    class TiledSpriteDrawablwComponent : public ke::EntityRenderableComponent
    {
        KE_DEFINE_ENTITY_COMPONENT_COMMON_PROPERTIES(TiledSpriteDrawablwComponent, 0x2a68065e)

    public:
        TiledSpriteDrawablwComponent(ke::EntitySptr entity, const ke::Transform2D & localTransform, std::int32_t depth, size_t textureId,
            const ke::Rect2DInt32 & textureRect, const ke::Color & color = ke::Color::WHITE, bool tileXDirection = false, bool tileYDirection = false);

    private:
        static const int32_t MIN_TILE_X_BOUND;
        static const int32_t MAX_TILE_X_BOUND;
        static const int32_t MIN_TILE_Y_BOUND;
        static const int32_t MAX_TILE_Y_BOUND;

        bool m_tileX = false;
        bool m_tileY = false;
    };

}