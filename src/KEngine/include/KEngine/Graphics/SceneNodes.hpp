#pragma once

#include "RenderCommand.hpp"
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

        virtual ke::RenderCommand getRenderCommand() const override
        {
            ke::RenderCommand cmd;
            cmd.type = ke::RenderObjectType::Invisible;
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

        virtual ke::RenderCommand getRenderCommand() const override
        {
            return ke::RenderCommand();
        }

        inline const ke::Dimension2D & getDimension() const { return this->dimension; }

    private:
        ke::Dimension2D dimension;
    };


    class CircleShapeNode : public ke::SceneNode
    {
    public:
        static ke::SceneNodeSptr create(ke::EntityId entityId, ke::Color fillColor = ke::Color::GREEN, float radius = 5.0f,
            ke::Color outlineColor = ke::Color::TRANSPARENT, float outlineThickness = 0.0f);

        using SceneNode::SceneNode;

        virtual RenderCommand getRenderCommand() const final;

    private:
        ke::RenderCommand states;
    };

}