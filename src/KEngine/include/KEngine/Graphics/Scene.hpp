#pragma once

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

        bool addNode(ke::SceneNodeSptr);
        bool removeNode(ke::EntityId);
        ke::SceneNodeSptr findNode(ke::EntityId);
        inline ke::ISceneNode * getCameraNode() { return this->cameraNode.get(); }

    private:
        ke::SceneNodeSptr rootNode;
        ke::SceneNodeSptr cameraNode;

        EntitySceneNodeMap entitySceneNodeMap;
    };

}