#include "KEngine/Graphics/SceneNodes.hpp"

#include <algorithm>


namespace ke
{
    void SceneNode::update(const ke::Time & p_elapsedTime)
    {
        for (auto child : this->getChildren())
        {
            child->update(p_elapsedTime);
        }
    }


    ke::SceneNodeSptr ke::RootNode::create()
    {
        return ke::makeSceneNode<ke::RootNode>(ke::INVALID_ENTITY_ID);
    }


    ke::SceneNodeSptr InvisibleContainerNode::create(ke::SceneNodeId sceneNodeId,
        const ke::Transform2D & localTransform)
    {
        auto newNode = ke::makeSceneNode<ke::InvisibleContainerNode>(sceneNodeId);
        newNode->setLocalTransform(localTransform);
        return newNode;
    }

    ke::GraphicsCommand InvisibleContainerNode::getGraphicsCommand() const
    {
        ke::GraphicsCommand cmd;
        cmd.type = ke::GraphicsCommand::Types::DrawInvisible;
        return cmd;
    }

    ke::CameraNode::Sptr CameraNode::create(ke::SceneNodeId newSceneNodeId)
    {
        return ke::makeSceneNode<ke::CameraNode>(newSceneNodeId);
    }

    ke::GraphicsCommand CameraNode::getGraphicsCommand() const
    {
        ke::GraphicsCommand cmd;
        cmd.type = ke::GraphicsCommand::Types::SetViewContext;
        ke::graphics::ViewContextInfo viewInfo;
        viewInfo.transform = this->getGlobalTransform();
        viewInfo.dimension = this->getViewDimension();
        cmd.info = viewInfo;
        return cmd;
    }

    ke::SceneNodeSptr CircleShapeNode::create(ke::SceneNodeId sceneNodeId,
        const ke::Transform2D & localTransform, std::int32_t depth,
        ke::Color fillColor, float radius,
        ke::Color outlineColor, float outlineThickness)
    {
        auto newNode = ke::makeSceneNode<ke::CircleShapeNode>(sceneNodeId);
        newNode->setLocalTransform(localTransform);
        auto & states   = newNode->states;
        states.type     = ke::GraphicsCommand::Types::DrawCircleShape;
        states.id       = newNode->getId();
        states.depth    = depth;
        ke::graphics::CircleShapeRenderInfo shapeInfo;
        shapeInfo.radius           = radius;
        shapeInfo.origin           = { radius, radius };
        shapeInfo.outlineThickness = outlineThickness;
        shapeInfo.fillColor        = fillColor;
        shapeInfo.outlineColor     = outlineColor;
        states.info = shapeInfo;
        return newNode;
    }

    GraphicsCommand CircleShapeNode::getGraphicsCommand() const
    {
        auto & shapeInfo = std::get<ke::graphics::CircleShapeRenderInfo>(states.info);
        shapeInfo.globalTransform = this->getGlobalTransform();
        return this->states;
    }


    ke::SceneNodeSptr LineNode::create(ke::SceneNodeId sceneNodeId, const Point2DFloat & begin,
        const Point2DFloat & end, std::int32_t depth, const ke::Color & color)
    {
        auto newNode = ke::makeSceneNode<ke::LineNode>(sceneNodeId);
        auto & states   = newNode->states;
        states.type     = ke::GraphicsCommand::Types::DrawLine;
        states.id       = newNode->getId();
        states.depth    = depth;
        ke::graphics::LineRenderInfo lineInfo;
        lineInfo.begin  = begin;
        lineInfo.end    = end;
        lineInfo.color  = color;
        states.info     = lineInfo;
        return newNode;
    }

    GraphicsCommand LineNode::getGraphicsCommand() const
    {
        return this->states;
    }


