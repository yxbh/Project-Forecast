#pragma once

#include "IEntity.hpp"

#include "KEngine/Common/Point2D.hpp"
#include "KEngine/Common/Transform2D.hpp"
#include "KEngine/Graphics/GraphicsCommand.hpp"

#include <atomic>
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
        inline bool removeChildByEntityId(ke::EntityId p_entityId)
        {
            auto it = this->childrenNodes.begin();
            while (it != this->childrenNodes.end())
            {
                auto sceneNode = it->get();
                if (sceneNode->getEntityId() != ke::INVALID_ENTITY_ID &&
                    sceneNode->getEntityId() == p_entityId)
                {
                    it = this->childrenNodes.erase(it);
                    return true;
                }
                else
                {
                    ++it;
                }
            }

            return false;
        }

        /// <summary>
        /// Find and remove any child node that contains a matching entity ID.
        /// </summary>
        /// <param name="entityId"></param>
        /// <returns>true if matching scene nodes were found and erased.</returns>
        inline bool removeChildrenByEntityId(ke::EntityId p_entityId, bool recursive = true)
        {
            bool foundAndRemoved = false;
            auto it = this->childrenNodes.begin();
            while (it != this->childrenNodes.end())
            {
                auto sceneNode = it->get();
                if (sceneNode->getEntityId() != ke::INVALID_ENTITY_ID &&
                    sceneNode->getEntityId() == p_entityId)
                {
                    it = this->childrenNodes.erase(it);
                    foundAndRemoved = true;
                    continue;
                }
                else if (recursive)
                {
                    for (auto childNode : sceneNode->childrenNodes)
                    {
                        foundAndRemoved &= childNode->removeChildrenByEntityId(p_entityId, recursive);
                    }
                }

                ++it;
            }

            return foundAndRemoved;
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