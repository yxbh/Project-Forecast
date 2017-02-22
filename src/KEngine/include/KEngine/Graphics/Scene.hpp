#pragma once

#include "SceneNodes.hpp"

#include "KEngine/Interface/IEntity.hpp"
#include "KEngine/Interface/IScene.hpp"
#include "KEngine/Interface/ISceneNode.hpp"

#include <unordered_map>

namespace ke
{

    class Scene
    {
    public:
        using EntitySceneNodeMap = std::unordered_map<ke::EntityId, SceneNodeSptr>;
        
        Scene();

        bool addNode(ke::SceneNodeSptr);
        bool removeNode(ke::EntityId);
        ke::SceneNodeSptr findNode(ke::EntityId);

        inline void setCameraNode(std::shared_ptr<ke::CameraNode> p_cameraNode)
        {
            this->cameraNode = p_cameraNode;
        }

        inline ke::ISceneNode * getCameraNode() { return this->cameraNode.get(); }

        inline ke::ISceneNode * getRootNode() { return this->rootNode.get(); }

    protected:
        ke::SceneNodeSptr rootNode;
        ke::SceneNodeSptr cameraNode;

        EntitySceneNodeMap entitySceneNodeMap;
    };

}