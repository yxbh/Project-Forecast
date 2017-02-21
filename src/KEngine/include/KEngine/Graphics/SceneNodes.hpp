#pragma once

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
        static ke::SceneNodeSptr create();

    protected:
    };


    /// <summary>
    /// 
    /// </summary>
    class CameraNode : public ke::SceneNode
    {
    public:
        static ke::SceneNodeSptr create();

        inline const ke::Dimension2D & getDimension() const { return this->dimension; }

    private:
        ke::Dimension2D dimension;
    };

}