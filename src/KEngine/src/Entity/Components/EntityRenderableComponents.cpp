#include "KEngine/Entity/Components/EntityRenderableComponents.hpp"

#include "KEngine/Core/EventManager.hpp"
#include "KEngine/Core/Entity.hpp"
#include "KEngine/Graphics/SceneNodes.hpp"
#include "KEngine/Events/OtherGraphicsEvents.hpp"

namespace ke
{

    EntityRenderableComponent::~EntityRenderableComponent(void)
    {
        ke::EventManager::enqueue(ke::makeEvent<ke::SceneNodeDestroyRequestEvent>(this->sceneNode));
    }

    bool EntityRenderableComponent::initialise(void)
    {
        ke::EventManager::enqueue(ke::makeEvent<ke::SceneNodeCreatedEvent>(this->sceneNode));
        this->setInitialised();
        return true;
    }


    EntityRenderableCircleShapeComponent::EntityRenderableCircleShapeComponent(
        ke::EntitySptr entity, const ke::Transform2D & localTransform, std::int32_t depth,
        const ke::Color & fillColor, float radius, const ke::Color & outlineColor, float outlineThickness)
        : EntityRenderableComponent(entity)
    {
        this->sceneNode = ke::CircleShapeNode::create(entity->getId(), localTransform,
            depth, fillColor, radius, outlineColor, outlineThickness);
    }


    SpriteDrawableComponent::SpriteDrawableComponent(
        ke::EntitySptr entity, const ke::Transform2D & localTransform, const ke::Point2DInt32 & origin,
        std::int32_t depth, size_t textureId, const ke::Rect2DInt32 & textureRect, const ke::Color & color)
        : EntityRenderableComponent(entity)
    {
        this->sceneNode = ke::SpriteNode::create(entity->getId(), localTransform, origin, depth, textureId, textureRect, color);
    }


    LineDrawableComponent::LineDrawableComponent(
        ke::EntitySptr entity, const Point2DFloat & begin, const Point2DFloat & end, std::int32_t depth, const ke::Color & color)
        : EntityRenderableComponent(entity)
    {
        this->sceneNode = ke::LineNode::create(entity->getId(), begin, end, depth, color);
    }


    int32_t const TiledSpriteDrawablwComponent::MIN_TILE_X_BOUND = -3000;
    int32_t const TiledSpriteDrawablwComponent::MAX_TILE_X_BOUND = 3000;
    int32_t const TiledSpriteDrawablwComponent::MIN_TILE_Y_BOUND = -2000;
    int32_t const TiledSpriteDrawablwComponent::MAX_TILE_Y_BOUND = 2000;

    TiledSpriteDrawablwComponent::TiledSpriteDrawablwComponent(
        ke::EntitySptr entity, const ke::Transform2D & localTransform, const ke::Point2DInt32 & origin,
        std::int32_t depth, size_t textureId, const ke::Rect2DInt32 & textureRect,
        const ke::Color & color, bool tileXDirection, bool tileYDirection)
        : EntityRenderableComponent(entity), m_tileX(tileXDirection), m_tileY(tileYDirection)
    {
        this->sceneNode = ke::InvisibleContainerNode::create(entity->getId(), localTransform);
        int32_t tile_x_start = 0;
        int32_t tile_x_end = 0;
        if (this->m_tileX)
        {
            tile_x_start = MIN_TILE_X_BOUND;
            tile_x_end = MAX_TILE_X_BOUND;
        }
        int32_t tile_y_start = 0;
        int32_t tile_y_end = 0;
        if (this->m_tileY)
        {
            tile_y_start = MIN_TILE_Y_BOUND;
            tile_y_end = MAX_TILE_Y_BOUND;
        }
        for (int32_t x = tile_x_start; x <= tile_x_end; x += textureRect.width)
        {
            for (int32_t y = tile_y_start; y <= tile_y_end; y += textureRect.height)
            {
                ke::Transform2D transform{ static_cast<ke::Transform2D::PointType>(x), static_cast<ke::Transform2D::PointType>(y) };
                auto spriteNode = ke::SpriteNode::create(ISceneNode::newId(), transform, origin, depth, textureId, textureRect, color);
                this->sceneNode->addChild(spriteNode);
            }
        }
    }
    
}