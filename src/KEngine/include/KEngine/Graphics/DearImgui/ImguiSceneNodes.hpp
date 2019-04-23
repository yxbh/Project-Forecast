#pragma once

#include <KEngine/Graphics/SceneNodes.hpp>

namespace ke
{
    // some forward declarations
    class AppConsoleSystem;
    namespace priv {
        class AppConsoleSystem;
    }


    class ImguiNode : public ke::SceneNode
    {
    public:
        using SceneNode::SceneNode;

        static ke::SceneNodeSptr create(ke::SceneNodeId SceneNodeId);

        virtual ke::GraphicsCommand getGraphicsCommand(void) const override;

    private:
        mutable ke::GraphicsCommand states;

        friend class ::ke::AppConsoleSystem;
        friend class ::ke::priv::AppConsoleSystem;
    };

} // ke ns