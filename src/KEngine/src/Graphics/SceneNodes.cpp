#include "KEngine/Graphics/SceneNodes.hpp"

namespace ke
{

    ke::SceneNodeSptr ke::RootNode::create()
    {
        return ke::makeSceneNode<ke::RootNode>(ke::INVALID_ENTITY_ID);
    }


    ke::SceneNodeSptr CircleShapeNode::create(ke::SceneNodeId sceneNodeId,
        const ke::Transform2D & localTransform, std::int16_t depth,
        ke::Color fillColor, float radius,
        ke::Color outlineColor, float outlineThickness)
    {
        auto newNode = ke::makeSceneNode<ke::CircleShapeNode>(sceneNodeId);
        newNode->setLocalTransform(localTransform);
        auto & states                  = newNode->states;
        states.type                    = ke::GraphicsCommand::Types::RenderCircleShape;
        states.shape.id               = newNode->getId();
        states.shape.radius           = radius;
        states.shape.origin           = { radius, radius };
        states.shape.outlineThickness = outlineThickness;
        states.shape.fillColor        = fillColor;
        states.shape.outlineColor     = outlineColor;
        return newNode;
    }

    GraphicsCommand CircleShapeNode::getGraphicsCommand() const
    {
        this->states.shape.globalTransform = this->getGlobalTransform();
        return this->states;
    }


    ke::SceneNodeSptr SpriteNode::create(ke::SceneNodeId sceneNodeId, const ke::Transform2D & localTransform,
        std::int16_t depth, size_t textureId, const ke::Rect2DInt32 & textureRect, ke::Color color)
    {
        auto newNode = ke::makeSceneNode<ke::SpriteNode>(sceneNodeId);
        newNode->setLocalTransform(localTransform);
        auto & states                = newNode->states;
        states.type                  = ke::GraphicsCommand::Types::RenderSprite;
        states.sprite.id             = newNode->getId();
        states.sprite.depth          = depth;
        states.sprite.textureId      = textureId;
        states.sprite.textureRect    = textureRect;
        states.sprite.color          = color;
        return newNode;
    }

    GraphicsCommand SpriteNode::getGraphicsCommand() const
    {
        this->states.shape.globalTransform = this->getGlobalTransform();
        return this->states;
    }


    ke::SceneNodeSptr LineNode::create(ke::SceneNodeId sceneNodeId, const Point2DFloat & begin,
        const Point2DFloat & end, std::int16_t depth, const ke::Color & color)
    {
        auto newNode = ke::makeSceneNode<ke::LineNode>(sceneNodeId);
        auto & states       = newNode->states;
        states.type         = ke::GraphicsCommand::Types::RenderLine;
        states.line.id      = newNode->getId();
        states.line.depth   = depth;
        states.line.begin   = begin;
        states.line.end     = end;
        states.sprite.color = color;
        return newNode;
    }

    GraphicsCommand LineNode::getGraphicsCommand() const
    {
        this->states.shape.globalTransform = this->getGlobalTransform();
        return this->states;
    }
}
