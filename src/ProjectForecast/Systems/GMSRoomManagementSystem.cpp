#include "GMSRoomManagementSystem.hpp"

#include "../Events/GMSRoomLoadRequestEvent.hpp"
#include "../AssetResources/TextureInfoResource.hpp"
#include "../AssetResources/OtherGMSResources.hpp"

#include <KEngine/Graphics/SceneNodes.hpp>
#include <KEngine/Entity/Components/EntityRenderableComponents.hpp>
#include <KEngine/Events/OtherGraphicsEvents.hpp>
#include <KEngine/App.hpp>
#include <KEngine/Core/EventManager.hpp>
#include <KEngine/Log/Log.hpp>
#include <KEngine/Common/ScopeFunc.hpp>

#include <filesystem>
#include <unordered_set>

namespace
{
    static const auto ProjectForecastExecAssetsRoot = "D:/workspace/ProjectForecastExecAssetsRoot";
}

namespace pf
{

    bool GMSRoomManagementSystem::initialise()
    {
        ke::EventManager::registerListener<pf::GMSRoomLoadRequestEvent>(this, &GMSRoomManagementSystem::handleGMSRoomLoadRequest);
        return true;
    }

    void GMSRoomManagementSystem::shutdown()
    {
        this->unloadCurrentEntities();
        ke::EventManager::deregisterListener<pf::GMSRoomLoadRequestEvent>(this, &GMSRoomManagementSystem::handleGMSRoomLoadRequest);
    }

    void GMSRoomManagementSystem::update(ke::Time elapsedTime)
    {
        KE_UNUSED(elapsedTime);
        auto currentHumanView = ke::App::instance()->getLogic()->getCurrentHumanView();
        if (currentHumanView)
        {
            auto currentCameraNode = dynamic_cast<ke::CameraNode*>(currentHumanView->getScene()->getCameraNode());
            if (currentCameraNode)
            {
                if (this->currentLevelName == "Desolate Forest")
                {
                    this->updateRorLevelBg_DesolateForest(currentCameraNode);
                }
                else if (this->currentLevelName == "Dry Lake")
                {
                    this->updateRorLevelBg_DryLake(currentCameraNode);
                }
            }
        }
    }

