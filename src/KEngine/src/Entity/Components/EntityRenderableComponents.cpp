#include "KEngine/Entity/Components/EntityRenderableComponents.hpp"

namespace ke
{

    int32_t const TiledSpriteDrawablwComponent::MIN_TILE_X_BOUND = -2000;
    int32_t const TiledSpriteDrawablwComponent::MAX_TILE_X_BOUND = 4000;
    int32_t const TiledSpriteDrawablwComponent::MIN_TILE_Y_BOUND = -4000;
    int32_t const TiledSpriteDrawablwComponent::MAX_TILE_Y_BOUND = 4000;

    TiledSpriteDrawablwComponent::TiledSpriteDrawablwComponent(ke::EntitySptr entity, const ke::Transform2D & localTransform, std::int32_t depth, size_t textureId,
        const ke::Rect2DInt32 & textureRect, const ke::Color & color, bool tileXDirection, bool tileYDirection)
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
                ke::Transform2D transform{ static_cast<float>(x), static_cast<float>(y) };
                auto spriteNode = ke::SpriteNode::create(ISceneNode::newId(), transform, depth, textureId, textureRect, color);
                this->sceneNode->addChild(spriteNode);
            }
        }
    }
    
}