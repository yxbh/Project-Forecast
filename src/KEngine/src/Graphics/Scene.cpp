#include "KEngine/Graphics/Scene.hpp"

#include "KEngine/Events/OtherGraphicsEvents.hpp"
#include "KEngine/Core/EventManager.hpp"
#include "KEngine/Log/Log.hpp"

#include <cassert>

namespace ke
{

    Scene::Scene()
        : rootNode(ke::RootNode::create())
    {
        ke::EventManager::registerListener<ke::SceneNodeDestroyRequestEvent>(this, &Scene::handleSceneNodeDestroyRequest);
    }

    Scene::~Scene()
    {
        ke::EventManager::deregisterListener<ke::SceneNodeDestroyRequestEvent>(this, &Scene::handleSceneNodeDestroyRequest);
    }

    bool Scene::addNode(ke::SceneNodeSptr node)
    {
        assert(node->getEntityId());
#if defined(KE_DEBUG)
        if (this->entitySceneNodeMap.find(node->getEntityId()) != this->entitySceneNodeMap.end())
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
        return this->rootNode->removeChildByEntityId(entityId);
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

    void Scene::handleSceneNodeDestroyRequest(ke::EventSptr event)
    {
        if (event->getType() == ke::SceneNodeDestroyRequestEvent::TYPE)
        {
            auto request = static_cast<ke::SceneNodeDestroyRequestEvent*>(event.get());
            auto node = request->getSceneNode();
            if (this->rootNode)
            {
                this->rootNode->removeChildrenByEntityId(node->getEntityId(), true);
                this->rootNode = (this->rootNode == node) ? nullptr : this->rootNode;
            }
            this->entitySceneNodeMap.erase(node->getEntityId());
            this->cameraNode = (this->cameraNode == node) ? nullptr : this->cameraNode;
        }
    }

}