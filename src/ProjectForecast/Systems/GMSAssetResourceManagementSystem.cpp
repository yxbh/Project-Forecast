#include "GMSAssetResourceManagementSystem.hpp"

#include "../AssetResources/TextureInfoResource.hpp"
#include "../AssetResources/GMSRoomResource.hpp"
#include "../AssetResources/GMSObjectResource.hpp"
#include "../AssetResources/OtherGMSResources.hpp"

#include "../CommandLineOptions.hpp"

#include <KEngine/Events/OtherGraphicsEvents.hpp>
#include <KEngine/App.hpp>
#include <KEngine/Utility/FileSystemHelper.hpp>
#include <KEngine/Log/Log.hpp>

#include <SFML/Graphics/Image.hpp>

#include <algorithm>
#include <execution>
#include <filesystem>
#include <fstream>
#include <limits>
#include <mutex>
#include <utility>

namespace fs = std::filesystem;

namespace
{
    ke::Colour gmsColourStrToColour(const ke::String & p_colourStr)
    {
        assert(p_colourStr.length() == 9);
        assert(p_colourStr[0] == '#');
        ke::Colour roomColour = {
            // assume colour is in hex RGBA starting with the '#' symbol.
            static_cast<uint8_t>(std::stol(p_colourStr.substr(1, 2), nullptr, 16)),
            static_cast<uint8_t>(std::stol(p_colourStr.substr(3, 2), nullptr, 16)),
            static_cast<uint8_t>(std::stol(p_colourStr.substr(5, 2), nullptr, 16)),
            static_cast<uint8_t>(std::stol(p_colourStr.substr(7, 2), nullptr, 16)) };
        return roomColour;
    }
}

namespace pf
{

    bool GMSAssetResourceManagementSystem::initialise()
    {
        ke::Log::instance()->info("Scanning assets...");
        this->loadTexpageAssets();
        this->loadTextureAssets();
        this->loadBgAssets();
        this->loadCodeAssets();
        this->loadSpriteAssets();
        this->loadObjectAssets();
        this->loadRoomAssets();
        ke::Log::instance()->info("Scanning assets... DONE");
        return true;
    }

    void GMSAssetResourceManagementSystem::shutdown()
    {
    }

    void GMSAssetResourceManagementSystem::update(ke::Time elapsedTime)
    {
        KE_UNUSED(elapsedTime);
    }

    void GMSAssetResourceManagementSystem::loadTexpageAssets(void)
    {
        ke::Log::instance()->info("Scanning GM:S texpage assets...");
        const auto assetDirPath         = ke::App::getCommandLineArgValue(pf::cli::ExecAssetsPath).as<ke::String>();
        const auto texpageRootDirPath   = fs::path{ assetDirPath } / "texpage";
        if (!fs::exists(texpageRootDirPath))
        {
            ke::Log::instance()->error("Cannot find asset path: {}", texpageRootDirPath.string());
            return;
        }
        auto texpageMapResource         = std::make_shared<pf::TexpageMapResource>("texpages", __FILE__);
        auto & texpages                 = texpageMapResource->texpages; // <texpage_id, texpage>
        const auto texpagePaths         = ke::FileSystemHelper::getChildPaths(texpageRootDirPath);
        std::mutex texpagesMutex;
        std::for_each(std::execution::par_unseq, std::begin(texpagePaths), std::end(texpagePaths), [&](const auto & path)
        {
            if (!path.has_extension() || path.extension() != ".json") return;

            ke::Log::instance()->debug("Loading GM:S texpage asset: {}", path.string());

            std::ifstream texpageFileStream{ path };
            ke::json texpageJson;
            texpageFileStream >> texpageJson;

            auto texpage = std::make_shared<pf::GMSTexpageResource>("texpage_" + path.stem().string(), path.string());

            texpage->id = std::stoi(path.stem().string());
            texpage->sourcePosition.x            = texpageJson["src"]["x"].get<int>();
            texpage->sourcePosition.y            = texpageJson["src"]["y"].get<int>();
            texpage->sourceDimension.width       = texpageJson["src"]["width"].get<unsigned>();
            texpage->sourceDimension.height      = texpageJson["src"]["height"].get<unsigned>();
            texpage->destinationPosition.x       = texpageJson["dest"]["x"].get<int>();
            texpage->destinationPosition.y       = texpageJson["dest"]["y"].get<int>();
            texpage->destinationDimension.width  = texpageJson["dest"]["width"].get<unsigned>();
            texpage->destinationDimension.height = texpageJson["dest"]["height"].get<unsigned>();
            texpage->dimension.width             = texpageJson["size"]["width"].get<unsigned>();
            texpage->dimension.height            = texpageJson["size"]["height"].get<unsigned>();
            texpage->sheetid                     = texpageJson["sheetid"].get<unsigned>();

            ke::App::instance()->getResourceManager()->registerResource(texpage);

            std::scoped_lock lock(texpagesMutex);
            texpages[texpage->id] = texpage;
        });
        ke::App::instance()->getResourceManager()->registerResource(texpageMapResource);
    }

