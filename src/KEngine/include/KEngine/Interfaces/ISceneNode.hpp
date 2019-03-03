#pragma once

#include "IEntity.hpp"

#include "KEngine/Common/Point2D.hpp"
#include "KEngine/Common/Transform2D.hpp"
#include "KEngine/Graphics/GraphicsCommand.hpp"

#include <cassert>
#include <algorithm>
#include <atomic>
#include <execution>
#include <memory>
#include <vector>

namespace ke
{
    using SceneNodeId = size_t;

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

        ISceneNode(ke::EntityId p_entityId) : id(newId()), entityId(p_entityId) {}
        ISceneNode(const ISceneNode&) = delete;
        ISceneNode & operator=(const ISceneNode&) = delete;

        virtual ke::GraphicsCommand getGraphicsCommand() const = 0;

        inline SceneNodeId getId() const { return this->id; }
        inline ke::EntityId getEntityId() const { return this->entityId; }
        inline ISceneNode * getParent() const { return this->parentNode; }
        inline const SceneNodeList & getChildren() const { return this->childrenNodes; }

        /// <summary>
        /// Update the scene node with the given time delta (i.e. time passed since last update).
        /// </summary>
        /// <param name="p_elapsedTime">Time passed since last update.</param>
        virtual void update(const ke::Time & p_elapsedTime) = 0;

        inline bool addChild(ke::SceneNodeSptr childNode)
        {
            this->childrenNodes.push_back(childNode);
            childNode->parentNode = this;

            return true;
        }

        inline void setLocalTransform(const ke::Transform2D & transform)
        {
            this->isGLobalTransformRecalculationRequired = true;
            this->localTransform = transform;
        }

        inline const ke::Transform2D & getLocalTransform() const { return this->localTransform; }

        inline ke::Transform2D & getLocalTransform()
        {
            this->isGLobalTransformRecalculationRequired = true;
            return this->localTransform;
        }

        inline const ke::Transform2D & getGlobalTransform(bool p_forceRecalculate = true) const
        {
            if (this->isGLobalTransformRecalculationRequired || p_forceRecalculate)
            {
                const ke::Transform2D & parentGlobalTransform
                    = this->getParent() ? this->getParent()->getGlobalTransform(false) : ke::Transform2D();
                this->globalTransform = this->localTransform + parentGlobalTransform;
                this->isGLobalTransformRecalculationRequired = false;
            }

            return this->globalTransform;
        }



        /// <summary>
        /// Find and remove the first child that contains a matching entity ID.
        /// </summary>
        /// <param name="entityId"></param>
        /// <returns>true if a matching scene node was found and erased.</returns>
        [[deprecated]]
        inline bool removeChildByEntityId(ke::EntityId p_entityId, bool p_recursive = true)
        {
            for (std::size_t i = 0; i < this->childrenNodes.size(); ++i)
            {
                auto sceneNode = this->childrenNodes[i];
                if (sceneNode->getEntityId() != ke::INVALID_ENTITY_ID &&
                    sceneNode->getEntityId() == p_entityId)
                {
                    this->childrenNodes[i] = this->childrenNodes.back();
                    this->childrenNodes.pop_back();
                    return true;
                }
            }

            if (p_recursive)
            {
                for (auto & childNode : this->childrenNodes)
                    if (childNode->removeChildByEntityId(p_entityId, p_recursive))
                        return true;
            }

            return false;
        }

        /// <summary>
        /// Find and remove any child node that contains a matching entity ID.
        /// </summary>
        /// <param name="entityId"></param>
        /// <returns>true if matching scene nodes were found and erased.</returns>
        [[deprecated]]
        inline bool removeChildrenByEntityId(ke::EntityId p_entityId, bool p_recursive = true)
        {
            auto removeItr = std::remove_if(std::begin(this->childrenNodes), std::end(this->childrenNodes), [p_entityId](auto node)
            {
                return node->getEntityId() != ke::INVALID_ENTITY_ID &&
                       node->getEntityId() == p_entityId;
            });
            bool foundAndRemoved = removeItr != std::end(this->childrenNodes);
            this->childrenNodes.erase(removeItr, std::end(this->childrenNodes));

            if (!foundAndRemoved && p_recursive)
            {
                for (auto childNode : this->childrenNodes)
                {
                    foundAndRemoved |= childNode->removeChildrenByEntityId(p_entityId, p_recursive);
                }
            }

            return foundAndRemoved;
        }

        /// <summary>
        /// Find and remove the given child node.
        /// This function will search recursively.
        /// </summary>
        /// <param name="p_node"></param>
        /// <returns>true if the matching scene node was found and erased.</returns>
        inline bool removeNode(const ke::ISceneNode * p_node)
        {
            assert(p_node != this);
            auto findItr = std::find_if(std::execution::par_unseq, std::begin(this->childrenNodes), std::end(this->childrenNodes), [&](auto & n)
            {
                return n.get() == p_node;
            });
            if (findItr != std::end(this->childrenNodes))
            {
                *findItr = this->childrenNodes.back();
                this->childrenNodes.pop_back();
                return true;
            }

            for (auto & childNode : this->childrenNodes)
                if (childNode->removeNode(p_node))
                    return true;

            return false;
        }

        static SceneNodeId newId()
        {
            static std::atomic<SceneNodeId> lastId = 0;
            return ++lastId;
        }

    protected:
        inline void setEntityId(ke::EntityId p_entityId) { this->entityId = p_entityId; }
        
        SceneNodeId id = 0;
        ke::EntityId entityId = ke::INVALID_ENTITY_ID;
        ke::Transform2D localTransform; // transform relative to its parent.
        mutable ke::Transform2D globalTransform;
        mutable bool isGLobalTransformRecalculationRequired = true;

        ISceneNode * parentNode = nullptr;
        SceneNodeList childrenNodes;
        
    };

    
    template<typename SceneNodeT, typename... ArgTs>
    inline std::shared_ptr<SceneNodeT> makeSceneNode(ArgTs && ... p_rrArgs)
    {
        static_assert(std::is_convertible<SceneNodeT*, ISceneNode*>::value, "Given type cannot convert to ke::ISceneNode.");
        static_assert(std::is_base_of<ISceneNode, SceneNodeT>::value, "Given type does not have ISceneNode as base class.");
        return std::make_shared<SceneNodeT>(std::forward<ArgTs>(p_rrArgs)...);
    }

}