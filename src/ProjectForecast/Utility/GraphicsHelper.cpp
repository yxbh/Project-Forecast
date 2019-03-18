#include "GraphicsHelper.hpp"

#include "../AssetResources/OtherGMSResources.hpp"

namespace pf
{
    ke::AnimatedSpriteNode::OriginContainer
    GraphicsHelper::computeSpriteOrigins(const pf::GMSSpriteResource * const spriteResource)
    {
        assert(spriteResource->texpageResources.size());
        ke::AnimatedSpriteNode::OriginContainer frameOrigins;
        frameOrigins.reserve(spriteResource->texpageResources.size());
        for (const auto & texpageResource : spriteResource->texpageResources)
        {
            // NOTE:
            //   We take account of texpage dest.x and dest.y by adjusting the texture origin with it.
            //   Not sure if this is the best way to handle non-zero dest coordinates
            //   in a texpage file but it works for now.
            const auto & destPos = texpageResource->destinationPosition;
            auto origin = spriteResource->origin;
            origin.x += destPos.x;
            origin.y -= destPos.y;  // convert from img coordinate system to world coordinate system.
            frameOrigins.emplace_back(origin);
        }
        return frameOrigins;
    }

    ke::AnimatedSpriteNode::TextureIdContainer
    GraphicsHelper::computeSpriteTextureIds(const pf::GMSSpriteResource * const spriteResource)
    {
        assert(spriteResource->texpageResources.size());
        ke::AnimatedSpriteNode::TextureIdContainer   frameTextureIds;
        frameTextureIds.reserve(spriteResource->texpageResources.size());
        for (const auto & texpageResource : spriteResource->texpageResources)
        {
            frameTextureIds.push_back(texpageResource->sheetid);
        }
        return frameTextureIds;
    }

    ke::AnimatedSpriteNode::TextureRectContainer
    GraphicsHelper::computeSpriteFrameRects(const pf::GMSSpriteResource * const spriteResource)
    {
        assert(spriteResource->texpageResources.size());
        ke::AnimatedSpriteNode::TextureRectContainer frameRects;
        frameRects.reserve(spriteResource->texpageResources.size());
        for (const auto & texpageResource : spriteResource->texpageResources)
        {
            ke::Rect2DInt32 rect;
            rect.left   = texpageResource->sourcePosition.x;
            rect.top    = texpageResource->sourcePosition.y;
            rect.width  = texpageResource->sourceDimension.width;
            rect.height = texpageResource->sourceDimension.height;
            frameRects.push_back(std::move(rect));
        }
        return frameRects;
    }
}