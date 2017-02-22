#include "KEngine/Graphics/RenderSystem.hpp"

#include <queue>

namespace ke
{

    void RenderSystem::prepareRenderCommands(ke::Scene * scene)
    {
        if (!scene) return; // ignore null scenes;
        
        this->logicThreadRenderCommandQueue.clear();
        this->logicThreadRenderCommandQueue.reserve(2148);
        std::queue<ke::ISceneNode*> nodes;
        nodes.push(scene->getRootNode());
        while (nodes.size())
        {
            auto node = nodes.front();
            nodes.pop();
            this->logicThreadRenderCommandQueue.emplace_back(std::move(node->getRenderCommand()));
            for (auto child : node->getChildren())
            {
                nodes.push(child.get());
            }
        }
    }

    void RenderSystem::dispatchRenderCommands()
    {

    }

    void RenderSystem::processRenderCommands(ke::Time elapsedTime)
    {

    }

    void RenderSystem::render()
    {

    }

}