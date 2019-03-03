#include "KEngine/Graphics/Scene.hpp"

#include "KEngine/Graphics/SceneNodes.hpp"

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

    void Scene::update(ke::Time p_elapsedTime)
    {
        this->rootNode->update(p_elapsedTime); // NOTE: camera node is not updated if it is not attached in the scene.
    }

    bool Scene::addNode(ke::SceneNodeSptr p_node)
    {
        assert(p_node->getEntityId());
#if defined(KE_DEBUG)
        if (this->entitySceneNodeMap.find(p_node->getEntityId()) != this->entitySceneNodeMap.end())
        {
            ke::Log::instance()->error("A scene node associated with entity ID({}) already exists in the entity scenenode map.", p_node->getEntityId());
        }
#endif

        this->entitySceneNodeMap[p_node->getEntityId()] = p_node;
        return this->rootNode->addChild(p_node);
    }

    bool Scene::removeNode(ke::SceneNodeSptr p_node)
    {
        assert(nullptr != p_node);

        bool nodeRemoved = false;
        if (this->rootNode)
        {
            if (this->rootNode == p_node)
            {
                this->rootNode = nullptr;
                nodeRemoved = true;
            }
            if (!nodeRemoved)
            {
                nodeRemoved = this->rootNode->removeNode(p_node.get());
            }
        }
        if (this->cameraNode == p_node)
        {
            this->cameraNode = nullptr;
            nodeRemoved = true;
        }
        if (nodeRemoved)
        {
            this->entitySceneNodeMap.erase(p_node->getEntityId());
        }

        return nodeRemoved;
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

    void Scene::setCameraNode(ke::SceneNodeSptr p_cameraNode)
    {
        assert(nullptr != std::dynamic_pointer_cast<ke::CameraNode>(p_cameraNode));
        this->cameraNode = p_cameraNode;
    }

    void Scene::handleSceneNodeDestroyRequest(ke::EventSptr event)
    {
        if (event->getType() == ke::SceneNodeDestroyRequestEvent::TYPE)
        {
            auto request = static_cast<ke::SceneNodeDestroyRequestEvent*>(event.get());
            auto node = request->getSceneNode();
            this->removeNode(node);
        }
    }

}