    void GMSAssetResourceManagementSystem::loadTextureAssets(void)
    {
        ke::Log::instance()->info("Scanning texture assets...");
        const auto assetDirPath = ke::App::getCommandLineArgValue(pf::cli::ExecAssetsPath).as<ke::String>();
        const auto texturesRootDirPath = fs::path{ assetDirPath } / "texture";
        if (!fs::exists(texturesRootDirPath))
        {
            ke::Log::instance()->error("Cannot find asset path: {}", texturesRootDirPath.string());
            return;
        }
        sf::Image tempImage;
        static constexpr auto RSRC_PREFIX = "texture_";
        for (const auto & path : ke::FileSystemHelper::getChildPaths(texturesRootDirPath))
        {
            if (fs::is_directory(path)) // is a texture directory.
            {
                auto textureFilePaths = ke::FileSystemHelper::getFilePaths(path);
                if (textureFilePaths.size() == 1)
                {
                    // This loads the unpacked textures.

                    //const auto & texPath = textureFilePaths[0];
                    //ke::Log::instance()->debug("Loading texture asset: {}", texPath.string());

                    //auto textureResource = std::make_shared<TextureInfoResource>();
                    //textureResource->setName(RSRC_PREFIX + texPath.stem().string());
                    //textureResource->setTextureId(hasher(textureResource->getName()));
                    //textureResource->setSourcePath(texPath.string());

                    //// retrieve size
                    //bool ret = tempImage.loadFromFile(texPath.string());
                    //assert(ret);
                    //TextureInfoResource::DimensionType dimension;
                    //dimension.width = tempImage.getSize().x;
                    //dimension.height = tempImage.getSize().y;
                    //textureResource->setTextureSize(dimension);

                    //ke::App::instance()->getResourceManager()->registerResource(textureResource);
                }
                else
                {
                    // ignore when there're multiple texture files in a single dir for now.
                }
            }
            else if (fs::is_regular_file(path) && path.extension() == ".png") // is a png texture.
            {
                // This loads the GM:S packed texture sheets.

                ke::Log::instance()->debug("Loading texture asset: {}", path.string());

                auto textureResource = std::make_shared<TextureInfoResource>();
                textureResource->setName(RSRC_PREFIX + path.stem().string());
                textureResource->setTextureId(std::stoi(path.stem().string()));
                textureResource->setSourcePath(path.string());

                // retrieve size
                bool ret = tempImage.loadFromFile(path.string());
                assert(ret);
                TextureInfoResource::DimensionType dimension;
                dimension.width  = tempImage.getSize().x;
                dimension.height = tempImage.getSize().y;
                textureResource->setTextureSize(dimension);

                ke::App::instance()->getResourceManager()->registerResource(textureResource);
            }
        }
    }

