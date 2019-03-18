#pragma once

#include "KEngine/Graphics/SceneNodes.hpp"

namespace pf
{
    // forward declarations

    class GMSSpriteResource;


    class GraphicsHelper
    {
    public:
        GraphicsHelper(void) = delete;

        static ke::AnimatedSpriteNode::OriginContainer computeSpriteOrigins(const pf::GMSSpriteResource * const spriteResource);
        static ke::AnimatedSpriteNode::TextureIdContainer computeSpriteTextureIds(const pf::GMSSpriteResource * const spriteResource);
        static ke::AnimatedSpriteNode::TextureRectContainer computeSpriteFrameRects(const pf::GMSSpriteResource * const spriteResource);

    };
}