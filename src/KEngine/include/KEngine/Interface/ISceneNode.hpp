#pragma once

#include "IEntity.hpp"

#include "KEngine/Common/Point2D.hpp"
#include "KEngine/Common/Transform2D.hpp"

#include <memory>
#include <vector>

namespace ke
{

    class ISceneNode;

    using SceneNodeUptr = std::unique_ptr<ISceneNode>;
    using SceneNodeSptr = std::shared_ptr<ISceneNode>;
    using SceneNodeWptr = std::weak_ptr<ISceneNode>;


    /// <summary>
    /// Interface for implementing scene nodes used for rendering.
    /// </summary>
    class ISceneNode
    {
    public:
        using SceneNodeList = std::vector<SceneNodeSptr>;

        inline ke::EntityId getEntityId() const { return this->entityId; }
        inline ISceneNode * getParent() { return this->parentNode; }

        inline bool addChild(ke::SceneNodeSptr childNode)
        {
            this->childrenNodes.push_back(childNode);
            childNode->parentNode = this;

            return true;
        }

        inline const ke::Transform2D & getLocalTransform() const { return this->localTransform; }
        inline void setLocalTransform(const ke::Transform2D & p_localTransform) { this->localTransform = p_localTransform; }

        /// <summary>
        /// Find and remove the first child that contains a matching entity ID.
        /// </summary>
        /// <param name="entityId"></param>
        /// <returns>true if a matching scene node was found and erased.</returns>
        inline bool removeChild(ke::EntityId p_entityId)
        {
            for (auto it = this->childrenNodes.begin(); it != this->childrenNodes.end(); ++it)
            {
                auto sceneNode = it->get();
                if (sceneNode->getEntityId() != ke::INVALID_ENTITY_ID &&
                    sceneNode->getEntityId() == p_entityId)
                {
                    it = this->childrenNodes.erase(it);
                    return true;
                }
            }

            return false;
        }

        /// <summary>
        /// Find and remove the all first level children that contains a matching entity ID.
        /// </summary>
        /// <param name="entityId"></param>
        /// <returns>true if matching scene nodes were found and erased.</returns>
        inline bool removeChildren(ke::EntityId p_entityId)
        {
            bool foundAndRemove = false;
            for (auto it = this->childrenNodes.begin(); it != this->childrenNodes.end(); ++it)
            {
                auto sceneNode = it->get();
                if (sceneNode->getEntityId() != ke::INVALID_ENTITY_ID &&
                    sceneNode->getEntityId() == p_entityId)
                {
                    it = this->childrenNodes.erase(it);
                    foundAndRemove = true;
                }
            }

            return foundAndRemove;
        }

    protected:
        inline void setEntityId(ke::EntityId p_entityId) { this->entityId = p_entityId; }

        ke::EntityId entityId = ke::INVALID_ENTITY_ID;
        ke::Transform2D localTransform; // transform relative to its parent.

        ISceneNode * parentNode = nullptr;
        SceneNodeList childrenNodes;
        
    };

}