    void GMSAssetResourceManagementSystem::loadBgAssets(void)
    {
        ke::Log::instance()->info("Scanning GM:S bg assets...");
        const auto assetDirPath     = ke::App::getCommandLineArgValue(pf::cli::ExecAssetsPath).as<ke::String>();
        const auto bgRootDirPath    = fs::path{ assetDirPath } / "bg";
        if (!fs::exists(bgRootDirPath))
        {
            ke::Log::instance()->error("Cannot find asset path: {}", bgRootDirPath.string());
            return;
        }
        const auto bgPaths          = ke::FileSystemHelper::getChildPaths(bgRootDirPath);
        auto resourceManager        = ke::App::instance()->getResourceManager();
        const auto texpageMapResource = std::dynamic_pointer_cast<pf::TexpageMapResource>(resourceManager->getResource("texpages"));
        assert(texpageMapResource);
        std::for_each(std::execution::par_unseq, std::cbegin(bgPaths), std::cend(bgPaths), [&](const auto & path)
        {
            if (!path.has_extension() || path.extension() != ".json") return;

            ke::Log::instance()->debug("Loading GM:S bg asset: {}", path.string());
            std::ifstream bgFileStream{ path };
            ke::json bgJson;
            bgFileStream >> bgJson;

            auto bgResource = std::make_shared<pf::GMSBgResource>(path.stem().string(), path.string());
            bgResource->texture = bgJson["texture"].get<unsigned>();

            // find and attach the associated texpage resource
            bgResource->texpageResource = texpageMapResource->texpages[bgResource->texture];

            resourceManager->registerResource(bgResource);
        });
    }

    void GMSAssetResourceManagementSystem::loadCodeAssets(void)
    {
        ke::Log::instance()->info("Scanning GM:S bg assets...");
        const auto assetDirPath = ke::App::getCommandLineArgValue(pf::cli::ExecAssetsPath).as<ke::String>();
        const auto codeRootDirPath = fs::path{ assetDirPath } / "code";
        if (!fs::exists(codeRootDirPath))
        {
            ke::Log::instance()->error("Cannot find asset path: {}", codeRootDirPath.string());
            return;
        }
        const auto codePaths = ke::FileSystemHelper::getChildPaths(codeRootDirPath);
        auto resourceManager = ke::App::instance()->getResourceManager();
        std::for_each(std::execution::par_unseq, std::cbegin(codePaths), std::cend(codePaths), [&](const auto & path)
        {
            if (!path.has_extension() || path.extension() != ".lsp") return;

            ke::Log::instance()->debug("Loading GM:S code asset: {}", path.string());
            std::ifstream codeFileStream{ path };
            ke::String codeContent;

            codeFileStream.seekg(0, std::ios::end);
            codeContent.reserve(codeFileStream.tellg());
            codeFileStream.seekg(0, std::ios::beg);

            codeContent.assign((std::istreambuf_iterator<char>(codeFileStream)), std::istreambuf_iterator<char>());

            auto codeResource = std::make_shared<pf::GMSCodeResource>(path.stem().string(), path.string());
            codeResource->code = std::move(codeContent);

            resourceManager->registerResource(codeResource);
        });
    }

