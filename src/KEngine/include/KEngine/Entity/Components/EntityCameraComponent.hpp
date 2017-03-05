#pragma once

#include "KEngine/Interface/IEntityComponent.hpp"

#include "KEngine/Graphics/SceneNodes.hpp"

namespace ke
{

    class EntityCameraComponent : public ke::IEntityComponent
    {
        KE_DEFINE_ENTITY_COMPONENT_COMMON_PROPERTIES(EntityCameraComponent, 0x1A618AD2)

    public:
        using IEntityComponent::IEntityComponent;

        inline void setCameraNode(ke::CameraNode::Sptr newCameraNode)
        {
            this->cameraNode = newCameraNode;
        }

        inline ke::CameraNode::Sptr getCameraNode()
        {
            return this->cameraNode;
        }

    protected:
        ke::CameraNode::Sptr cameraNode;
    };

}
