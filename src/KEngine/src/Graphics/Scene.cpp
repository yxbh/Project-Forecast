#include "KEngine/Graphics/Scene.hpp"

#include "KEngine/Log/Log.hpp"

#include <cassert>

namespace ke
{

    Scene::Scene()
        : rootNode(ke::RootNode::create())
    {
    }

    bool Scene::addNode(ke::SceneNodeSptr node)
    {
        assert(node->getEntityId());
#if defined(KE_DEBUG)
        if (this->entitySceneNodeMap.find(node->getEntityId()) == this->entitySceneNodeMap.end())
        {
            ke::Log::instance()->error("A scene node associated with entity ID({}) already exists in the entity scenenode map.", node->getEntityId());
        }
#endif

        this->entitySceneNodeMap[node->getEntityId()] = node;
        return this->rootNode->addChild(node);
    }

    bool Scene::removeNode(ke::EntityId entityId)
    {
        assert(entityId != ke::INVALID_ENTITY_ID);

        this->entitySceneNodeMap.erase(entityId);
        return this->rootNode->removeChild(entityId);
    }

    ke::SceneNodeSptr Scene::findNode(ke::EntityId entityId)
    {
        auto it = this->entitySceneNodeMap.find(entityId);
        if (it == this->entitySceneNodeMap.end())
        {
            return nullptr;
        }

        return it->second;
    }

}