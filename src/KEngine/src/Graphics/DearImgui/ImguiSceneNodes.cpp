#include "KEngine/Graphics/DearImgui/ImguiSceneNodes.hpp"

namespace ke
{

    ke::SceneNodeSptr ImguiNode::create(ke::SceneNodeId SceneNodeId)
    {
        auto newNode = ke::makeSceneNode<ke::ImguiNode>(SceneNodeId);
        auto & states = newNode->states;
        states.type = ke::GraphicsCommand::Types::DrawImgui;
        return newNode;
    }

    ke::GraphicsCommand ImguiNode::getGraphicsCommand(void) const
    {
        return this->states;
    }

} // ns ke