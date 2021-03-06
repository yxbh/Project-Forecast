#pragma once

#include "GraphicsCommand.hpp"
#include "KEngine/Common/Dimension2D.hpp"
#include "KEngine/Common/Rect2D.hpp"
#include "KEngine/Interfaces/ISceneNode.hpp"

namespace ke
{

    /// <summary>
    ///  A concrete base class for implementing scene nodes.
    /// </summary>
    class SceneNode : public ke::ISceneNode
    /// 
    {
    public:
        using ISceneNode::ISceneNode;

        virtual void update(const ke::Time & p_elapsedTime) override;
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
            cmd.type = ke::GraphicsCommand::Types::DrawInvisible;
            return cmd;
        }

    protected:
    };


    /// <summary>
    /// A container node purely for holding other scene nodes.
    /// </summary>
    class InvisibleContainerNode : public ke::SceneNode
    {
    public:
        static ke::SceneNodeSptr create(ke::SceneNodeId sceneNodeId,
            const ke::Transform2D & localTransform = {});

        using SceneNode::SceneNode;

        virtual ke::GraphicsCommand getGraphicsCommand() const override;
    };


    /// <summary>
    /// 
    /// </summary>
    class CameraNode : public ke::SceneNode
    {
    public:
        using Uptr = std::unique_ptr<CameraNode>;
        using Sptr = std::shared_ptr<CameraNode>;
        using Wptr = std::weak_ptr<CameraNode>;

        using SceneNode::SceneNode;

        static ke::CameraNode::Sptr create(ke::SceneNodeId newSceneNodeId = ke::ISceneNode::newId());

        virtual ke::GraphicsCommand getGraphicsCommand() const override;

        /// <summary>
        /// Get the view size of the camera.
        /// </summary>
        /// <returns></returns>
        inline const ke::Dimension2DUInt32 & getViewDimension() const { return this->dimension; }
        inline void setViewDimension(const ke::Dimension2DUInt32 & newDimension) { this->dimension = newDimension; }

    protected:
        ke::Dimension2DUInt32 dimension;
    };


    class CircleShapeNode : public ke::SceneNode
    {
    public:
        static ke::SceneNodeSptr create(ke::SceneNodeId sceneNodeId,
            const ke::Transform2D & localTransform, std::int32_t depth,
            ke::Color fillColor = ke::Color::GREEN, float radius = 5.0f,
            ke::Color outlineColor = ke::Color::TRANSPARENT, float outlineThickness = 0.0f);

        using SceneNode::SceneNode;

        virtual GraphicsCommand getGraphicsCommand() const final;

    private:
        mutable ke::GraphicsCommand states;
    };


    class LineNode : public ke::SceneNode
    {
    public:
        static ke::SceneNodeSptr create(ke::SceneNodeId sceneNodeId, const Point2DFloat & begin, const Point2DFloat & end,
            std::int32_t depth, const ke::Color & color = ke::Color::WHITE);

        using SceneNode::SceneNode;

        virtual GraphicsCommand getGraphicsCommand() const final;

    private:
        mutable ke::GraphicsCommand states;
    };


    class SpriteNode : public ke::SceneNode
    {
    public:
        static ke::SceneNodeSptr create(
            ke::SceneNodeId sceneNodeId, const ke::Transform2D & localTransform, const ke::Point2DInt32 & origin,
            std::int32_t depth, size_t textureId, const ke::Rect2DInt32 & textureRect, ke::Color fillColor = ke::Color::WHITE);

        using SceneNode::SceneNode;

        virtual GraphicsCommand getGraphicsCommand() const final;

    private:
        mutable ke::GraphicsCommand states;
    };


    class AnimatedSpriteNode : public ke::SceneNode
    {
    public:
        using Origin                = ke::Point2DInt32;
        using OriginContainer       = std::vector<Origin>;
        using TextureIdContainer    = std::vector<unsigned>;
        using TextureRectContainer  = std::vector<Rect2DInt32>;

        static ke::SceneNodeSptr create(
            ke::SceneNodeId sceneNodeId, const ke::Transform2D & localTransform, OriginContainer && frameOrigins,
            std::int32_t depth, TextureIdContainer && frameTextureIds, TextureRectContainer && textureRects,
            const ke::Time frameDuration, const std::size_t startingFrame = 0, const ke::Color fillColor = ke::Color::WHITE);

        using SceneNode::SceneNode;

        virtual GraphicsCommand getGraphicsCommand() const final;

        virtual void update(const ke::Time & p_timeDelta) override;

    private:
        mutable ke::GraphicsCommand states;

        std::size_t currentFrame = 0;
        OriginContainer origins;
        TextureIdContainer frameTextureIds;
        TextureRectContainer frameRects;
        ke::Time frameDuration;
        ke::Time frameTimeCounter;
    };
}
