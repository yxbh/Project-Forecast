#pragma once

#include "KEngine/Graphics/SceneNodes.hpp"

namespace ke
{

    ke::SceneNodeSptr ke::RootNode::create()
    {
        return ke::makeSceneNode<ke::RootNode>(ke::INVALID_ENTITY_ID);
    }

    ke::SceneNodeSptr CircleShapeNode::create(ke::EntityId entityId,
        ke::Color fillColor, float radius, ke::Color outlineColor, float outlineThickness)
    {
        auto newNode = ke::makeSceneNode<ke::CircleShapeNode>(entityId);
        auto & states           = newNode->states;
        states.type             = ke::RenderObjectType::CircleShape;
        states.radius           = radius;
        states.outlineThickness = outlineThickness;
        states.fillColor        = fillColor;
        states.outlineColor     = outlineColor;
        return newNode;
    }

    RenderCommand CircleShapeNode::getRenderCommand() const
    {
        return this->states;
    }

}