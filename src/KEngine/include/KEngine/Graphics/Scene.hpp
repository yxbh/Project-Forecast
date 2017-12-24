#pragma once

#include "SceneNodes.hpp"

#include "KEngine/Interfaces/IEntity.hpp"
#include "KEngine/Interfaces/IEvent.hpp"
#include "KEngine/Interfaces/IScene.hpp"
#include "KEngine/Interfaces/ISceneNode.hpp"

#include <unordered_map>

namespace ke
{

    class Scene
    {
    public:
        using EntitySceneNodeMap = std::unordered_map<ke::EntityId, SceneNodeSptr>;
        
        Scene();
        virtual ~Scene();

        bool addNode(ke::SceneNodeSptr);
        bool removeNode(ke::EntityId);
        ke::SceneNodeSptr findNode(ke::EntityId);

        inline void setCameraNode(std::shared_ptr<ke::CameraNode> p_cameraNode)
        {
            this->cameraNode = p_cameraNode;
        }

        inline ke::ISceneNode * getCameraNode() { return this->cameraNode.get(); }

        inline ke::ISceneNode * getRootNode() { return this->rootNode.get(); }

    private:
        void handleSceneNodeDestroyRequest(ke::EventSptr event);

    protected:
        ke::SceneNodeSptr rootNode;
        ke::SceneNodeSptr cameraNode;

        EntitySceneNodeMap entitySceneNodeMap;
    };

}