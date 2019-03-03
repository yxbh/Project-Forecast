#pragma once

#include "KEngine/Interfaces/IEntity.hpp"
#include "KEngine/Interfaces/IEvent.hpp"
#include "KEngine/Interfaces/IScene.hpp"
#include "KEngine/Interfaces/ISceneNode.hpp"

#include "KEngine/Common/Time.hpp"

#include <unordered_map>

namespace ke
{

    class Scene
    {
    public:
        using EntitySceneNodeMap = std::unordered_map<ke::EntityId, SceneNodeSptr>;
        
        Scene();
        virtual ~Scene();

        virtual void update(ke::Time p_elapsedTime);

        bool addNode(ke::SceneNodeSptr);
        bool removeNode(ke::SceneNodeSptr);
        ke::SceneNodeSptr findNode(ke::EntityId);

        void setCameraNode(ke::SceneNodeSptr p_cameraNode);

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