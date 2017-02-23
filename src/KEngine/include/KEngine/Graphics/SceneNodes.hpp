#pragma once

#include "GraphicsCommand.hpp"
#include "KEngine/Common/Dimension2D.hpp"
#include "KEngine/Interface/ISceneNode.hpp"

namespace ke
{

    /// <summary>
    /// 
    /// </summary>
    class SceneNode : public ke::ISceneNode
    {
    public:
        using ISceneNode::ISceneNode;

    protected:
    };


    /// <summary>
    /// 
    /// </summary>
    class RootNode : public ke::SceneNode
    {
    public:
        static ke::SceneNodeSptr create();

        using SceneNode::SceneNode;

        virtual ke::GraphicsCommand getGraphicsCommand() const override
        {
            ke::GraphicsCommand cmd;
            cmd.type = ke::GraphicsCommand::Types::RenderInvisible;
            return cmd;
        }

    protected:
    };


    /// <summary>
    /// 
    /// </summary>
    class CameraNode : public ke::SceneNode
    {
    public:
        static ke::SceneNodeSptr create();

        using SceneNode::SceneNode;

        virtual ke::GraphicsCommand getGraphicsCommand() const override
        {
            return ke::GraphicsCommand();
        }

        inline const ke::Dimension2D & getDimension() const { return this->dimension; }

    private:
        ke::Dimension2D dimension;
    };


    class CircleShapeNode : public ke::SceneNode
    {
    public:
        static ke::SceneNodeSptr create(ke::EntityId entityId, const ke::Transform2D & localTransform = {},
            ke::Color fillColor = ke::Color::GREEN, float radius = 5.0f,
            ke::Color outlineColor = ke::Color::TRANSPARENT, float outlineThickness = 0.0f);

        using SceneNode::SceneNode;

        virtual GraphicsCommand getGraphicsCommand() const final;

    private:
        mutable ke::GraphicsCommand states;
    };

}