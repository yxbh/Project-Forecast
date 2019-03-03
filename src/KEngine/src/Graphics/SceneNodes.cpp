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

    ke::CameraNode::Sptr CameraNode::create(ke::SceneNodeId newSceneNodeId)
    {
        return ke::makeSceneNode<ke::CameraNode>(newSceneNodeId);
    }

    ke::SceneNodeSptr CircleShapeNode::create(ke::SceneNodeId sceneNodeId,
        const ke::Transform2D & localTransform, std::int32_t depth,
        ke::Color fillColor, float radius,
        ke::Color outlineColor, float outlineThickness)
    {
        auto newNode = ke::makeSceneNode<ke::CircleShapeNode>(sceneNodeId);
        newNode->setLocalTransform(localTransform);
        auto & states                 = newNode->states;
        states.type                   = ke::GraphicsCommand::Types::RenderCircleShape;
        states.shape.id               = newNode->getId();
        states.shape.radius           = radius;
        states.shape.origin           = { radius, radius };
        states.shape.outlineThickness = outlineThickness;
        states.shape.fillColor        = fillColor;
        states.shape.outlineColor     = outlineColor;
        states.shape.depth            = depth;
        return newNode;
    }

    GraphicsCommand CircleShapeNode::getGraphicsCommand() const
    {
        this->states.shape.globalTransform = this->getGlobalTransform();
        return this->states;
    }


    ke::SceneNodeSptr LineNode::create(ke::SceneNodeId sceneNodeId, const Point2DFloat & begin,
        const Point2DFloat & end, std::int32_t depth, const ke::Color & color)
    {
        auto newNode = ke::makeSceneNode<ke::LineNode>(sceneNodeId);
        auto & states     = newNode->states;
        states.type       = ke::GraphicsCommand::Types::RenderLine;
        states.line.id    = newNode->getId();
        states.line.depth = depth;
        states.line.begin = begin;
        states.line.end   = end;
        states.line.color = color;
        return newNode;
    }

    GraphicsCommand LineNode::getGraphicsCommand() const
    {
        return this->states;
    }


    ke::SceneNodeSptr SpriteNode::create(ke::SceneNodeId sceneNodeId, const ke::Transform2D & localTransform,
        std::int32_t depth, size_t textureId, const ke::Rect2DInt32 & textureRect, ke::Color color)
    {
        auto newNode = ke::makeSceneNode<ke::SpriteNode>(sceneNodeId);
        newNode->setLocalTransform(localTransform);
        auto & states = newNode->states;
        states.type = ke::GraphicsCommand::Types::RenderSprite;
        states.sprite.id = newNode->getId();
        states.sprite.depth = depth;
        states.sprite.textureId = textureId;
        states.sprite.textureRect = textureRect;
        states.sprite.color = color;
        return newNode;
    }

    GraphicsCommand SpriteNode::getGraphicsCommand() const
    {
        this->states.shape.globalTransform = this->getGlobalTransform();
        return this->states;
    }


    ke::SceneNodeSptr AnimatedSpriteNode::create(
        ke::SceneNodeId sceneNodeId, const ke::Transform2D & localTransform, std::int32_t depth,
        size_t textureId, const TextureRectContainer & textureRects, ke::Time frameDuration, ke::Color color)
    {
        assert(textureRects.size());
        auto newNode = ke::makeSceneNode<ke::AnimatedSpriteNode>(sceneNodeId);
        newNode->setLocalTransform(localTransform);
        newNode->frameDuration = frameDuration;
        auto & states = newNode->states;
        states.type = ke::GraphicsCommand::Types::RenderSprite;
        states.sprite.id = newNode->getId();
        states.sprite.depth = depth;
        states.sprite.textureId = textureId;
        states.sprite.textureRect = textureRects[0];
        states.sprite.color = color;
        return newNode;
    }

    GraphicsCommand AnimatedSpriteNode::getGraphicsCommand() const
    {
        this->states.shape.globalTransform = this->getGlobalTransform();
        states.sprite.textureRect = this->frameRects[this->currentFrame];
        return this->states;
    }

    void AnimatedSpriteNode::update(const ke::Time & p_timeDelta)
    {
        this->frameTimeCounter += p_timeDelta;
        while (this->frameTimeCounter >= this->frameDuration)
        {
            this->frameTimeCounter -= this->frameDuration;
            this->currentFrame = (this->currentFrame + 1) % this->frameRects.size();
        }

        this->SceneNode::update(p_timeDelta);
    }
}
