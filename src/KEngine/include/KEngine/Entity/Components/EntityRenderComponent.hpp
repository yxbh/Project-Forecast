#pragma once

#include "KEngine/Interface/IEntityComponent.hpp"
#include "KEngine/Interface/ISceneNode.hpp"

#include <cassert>

namespace ke
{

    class EntityRenderComponent : public ke::IEntityComponent
    {
        KE_DEFINE_ENTITY_COMPONENT_COMMON_PROPERTIES(EntityRenderComponent, 0xC2CFA5BB)

    public:
        using IEntityComponent::IEntityComponent;

        inline void setSceneNode(ke::SceneNodeSptr p_sceneNode)
        {
            assert(p_sceneNode);
            this->sceneNode = p_sceneNode;
        }

        inline ke::SceneNodeSptr getSceneNode(ke::SceneNodeSptr p_sceneNode)
        {
            return this->sceneNode;
        }

    private:
        ke::SceneNodeSptr sceneNode;
    };

}