    void GMSRoomManagementSystem::handleGMSRoomLoadRequest(ke::EventSptr event)
    {
        auto request = std::dynamic_pointer_cast<pf::GMSRoomLoadRequestEvent>(event);
        if (nullptr == request)
        {
            ke::Log::instance()->error("GMSRoomManagementSystem received unexpected event: {}", event->getName());
            return;
        }

        if (this->isLoadingRoom)
        {
            ke::Log::instance()->warn("GMSRoomManagementSystem is alreadying loading a room. Ignoring new load request.");
            return;
        }
        this->isLoadingRoom = true;
        KE_MAKE_SCOPEFUNC([this]() { isLoadingRoom = false; });

        this->unloadCurrentEntities();
        if (this->currentRoomResource)
        {
            ke::Log::instance()->info("Unloading GM:S room: {} ...", this->currentRoomResource->getName());
            auto entityManager = ke::App::instance()->getLogic()->getEntityManager();
            for (auto entity : this->currentRoomEntities)
            {
                entityManager->removeEntity(entity->getId());
            }
            this->currentRoomEntities.clear();
            this->currentRoomResource = nullptr;
            /*for (const auto & bgInfo : this->currentRoomResource->getBackgroundInfos())
            {
                ke::EventManager::enqueue(ke::makeEvent<ke::TextureUnloadRequestEvent>(bgInfo.bg, bgInfo.bg_hash));
            }*/
        }

        if (request->getRoomName().empty())
        {
            // We interpret an empty room name as a room unload only request.
            return;
        }

        auto resourceManager = ke::App::instance()->getResourceManager();
        this->currentRoomResource = std::dynamic_pointer_cast<GMSRoomResource>(resourceManager->getResource(request->getRoomName()));
        if (this->currentRoomResource != nullptr)
        {
            ke::Log::instance()->info("Loading GM:S room: {} ...", request->getRoomName());

            auto textureLoadRequestEvent = ke::makeEvent<ke::TexturesBulkLoadViaFilesRequestEvent>();
            const auto texpagesResource = std::dynamic_pointer_cast<pf::TexpageMapResource>(resourceManager->getResource("texpages"));
            assert(texpagesResource);

            for (const auto & bgInfo : this->currentRoomResource->getBackgroundInfos())
            {
                if (bgInfo.bg.length() != 0)
                {
                    auto textureInfo = std::dynamic_pointer_cast<TextureInfoResource>(resourceManager->getResource(bgInfo.bg));
                    if (textureInfo)
                    {
                        textureLoadRequestEvent->addTextureInfo(textureInfo->getName(), textureInfo->getTextureId(), textureInfo->getSourcePath());
                    }
                }                
            }

            ke::EventManager::enqueue(ke::makeEvent<ke::SetClearColourRequestEvent>(this->currentRoomResource->getColour()));

            // Instantiate tile entities.
            std::unordered_set<std::shared_ptr<TextureInfoResource>> textureInfos;
            auto entityManager = ke::App::instance()->getLogic()->getEntityManager();
            for (const auto & tile : this->currentRoomResource->getTiles())
            {
                auto bgResource = std::dynamic_pointer_cast<pf::GMSBgResource>(resourceManager->getResource(tile.bg));
                if (bgResource)
                {
                    const auto texpageResource = texpagesResource->texpages[bgResource->texture].get();
                    const auto textureInfo = std::dynamic_pointer_cast<pf::TextureInfoResource>(resourceManager->getResource("texture_" + std::to_string(texpageResource->sheetid)));
                    assert(textureInfo);
                    textureInfos.insert(textureInfo);
                    const auto textureId = textureInfo->getTextureId();
                    
                    ke::Transform2D transform;
                    transform.x      = static_cast<ke::Transform2D::PointType>(tile.pos.x);
                    transform.y      = static_cast<ke::Transform2D::PointType>(tile.pos.y);
                    transform.scaleX = tile.scale.x;
                    transform.scaleY = tile.scale.y;
                    ke::Rect2DInt32 textureRect;
                    textureRect.top    = tile.sourcepos.y;
                    textureRect.left   = tile.sourcepos.x;
                    textureRect.width  = tile.size.width;
                    textureRect.height = tile.size.height;

                    auto tileEntity    = entityManager->newEntity(tile.instanceid).lock();
                    tileEntity->addComponent(ke::makeEntityComponent<ke::SpriteDrawableComponent>(
                        tileEntity, transform, tile.tiledepth, textureId, textureRect, tile.colour));

                    this->currentRoomEntities.push_back(tileEntity.get());
                }
            }

            // Instantiate background entities.
            std::unordered_set<std::shared_ptr<TextureInfoResource>> backgroundTextureInfos;
            ke::graphics::DepthType depth = std::numeric_limits<ke::graphics::DepthType>::min();
            for (const auto & bgInfo : this->currentRoomResource->getBackgroundInfos())
            {
                if (!bgInfo.enabled) continue;

                auto bgResource = std::dynamic_pointer_cast<pf::GMSBgResource>(resourceManager->getResource(bgInfo.bg));
                if (bgResource)
                {
                    const auto texpageResource  = texpagesResource->texpages[bgResource->texture].get();
                    const auto textureInfo      = std::dynamic_pointer_cast<pf::TextureInfoResource>(resourceManager->getResource("texture_" + std::to_string(texpageResource->sheetid)));
                    const auto textureId        = textureInfo->getTextureId();

                    ke::Transform2D transform;
                    transform.x         = static_cast<ke::Transform2D::PointType>(bgInfo.pos.x);
                    transform.y         = static_cast<ke::Transform2D::PointType>(bgInfo.pos.y);
                    transform.scaleX    = 1.0f;
                    transform.scaleY    = 1.0f;

                    backgroundTextureInfos.insert(textureInfo);

                    const auto & texWidth  = textureInfo->getTextureSize().width;
                    const auto & texHeight = textureInfo->getTextureSize().height;
                    assert(texWidth);
                    assert(texHeight);
                    ke::Rect2DInt32 textureRect;
                    textureRect.top     = bgInfo.sourcepos.y;
                    textureRect.left    = bgInfo.sourcepos.x;
                    textureRect.width   = bgInfo.size.width;
                    textureRect.height  = bgInfo.size.height;
                    ++depth;

                    auto bgEntity = entityManager->newEntity().lock();
                    auto drawableComponent = ke::makeEntityComponent<ke::TiledSpriteDrawablwComponent>(
                        bgEntity, transform, depth, textureId, textureRect, ke::Color::WHITE, bgInfo.tilex, bgInfo.tiley);
                    bgEntity->addComponent(drawableComponent);

                    this->currentRoomEntities.push_back(bgEntity.get());
                    this->currentRoomBgEntities.push_back(bgEntity.get());
                }                    
            }

            // Load background textures.
            for (const auto & textureInfo : textureInfos)
            {
                textureLoadRequestEvent->addTextureInfo(textureInfo->getName(), textureInfo->getTextureId(), textureInfo->getSourcePath());
            }
            ke::EventManager::enqueue(textureLoadRequestEvent);

            // Instantiate room objects.
            auto entityFactory = ke::App::instance()->getLogic()->getEntityFactory();
            for (const auto & objInfo : this->currentRoomResource->getObjects())
            {
                // TODO: uncomment this when GM:S object loading is complete.
                //auto e = entityFactory->createNew(objInfo.obj, objInfo);
                //if (e)
                //{
                //    this->currentRoomEntities.push_back(e);
                //}
                //else
                //{
                //    ke::Log::instance()->error("Failed to create instance of: {}", objInfo.obj);
                //}
            }

            ke::Log::instance()->info("Loading GM:S room: {} ... DONE", request->getRoomName());
        }
        else
        {
            ke::Log::instance()->error("Cannot load GM:S room: {}. Room resource is missing.", request->getRoomName());
        }
    }