    void GMSAssetResourceManagementSystem::loadSpriteAssets(void)
    {
        ke::Log::instance()->info("Scanning GM:S sprite assets...");
        const auto assetDirPath         = ke::App::getCommandLineArgValue(pf::cli::ExecAssetsPath).as<ke::String>();
        const auto spriteRootDirPath    = fs::path{ assetDirPath } / "sprite";
        if (!fs::exists(spriteRootDirPath))
        {
            ke::Log::instance()->error("Cannot find asset path: {}", spriteRootDirPath.string());
            return;
        }
        const auto spritePaths          = ke::FileSystemHelper::getChildPaths(spriteRootDirPath);
        auto resourceManager            = ke::App::instance()->getResourceManager();
        std::for_each(std::execution::par_unseq, std::begin(spritePaths), std::end(spritePaths), [&](const auto & path)
        {
            if (!path.has_extension() || path.extension() != ".json") return;

            ke::Log::instance()->debug("Loading GM:S sprite asset: {}", path.string());
            std::ifstream spriteFileStream{ path };
            ke::json spriteJson;
            spriteFileStream >> spriteJson;

            auto spriteResource = std::make_shared<pf::GMSSpriteResource>(path.stem().string(), path.string());
            assert(spriteResource);

            spriteResource->dimension.width     = spriteJson["size"]["width"].get<unsigned>();
            spriteResource->dimension.height    = spriteJson["size"]["height"].get<unsigned>();
            spriteResource->boundingBox.top     = spriteJson["bounding"]["top"].get<unsigned>();
            spriteResource->boundingBox.left    = spriteJson["bounding"]["left"].get<unsigned>();
            spriteResource->boundingBox.bottom  = spriteJson["bounding"]["bottom"].get<unsigned>();
            spriteResource->boundingBox.right   = spriteJson["bounding"]["right"].get<unsigned>();
            spriteResource->boundingBoxMode     = spriteJson["bboxmode"].get<unsigned>();
            spriteResource->separateMasks       = spriteJson["sepmasks"].get<bool>();
            spriteResource->origin.x            = spriteJson["origin"]["x"].get<int>();
            spriteResource->origin.y            = spriteJson["origin"]["y"].get<int>();
            for (const auto & textureJson : spriteJson["textures"])
            {
                const auto texpageId = textureJson.get<unsigned>();
                spriteResource->texpageIds.push_back(texpageId);
                auto texpageResource = std::static_pointer_cast<pf::GMSTexpageResource>(resourceManager->getResource("texpage_" + std::to_string(texpageId)));
                assert(texpageResource);
                spriteResource->texpageResources.push_back(texpageResource);
            }

            resourceManager->registerResource(spriteResource);
        });
    }

