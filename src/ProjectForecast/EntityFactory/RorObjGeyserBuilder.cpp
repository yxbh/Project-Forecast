#include "RorObjGeyserBuilder.hpp"

#include "../AssetResources/GMSRoomResource.hpp"
#include "../AssetResources/GMSObjectResource.hpp"
#include "../AssetResources/OtherGMSResources.hpp"
#include "../Utility/GraphicsHelper.hpp"

#include "KEngine/App.hpp"
#include "KEngine/Entity/Components/EntityRenderableComponents.hpp"
#include "KEngine/Graphics/SceneNodes.hpp"
#include "KEngine/Utility/StringHelper.hpp"
#include "KEngine/Log/Log.hpp"

#include <cassert>
#include <cstdlib>
#include <sstream>
#include <string>
#include <string_view>
#include <utility>


namespace pf
{
    ke::Entity * RorObjGeyserBuilder::build(const std::any & p_parameters)
    {
        static constexpr auto OBJECT_RESOURCE_NAME = "oGeyser";

        const auto & roomObjInfo = std::any_cast<const pf::GMSRoomObjectInstance&>(p_parameters);

        auto resourceManager = ke::App::instance()->getResourceManager();
        const auto objectResource = std::static_pointer_cast<pf::GMSObjectResource>(resourceManager->getResource(OBJECT_RESOURCE_NAME));
        assert(objectResource);
        if (objectResource)
        {
            auto entityManager = ke::App::instance()->getLogic()->getEntityManager();
            auto entity = entityManager->newEntity(roomObjInfo.instanceid).lock();
            assert(entity);
            entity->setName(OBJECT_RESOURCE_NAME);

            auto nodeMaker = [&](const auto & spriteResource, const ke::Point2DInt32 & pos, bool flipX = false)
            {
                auto frameOrigins    = pf::GraphicsHelper::computeSpriteOrigins(spriteResource.get());
                auto frameTextureIds = pf::GraphicsHelper::computeSpriteTextureIds(spriteResource.get());
                auto frameRects      = pf::GraphicsHelper::computeSpriteFrameRects(spriteResource.get());
                ke::Transform2D localTransform;
                localTransform.x        = pos.x;
                localTransform.y        = pos.y;
                localTransform.scaleX   = flipX ? -roomObjInfo.scale.x : roomObjInfo.scale.x;
                localTransform.scaleY   = roomObjInfo.scale.y;

                auto opacity = roomObjInfo.colour;
                opacity.a = static_cast<decltype(opacity.a)>(opacity.a * 0.6);

                const auto frameDuration = ke::Time::seconds(1) / (60 * 0.3);
                const std::size_t startingFrame = static_cast<unsigned>(std::rand()) % frameRects.size();

                return ke::AnimatedSpriteNode::create(
                    entity->getId(), localTransform, std::move(frameOrigins), objectResource->depth,
                    std::move(frameTextureIds), std::move(frameRects), frameDuration, startingFrame, opacity);
            };

            const auto spriteResource = objectResource->spriteResource;
            assert(spriteResource);
            auto makeGeyserNode = [&](const ke::Point2DInt32 & posOffset, bool flipX = false) { return nodeMaker(spriteResource, posOffset, flipX); };

            // Set global position (location wihtin room) in root node.
            auto geyserNode = makeGeyserNode(roomObjInfo.pos);
            geyserNode->getLocalTransform().rotation = roomObjInfo.rotation;

            auto renderableComponent = ke::makeEntityComponent<ke::EntityRenderableComponent>(entity);
            renderableComponent->setSceneNode(geyserNode);
            entity->addComponent(renderableComponent);
            
            return entity.get();
        }
        else
        {
            ke::Log::instance()->error("Cannot instantiate entity. Missing resource: {}", OBJECT_RESOURCE_NAME);
        }
        return nullptr;
    }
}