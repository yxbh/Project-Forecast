#include "KEngine/Graphics/SceneNodes.hpp"

namespace ke
{

    ke::SceneNodeSptr ke::RootNode::create()
    {
        return ke::makeSceneNode<ke::RootNode>(ke::INVALID_ENTITY_ID);
    }

    ke::SceneNodeSptr CircleShapeNode::create(ke::EntityId entityId, const ke::Transform2D & localTransform,
        ke::Color fillColor, float radius, ke::Color outlineColor, float outlineThickness)
    {
        auto newNode = ke::makeSceneNode<ke::CircleShapeNode>(entityId);
        newNode->setLocalTransform(localTransform);
        auto & states           = newNode->states;
        states.type                    = ke::GraphicsCommand::Types::RenderCircleShape;
        states.render.radius           = radius;
        states.render.origin           = { radius, radius };
        states.render.outlineThickness = outlineThickness;
        states.render.fillColor        = fillColor;
        states.render.outlineColor     = outlineColor;
        return newNode;
    }

    GraphicsCommand CircleShapeNode::getGraphicsCommand() const
    {
        this->states.render.globalTransform = this->getGlobalTransform();
        return this->states;
    }

}