    ke::SceneNodeSptr SpriteNode::create(
        ke::SceneNodeId sceneNodeId, const ke::Transform2D & localTransform, const ke::Point2DInt32 & origin,
        std::int32_t depth, size_t textureId, const ke::Rect2DInt32 & textureRect, ke::Color color)
    {
        auto newNode = ke::makeSceneNode<ke::SpriteNode>(sceneNodeId);
        newNode->setLocalTransform(localTransform);
        auto & states = newNode->states;
        states.type             = ke::GraphicsCommand::Types::DrawSprite;
        states.id               = newNode->getId();
        states.depth            = depth;
        ke::graphics::SpriteRenderInfo spriteInfo;
        spriteInfo.origin       = { static_cast<float>(origin.x), static_cast<float>(origin.y) };  // int to float conversion.
        spriteInfo.textureId    = textureId;
        spriteInfo.textureRect  = textureRect;
        spriteInfo.color        = color;
        states.info = spriteInfo;
        return newNode;
    }

    GraphicsCommand SpriteNode::getGraphicsCommand() const
    {
        auto & spriteInfo = std::get<ke::graphics::SpriteRenderInfo>(this->states.info);
        spriteInfo.globalTransform = this->getGlobalTransform();
        return this->states;
    }


    ke::SceneNodeSptr AnimatedSpriteNode::create(
        ke::SceneNodeId sceneNodeId, const ke::Transform2D & localTransform, OriginContainer && frameOrigins,
        std::int32_t depth, TextureIdContainer && frameTextureIds, TextureRectContainer && textureRects,
        const ke::Time frameDuration, const std::size_t startingFrame, const ke::Color color)
    {
        assert(frameOrigins.size());
        assert(frameTextureIds.size());
        assert(textureRects.size());
        assert(frameOrigins.size() == frameTextureIds.size() && frameTextureIds.size() == textureRects.size());
        assert(startingFrame < textureRects.size());
        auto newNode = ke::makeSceneNode<ke::AnimatedSpriteNode>(sceneNodeId);
        newNode->setLocalTransform(localTransform);
        newNode->origins         = std::forward<OriginContainer>(frameOrigins);
        newNode->frameTextureIds = std::forward<TextureIdContainer>(frameTextureIds);
        newNode->frameRects      = std::forward<TextureRectContainer>(textureRects);
        newNode->frameDuration   = frameDuration;
        newNode->currentFrame    = startingFrame;
        auto & states           = newNode->states;
        states.type             = ke::GraphicsCommand::Types::DrawSprite;
        states.id               = newNode->getId();
        states.depth            = depth;
        ke::graphics::SpriteRenderInfo spriteInfo;
        spriteInfo.origin       = { static_cast<float>(newNode->origins[startingFrame].x), static_cast<float>(newNode->origins[startingFrame].y) };  // int to float conversion.
        spriteInfo.textureId    = newNode->frameTextureIds[startingFrame];
        spriteInfo.textureRect  = newNode->frameRects[startingFrame];
        spriteInfo.color        = color;
        states.info = spriteInfo;
        return newNode;
    }

    GraphicsCommand AnimatedSpriteNode::getGraphicsCommand() const
    {
        auto & spriteInfo = std::get<ke::graphics::SpriteRenderInfo>(states.info);
        spriteInfo.globalTransform = this->getGlobalTransform();
        spriteInfo.origin      = { static_cast<float>(this->origins[this->currentFrame].x), static_cast<float>(this->origins[this->currentFrame].y) };  // int to float conversion.
        spriteInfo.textureId   = this->frameTextureIds[this->currentFrame];
        spriteInfo.textureRect = this->frameRects[this->currentFrame];
        return this->states;
    }

    void AnimatedSpriteNode::update(const ke::Time & p_timeDelta)
    {
        this->frameTimeCounter += p_timeDelta;
        while (this->frameTimeCounter >= this->frameDuration)
        {
            this->frameTimeCounter -= this->frameDuration;
            this->currentFrame += 1;
        }
        this->currentFrame = this->currentFrame % this->frameRects.size();

        this->ke::SceneNode::update(p_timeDelta);
    }
}
