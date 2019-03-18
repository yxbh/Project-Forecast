#include "RorObjWaterfallBuilder.hpp"

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
#include <sstream>
#include <string>
#include <string_view>
#include <utility>


namespace pf
{
    ke::Entity * RorObjWaterfallBuilder::build(const std::any & p_parameters)
    {
        static constexpr char * const OBJECT_RESOURCE_NAME = "oWaterfall";

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

            //
            // Compute the waterfall width and height.
            //
            int waterfallWidth = objectResource->spriteResource->dimension.width;
            int waterfallHeight = objectResource->spriteResource->dimension.height;
            if (roomObjInfo.createcodeResource)
            {
                std::istringstream gmlCodeStream(roomObjInfo.createcodeResource->code);
                std::string temp;
                bool foundHeight = false, foundWidth = false;
                while (std::getline(gmlCodeStream, temp, '\n'))
                {
                    static const auto HEIGHT_STR = "self.height_b = ";
                    static const auto WIDTH_STR = "self.width_b = ";
                    auto & line = temp;
                    ke::trim(line);
                    if (std::string_view(line.c_str(), std::strlen(HEIGHT_STR)) == HEIGHT_STR)
                    {
                        waterfallHeight = std::stoi(std::string_view(line.c_str() + std::strlen(HEIGHT_STR)).data());
                        foundHeight = true;
                    }
                    else if (std::string_view(line.c_str(), std::strlen(WIDTH_STR)) == WIDTH_STR)
                    {
                        waterfallWidth = std::stoi(std::string_view(line.c_str() + std::strlen(WIDTH_STR)).data());
                        foundWidth = true;
                    }
                }
            }

            //
            // Build the waterfall scene tree.
            //

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

                static const auto frameDuration = ke::Time::seconds(1) / (60 * 0.2);
                static constexpr std::size_t startingFrame = 0;

                return ke::AnimatedSpriteNode::create(
                    entity->getId(), localTransform, std::move(frameOrigins), objectResource->depth,
                    std::move(frameTextureIds), std::move(frameRects), frameDuration, startingFrame, opacity);
            };

            const auto spriteResource = objectResource->spriteResource;
            assert(spriteResource);
            const auto spriteWaterfallMidResource = std::dynamic_pointer_cast<GMSSpriteResource>(resourceManager->getResource("sWaterfallMid"));
            assert(spriteWaterfallMidResource);
            const auto spriteWaterfallSplashResource = std::dynamic_pointer_cast<GMSSpriteResource>(resourceManager->getResource("sWaterfallSplash"));
            assert(spriteWaterfallSplashResource);
            auto makeWaterfallNode = [&](const ke::Point2DInt32 & posOffset, bool flipX = false) { return nodeMaker(spriteResource, posOffset, flipX); };
            auto makeWaterfallMidNode = [&](const ke::Point2DInt32 & posOffset) { return nodeMaker(spriteWaterfallMidResource, posOffset); };
            auto makeWaterfallSplashNode = [&](const ke::Point2DInt32 & posOffset) { return nodeMaker(spriteWaterfallSplashResource, posOffset); };

            // Set global position (location wihtin room) in root node.
            auto rootNode = ke::makeSceneNode<ke::InvisibleContainerNode>(entity->getId());
            rootNode->getLocalTransform().x = roomObjInfo.pos.x;
            rootNode->getLocalTransform().y = roomObjInfo.pos.y;
            rootNode->getLocalTransform().rotation = roomObjInfo.rotation;

            ke::SceneNodeSptr currentParentNode = rootNode;
            int currentHeight = 0;
            int heightDelta = 0;
            while (currentHeight <= waterfallHeight)
            {
                auto containerNode = ke::makeSceneNode<ke::InvisibleContainerNode>(entity->getId());
                containerNode->getLocalTransform().y = heightDelta;  // waterfall tile offset from the one above.
                currentParentNode->addChild(containerNode);
                currentParentNode = containerNode;

                // left side of waterfall.
                currentParentNode->addChild(makeWaterfallNode({0, 0}));

                // mid-section of waterfall.
                int currentWidth = 0;
                while (currentWidth < waterfallWidth)
                {
                    currentParentNode->addChild(makeWaterfallMidNode({ currentWidth + 10, 0 }));
                    if (currentHeight == waterfallHeight)
                    {
                        currentParentNode->addChild(makeWaterfallSplashNode({ currentWidth + 10, heightDelta }));
                    }
                    currentWidth += 10;
                }

                // right side of waterfall.
                currentParentNode->addChild(makeWaterfallNode({ currentWidth + 20, 0 }, true));

                // splash at the bottom of the waterfall.
                if (currentHeight == waterfallHeight)
                {
                    currentParentNode->addChild(makeWaterfallSplashNode({ 10 + currentWidth, heightDelta }));
                    currentParentNode->addChild(makeWaterfallSplashNode({ 20 + currentWidth, heightDelta }));
                    currentParentNode->addChild(makeWaterfallSplashNode({ 0, heightDelta }));
                    currentParentNode->addChild(makeWaterfallSplashNode({ -10 + currentWidth, heightDelta }));
                }

                heightDelta = -16;
                currentHeight += 16;
            }


            auto renderableComponent = ke::makeEntityComponent<ke::EntityRenderableComponent>(entity);
            renderableComponent->setSceneNode(rootNode);
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