    void GMSAssetResourceManagementSystem::loadRoomAssets(void)
    {
        ke::Log::instance()->info("Scanning GM:S room assets...");
        const auto assetDirPath         = ke::App::getCommandLineArgValue(pf::cli::ExecAssetsPath).as<ke::String>();
        const auto gmsRoomsRootDirPath  = fs::path{ assetDirPath } / "room";
        if (!fs::exists(gmsRoomsRootDirPath))
        {
            ke::Log::instance()->error("Cannot find asset path: {}", gmsRoomsRootDirPath.string());
            return;
        }
        const auto gmsRoomPaths         = ke::FileSystemHelper::getFilePaths(gmsRoomsRootDirPath);
        std::for_each(std::execution::par_unseq, std::begin(gmsRoomPaths), std::end(gmsRoomPaths), [&](const auto & gmsRoomPath)
        {
            if (!gmsRoomPath.has_extension() || gmsRoomPath.extension() != ".json") return;

            ke::Log::instance()->debug("Loading GM:S room asset: {}", gmsRoomPath.string());

            auto roomResource = std::make_shared<GMSRoomResource>();
            roomResource->setName(gmsRoomPath.stem().string());
            roomResource->setSourcePath(gmsRoomPath.string());

            std::ifstream roomFileStream{ gmsRoomPath };
            ke::json roomJson;
            roomFileStream >> roomJson;

            //
            // Load general room info.
            //
            GMSRoomResource::SizeType roomSize;
            roomSize.width  = roomJson["size"]["width"].get<unsigned>();
            roomSize.height = roomJson["size"]["height"].get<unsigned>();
            roomResource->setSize(roomSize);
            roomResource->setSpeed(roomJson["speed"].get<int>());
            auto roomColourStr = roomJson["colour"].get<ke::String>();
            roomResource->setColour(::gmsColourStrToColour(roomColourStr));

            auto resourceManager = ke::App::instance()->getResourceManager();
            const auto texpagesResource = std::dynamic_pointer_cast<pf::TexpageMapResource>(resourceManager->getResource("texpages"));
            assert(texpagesResource);

            //
            // load background info
            //
            const auto & roomBackgroundsJson = roomJson["bgs"];
            for (const auto & backgroundJson : roomBackgroundsJson)
            {
                // Here we make sure to convert the GM:S room coordinates to KEngine's world coordinates.
                // I.e. y-down to y-up.
                GMSRoomBackgroundInfo backgroundInfo;
                backgroundInfo.enabled    = backgroundJson["enabled"].get<bool>();
                backgroundInfo.foreground = backgroundJson["foreground"].get<bool>();
                backgroundInfo.pos        = { backgroundJson["pos"]["x"].get<int>(), -backgroundJson["pos"]["y"].get<int>() };
                backgroundInfo.tilex      = backgroundJson["tilex"].get<bool>();
                backgroundInfo.tiley      = backgroundJson["tiley"].get<bool>();
                backgroundInfo.speed      = { backgroundJson["speed"]["x"].get<int>(), -backgroundJson["speed"]["y"].get<int>() };
                backgroundInfo.stretch    = backgroundJson["stretch"].get<bool>();
                backgroundInfo.bg         = backgroundJson.value("bg", "");

                // Get the matching bg resource and adjust for texture sheet offset.
                if (!backgroundInfo.bg.empty())
                {
                    auto resource = std::dynamic_pointer_cast<pf::GMSBgResource>(resourceManager->getResource(backgroundInfo.bg));
                    assert(resource);
                    backgroundInfo.sourcepos = resource->texpageResource.lock()->sourcePosition;
                    backgroundInfo.size      = resource->texpageResource.lock()->sourceDimension;
                }

                roomResource->addBackgroundInfo(backgroundInfo);
            }

            //
            // load tile instances
            //
            const auto & roomTilesJson = roomJson["tiles"];
            roomResource->tiles.reserve(roomTilesJson.size());
            for (const auto & tileJson : roomTilesJson)
            {
                pf::GMSRoomTileInstance newTile;
                newTile.instanceid = tileJson["instanceid"].get<unsigned>();

                // Here we make sure to convert the GM:S room coordinates to KEngine's world coordinates.
                // I.e. y-down to y-up.
                // Texture coordinates are the same at the moment at y-down. I.e. (0,0) at top left.
                newTile.pos       = { tileJson["pos"]["x"].get<int>(), -tileJson["pos"]["y"].get<int>() };
                newTile.bg        = tileJson["bg"].get<ke::String>();
                newTile.sourcepos = { tileJson["sourcepos"]["x"].get<int>(), tileJson["sourcepos"]["y"].get<int>() }; // sourcepos is y-down local image coordinates.
                newTile.size      = { tileJson["size"]["width"].get<int>(), tileJson["size"]["height"].get<int>() };
                newTile.scale     = { tileJson["scale"]["x"].get<float>(), tileJson["scale"]["y"].get<float>() };
                newTile.colour    = ::gmsColourStrToColour(tileJson["colour"].get<ke::String>());

                // Here convert GM:S' depth system to KEngine's depth system.
                // GM:S depth value: larger == further back.
                // KEngine depth value: larger == further in front.
                newTile.tiledepth = -tileJson["tiledepth"].get<ke::graphics::DepthType>();

                // Get the matching bg resource and adjust for texture sheet offset.
                auto resource = std::dynamic_pointer_cast<pf::GMSBgResource>(resourceManager->getResource(newTile.bg));
                assert(resource);
                newTile.sourcepos += resource->texpageResource.lock()->sourcePosition;

                roomResource->addTile(newTile);
            }

            //
            // load object instances
            //
            const auto & roomObjsJson = roomJson["objs"];
            roomResource->objects.reserve(roomObjsJson.size());
            for (const auto & objJson : roomObjsJson)
            {
                // Here we make sure to convert the GM:S room coordinates to KEngine's world coordinates.
                // I.e. y-down to y-up.
                pf::GMSRoomObjectInstance obj;
                obj.instanceid   = objJson["instanceid"].get<ke::EntityId>();
                obj.createcodeid = objJson["createcodeid"].get<unsigned>();
                obj.obj          = objJson["obj"].get<ke::String>();
                obj.pos          = { objJson["pos"]["x"].get<int>(), -objJson["pos"]["y"].get<int>() };
                obj.scale        = { objJson["scale"]["x"].get<float>(), objJson["scale"]["y"].get<float>() };
                obj.rotation     = objJson["rotation"].get<float>();
                obj.colour       = ::gmsColourStrToColour(objJson["colour"].get<ke::String>());

                // Attach the code resource if there is one.
                if (obj.createcodeid != 4294967295) // magic number from Altar GMS room json files.
                {
                    ke::String codeResourceName = "gml_RoomCC_" + roomResource->getName() + "_" + std::to_string(obj.createcodeid) + "_Create.gml";
                    obj.createcodeResource = std::dynamic_pointer_cast<GMSCodeResource>(resourceManager->getResource(codeResourceName));
                    assert(obj.createcodeResource);
                }

                roomResource->addObject(obj);
            }

            ke::App::instance()->getResourceManager()->registerResource(roomResource);
        });
    }