    void GMSRoomManagementSystem::unloadCurrentEntities()
    {
        auto entityManager = ke::App::instance()->getLogic()->getEntityManager();
        for (auto entity : this->currentRoomEntities) entityManager->removeEntity(entity->getId());
        this->currentRoomEntities.clear();
        this->currentRoomBgEntities.clear();
    }

    void GMSRoomManagementSystem::updateRorLevelBg_DesolateForest(ke::CameraNode * p_cameraNode)
    {
        const auto cameraViewDimension = p_cameraNode->getViewDimension();
        const auto cameraViewTransform = p_cameraNode->getGlobalTransform();
        ke::Point2DDouble cameraTopLeftPos;
        cameraTopLeftPos.x = cameraViewTransform.x - cameraViewDimension.width / 2;
        cameraTopLeftPos.y = cameraViewTransform.y + cameraViewDimension.height / 2;
        for (int i = 0; i < this->currentRoomBgEntities.size(); ++i)
        {
            auto entity = this->currentRoomBgEntities[i];
            auto bgComponent = entity->getComponent<ke::TiledSpriteDrawablwComponent>(ke::TiledSpriteDrawablwComponent::TYPE).lock();
            auto node = bgComponent->getSceneNode();
            switch (i)
            {
            case 0: // bStars
            {
                node->setLocalTransform(cameraViewTransform);
                break;
            }
            case 1: // bPlanets
            {
                ke::Transform2D newTransform;
                newTransform.x = cameraTopLeftPos.x + cameraViewDimension.width * 0.666666666666667;
                newTransform.y = cameraTopLeftPos.y - cameraViewDimension.height * 0.142857142857143;
                node->setLocalTransform(newTransform);
                break;
            }
            case 2: // bClouds1
            {
                ke::Transform2D newTransform;
                newTransform.x = cameraTopLeftPos.x / 1.1 + 140;
                newTransform.y = cameraTopLeftPos.y / 1.07 - 60;
                node->setLocalTransform(newTransform);
                break;
            }
            case 3: // bClouds2
            {
                ke::Transform2D newTransform;
                newTransform.x = cameraTopLeftPos.x / 1.2;
                newTransform.y = cameraTopLeftPos.y / 1.1 - 106;
                node->setLocalTransform(newTransform);
                break;
            }
            case 4: // bMountains
            {
                ke::Transform2D newTransform;
                newTransform.x = cameraTopLeftPos.x;
                newTransform.y = cameraTopLeftPos.y - cameraViewDimension.height + 153;
                node->setLocalTransform(newTransform);
                break;
            }
            }
        }
    }

    void GMSRoomManagementSystem::updateRorLevelBg_DryLake(ke::CameraNode * p_cameraNode)
    {
        return this->updateRorLevelBg_DesolateForest(p_cameraNode);
    }

}