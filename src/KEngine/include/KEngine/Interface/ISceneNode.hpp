#pragma once

#include <memory>

namespace ke
{

    /// <summary>
    /// Interface for implementing scene nodes used for rendering.
    /// </summary>
    class ISceneNode
    {
    public:
    };

    using SceneNodeUptr = std::unique_ptr<ISceneNode>;
    using SceneNodeSptr = std::shared_ptr<ISceneNode>;
    using SceneNodeWptr = std::weak_ptr<ISceneNode>;

}