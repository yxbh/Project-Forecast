#include "GMSRoomManagementSystem.hpp"

#include "../Events/GMSRoomLoadRequestEvent.hpp"
#include "../AssetResources/TextureInfoResource.hpp"

#include <KEngine/Entity/Components/EntityRenderableComponents.hpp>
#include <KEngine/Events/OtherGraphicsEvents.hpp>
#include <KEngine/App.hpp>
#include <KEngine/Core/EventManager.hpp>
#include <KEngine/Log/Log.hpp>

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

        //static bool Loaded = false;

        //if (Loaded) return;
        //ke::Log::instance()->info("Loading GMS room...");

        //namespace fs =  std::experimental::filesystem::v1;

        //fs::path rootPath{ ProjectForecastExecAssetsRoot };
        //fs::recursive_directory_iterator recursive_dir_itr{ rootPath, fs::directory_options::follow_directory_symlink }, end;

        //for (; recursive_dir_itr != end; ++recursive_dir_itr)
        //{
        //    ke::Log::instance()->info("Found asset: " + recursive_dir_itr->path().string());
        //}

        //Loaded = true;
        //ke::Log::instance()->info("Loading GMS room... DONE");
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

        this->unloadCurrentEntities();
        if (this->currentRoomResource)
        {
            ke::Log::instance()->info("Unloading GM:S room: {} ...", this->currentRoomResource->getName());
            for (auto entity : this->currentRoomEntities)
            {
                auto entityManager = ke::App::instance()->getLogic()->getEntityManager();
                entityManager->removeEntity(entity->getId());
            }
            this->currentRoomEntities.clear();
            /*for (const auto & bgInfo : this->currentRoomResource->getBackgroundInfos())
            {
                ke::EventManager::enqueue(ke::makeEvent<ke::TextureUnloadRequestEvent>(bgInfo.bg, bgInfo.bg_hash));
            }*/
        }

        ke::Log::instance()->info("Loading GM:S room: {}", request->getRoomName());
        auto resourceManager = ke::App::instance()->getResourceManager();
        this->currentRoomResource = dynamic_cast<GMSRoomResource*>(resourceManager->getResource(request->getRoomName()));
        if (this->currentRoomResource != nullptr)
        {
            for (const auto & bgInfo : this->currentRoomResource->getBackgroundInfos())
            {
                if (bgInfo.bg.length() != 0)
                {
                    auto textureInfo = dynamic_cast<TextureInfoResource*>(resourceManager->getResource(bgInfo.bg));
                    if (textureInfo)
                    {
                        ke::EventManager::enqueue(ke::makeEvent<ke::TextureLoadViaFileRequestEvent>(
                            textureInfo->getName(), textureInfo->getTextureId(), textureInfo->getSourcePath()));
                    }
                }                
            }

            ke::EventManager::enqueue(ke::makeEvent<ke::SetClearColourRequestEvent>(this->currentRoomResource->getColour()));

            // Instantiate tile entities.
            std::unordered_set<ke::String> tileTextureNames;
            auto entityManager = ke::App::instance()->getLogic()->getEntityManager();
            for (const auto & tile : this->currentRoomResource->getTiles())
            {
                tileTextureNames.insert(tile.bg);
                auto textureId = tile.bg_hash;
                ke::Transform2D transform;
                transform.x      = static_cast<float>(tile.pos.x);
                transform.y      = static_cast<float>(tile.pos.y);
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

            // Load tile textures.
            for (const auto & tileTextureName : tileTextureNames)
            {
                auto textureInfo = dynamic_cast<TextureInfoResource*>(resourceManager->getResource(tileTextureName));
                if (textureInfo)
                {
                    ke::EventManager::enqueue(ke::makeEvent<ke::TextureLoadViaFileRequestEvent>(
                        textureInfo->getName(), textureInfo->getTextureId(), textureInfo->getSourcePath()));
                }
            }

            // Instantiate background entities.
            std::unordered_set<TextureInfoResource*> backgroundTextureInfos;
            for (const auto & bgInfo : this->currentRoomResource->getBackgroundInfos())
            {
                if (!bgInfo.enabled)
                {
                    continue;
                }

                auto textureId = bgInfo.bg_hash;
                ke::Transform2D transform;
                transform.x = static_cast<float>(bgInfo.pos.x);
                transform.y = static_cast<float>(bgInfo.pos.y);
                transform.scaleX = 1.0f;
                transform.scaleY = 1.0f;
                static const ke::graphics::DepthType depth = 0;

                auto textureInfo = dynamic_cast<TextureInfoResource*>(resourceManager->getResource(bgInfo.bg));
                if (textureInfo)
                {
                    backgroundTextureInfos.insert(textureInfo);

                    const auto & texWidth  = textureInfo->getTextureSize().width;
                    const auto & texHeight = textureInfo->getTextureSize().height;
                    assert(texWidth);
                    assert(texHeight);
                    ke::Rect2DInt32 textureRect;
                    textureRect.top    = 0;
                    textureRect.left   = 0;
                    textureRect.width  = texWidth;
                    textureRect.height = texHeight;

                    auto bgEntity = entityManager->newEntity().lock();
                    bgEntity->addComponent(ke::makeEntityComponent<ke::SpriteDrawableComponent>(
                        bgEntity, transform, depth, textureId, textureRect));
                    this->currentRoomEntities.push_back(bgEntity.get());

                    //
                    // Here we add additional background instances if tiling in any directions are enabled.
                    // We use the starting position of the first background position as an origin and tile additionals
                    // instances both ways from the origin.
                    //

                    if (bgInfo.tilex)
                    {
                        for (transform.x -= texWidth;
                            transform.x >= -this->currentRoomResource->getSize().width - texWidth;
                            transform.x -= texWidth)
                        {
                            bgEntity = entityManager->newEntity().lock();
                            bgEntity->addComponent(ke::makeEntityComponent<ke::SpriteDrawableComponent>(
                                bgEntity, transform, depth, textureId, textureRect));
                            this->currentRoomEntities.push_back(bgEntity.get());
                        }

                        for (transform.x = static_cast<float>(bgInfo.pos.x) + texWidth;
                            transform.x <= this->currentRoomResource->getSize().width + texWidth;
                            transform.x += texWidth)
                        {
                            bgEntity = entityManager->newEntity().lock();
                            bgEntity->addComponent(ke::makeEntityComponent<ke::SpriteDrawableComponent>(
                                bgEntity, transform, depth, textureId, textureRect));
                            this->currentRoomEntities.push_back(bgEntity.get());
                        }
                    }

                    if (bgInfo.tiley)
                    {
                        transform.x = static_cast<float>(bgInfo.pos.x); // reset x position value.

                        for (transform.y -= texHeight;
                            transform.y >= -this->currentRoomResource->getSize().height - texHeight;
                            transform.y -= texHeight)
                        {
                            bgEntity = entityManager->newEntity().lock();
                            bgEntity->addComponent(ke::makeEntityComponent<ke::SpriteDrawableComponent>(
                                bgEntity, transform, depth, textureId, textureRect));
                            this->currentRoomEntities.push_back(bgEntity.get());
                        }

                        for (transform.y = static_cast<float>(bgInfo.pos.y) + texHeight;
                            transform.y <= this->currentRoomResource->getSize().height + texHeight;
                            transform.y += texHeight)
                        {
                            bgEntity = entityManager->newEntity().lock();
                            bgEntity->addComponent(ke::makeEntityComponent<ke::SpriteDrawableComponent>(
                                bgEntity, transform, depth, textureId, textureRect));
                            this->currentRoomEntities.push_back(bgEntity.get());
                        }
                    }
                }
                
            }

            // Load background textures.
            for (const auto & textureInfo : backgroundTextureInfos)
            {
                ke::EventManager::enqueue(ke::makeEvent<ke::TextureLoadViaFileRequestEvent>(
                    textureInfo->getName(), textureInfo->getTextureId(), textureInfo->getSourcePath()));
            }
        }

        this->isLoadingRoom = false;
    }

    void GMSRoomManagementSystem::unloadCurrentEntities()
    {
        auto entityManager = ke::App::instance()->getLogic()->getEntityManager();
        for (auto entity : this->currentRoomEntities) entityManager->removeEntity(entity->getId());
        this->currentRoomEntities.clear();
    }

}