    void GMSAssetResourceManagementSystem::loadObjectAssets(void)
    {
        ke::Log::instance()->info("Scanning GM:S object assets...");
        const auto assetDirPath         = ke::App::getCommandLineArgValue(pf::cli::ExecAssetsPath).as<ke::String>();
        const auto gmsObjectRootDirPath = fs::path{ assetDirPath } / "object";
        if (!fs::exists(gmsObjectRootDirPath))
        {
            ke::Log::instance()->error("Cannot find asset path: {}", gmsObjectRootDirPath.string());
            return;
        }
        const auto gmsObjectPaths       = ke::FileSystemHelper::getFilePaths(gmsObjectRootDirPath);
        auto resourceManager            = ke::App::instance()->getResourceManager();
        std::for_each(std::execution::par_unseq, std::begin(gmsObjectPaths), std::end(gmsObjectPaths), [&](const auto & gmsObjectPath)
        {
            if (!gmsObjectPath.has_extension() || gmsObjectPath.extension() != ".json") return;

            ke::Log::instance()->debug("Loading GM:S object asset: {}", gmsObjectPath.string());

            std::ifstream objectFileStream{ gmsObjectPath };
            ke::json objectJson;
            objectFileStream >> objectJson;

            auto objectResource = std::make_shared<pf::GMSObjectResource>(fs::path(gmsObjectPath).stem().string(), gmsObjectPath.string());
            objectResource->sprite   = objectJson["sprite"].get<ke::String>();  // May be empty.
            objectResource->visible  = objectJson["visible"].get<bool>();
            objectResource->solid    = objectJson["solid"].get<bool>();
            objectResource->depth    = objectJson["depth"].get<ke::graphics::DepthType>();
            objectResource->persist  = objectJson["persist"].get<bool>();
            objectResource->sensor   = objectJson["sensor"].get<bool>();
            objectResource->colshape = objectJson["colshape"].get<ke::String>();

            // Here convert GM:S' depth system to KEngine's depth system.
            // GM:S depth value: larger == further back.
            // KEngine depth value: larger == further in front.
            objectResource->depth = -objectResource->depth;

            if (!objectResource->sprite.empty())
            {
                auto spriteResource = std::dynamic_pointer_cast<GMSSpriteResource>(resourceManager->getResource(objectResource->sprite));
                assert(spriteResource);
                objectResource->spriteResource = spriteResource;
            }

            resourceManager->registerResource(objectResource);
        });
    }
}