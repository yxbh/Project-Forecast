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
        this->loadTextureAssets();
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

    void GMSAssetResourceManagementSystem::loadTextureAssets(void)
    {
        ke::Log::instance()->info("Scanning texture assets...");
        const auto assetDirPath = ke::App::getCommandLineArgValue(pf::cli::ExecAssetsPath).as<ke::String>();
        const auto texturesRootDirPath = fs::path{ assetDirPath } / "textures";
        sf::Image tempImage;
        std::hash<ke::String> hasher;
        for (const auto & path : ke::FileSystemHelper::getChildPaths(texturesRootDirPath))
        {
            if (fs::is_directory(path)) // is a texture directory.
            {
                auto textureFilePaths = ke::FileSystemHelper::getFilePaths(path);
                if (textureFilePaths.size() == 1)
                {
                    const auto & texPath = textureFilePaths[0];
                    ke::Log::instance()->info("Loading texture asset: {}", texPath.string());

                    auto textureResource = std::make_shared<TextureInfoResource>();
                    textureResource->setName(texPath.stem().string());
                    textureResource->setTextureId(hasher(textureResource->getName()));
                    textureResource->setSourcePath(texPath.string());

                    // retrieve size
                    bool ret = tempImage.loadFromFile(texPath.string());
                    assert(ret);
                    TextureInfoResource::DimensionType dimension;
                    dimension.width = tempImage.getSize().x;
                    dimension.height = tempImage.getSize().y;
                    textureResource->setTextureSize(dimension);

                    ke::App::instance()->getResourceManager()->registerResource(textureResource);
                }
                else
                {
                    // ignore when there're multiple texture files in a single dir for now.
                }
            }
            else if (fs::is_regular_file(path) && path.extension() == "png") // is a png texture.
            {
                ke::Log::instance()->info("Loading texture asset: {}", path.string());

                auto textureResource = std::make_shared<TextureInfoResource>();
                textureResource->setName("texture_" + path.stem().string());
                textureResource->setTextureId(std::stoi(textureResource->getName()));
                textureResource->setSourcePath(path.string());

                // retrieve size
                bool ret = tempImage.loadFromFile(path.string());
                assert(ret);
                TextureInfoResource::DimensionType dimension;
                dimension.width = tempImage.getSize().x;
                dimension.height = tempImage.getSize().y;
                textureResource->setTextureSize(dimension);

                ke::App::instance()->getResourceManager()->registerResource(textureResource);
            }
        }
    }

    void GMSAssetResourceManagementSystem::loadSpriteAssets(void)
    {
        ke::Log::instance()->info("Scanning texpage assets...");
        const auto assetDirPath = ke::App::getCommandLineArgValue(pf::cli::ExecAssetsPath).as<ke::String>();
        const auto texpageRootDirPath = fs::path{ assetDirPath } / "texpage";
        std::mutex texpagesMutex;
        std::unordered_map<unsigned, std::shared_ptr<pf::GMSTexpageResource>> texpages; // <texpage_id, texpage>
        const auto texpagePaths = ke::FileSystemHelper::getChildPaths(texpageRootDirPath);
        std::for_each(std::execution::par_unseq, std::begin(texpagePaths), std::end(texpagePaths), [&](const auto & path)
        {
            if (!path.has_extension() || path.extension() != ".json") return;

            ke::Log::instance()->info("Loading GM:S texpage asset: {}", path.string());

            std::ifstream texpageFileStream{ path };
            ke::json texpageJson;
            texpageFileStream >> texpageJson;

            auto texpage = std::make_shared<pf::GMSTexpageResource>("texpage_" + path.stem().string(), path.string());

            texpage->id = std::stoi(path.stem().string());
            texpage->sourcePosition.x            = texpageJson["src"]["x"].get<unsigned>();
            texpage->sourcePosition.y            = texpageJson["src"]["y"].get<unsigned>();
            texpage->sourceDimension.width       = texpageJson["src"]["width"].get<unsigned>();
            texpage->sourceDimension.height      = texpageJson["src"]["height"].get<unsigned>();
            texpage->destinationPosition.x       = texpageJson["dest"]["x"].get<unsigned>();
            texpage->destinationPosition.y       = texpageJson["dest"]["y"].get<unsigned>();
            texpage->destinationDimension.width  = texpageJson["dest"]["width"].get<unsigned>();
            texpage->destinationDimension.height = texpageJson["dest"]["height"].get<unsigned>();
            texpage->dimension.width             = texpageJson["size"]["width"].get<unsigned>();
            texpage->dimension.height            = texpageJson["size"]["height"].get<unsigned>();
            texpage->textureId                   = texpageJson["sheetid"].get<unsigned>();

            std::scoped_lock lock(texpagesMutex);
            texpages[texpage->id] = texpage;
        });

        ke::Log::instance()->info("Scanning sprite assets...");
        const auto spriteRootDirPath = fs::path{ assetDirPath } / "sprite";
        const auto spritePaths = ke::FileSystemHelper::getChildPaths(spriteRootDirPath);
        std::for_each(std::execution::par_unseq, std::begin(spritePaths), std::end(spritePaths), [&](const auto & path)
        {
            if (!path.has_extension() || path.extension() != ".json") return;

            ke::Log::instance()->info("Loading GM:S sprite asset: {}", path.string());
            std::ifstream spriteFileStream{ path };
            ke::json spriteJson;
            spriteFileStream >> spriteJson;

            auto sprite = std::make_shared<pf::GMSSpriteResource>(path.stem().string(), path.string());

            sprite->dimension.width     = spriteJson["size"]["width"].get<unsigned>();
            sprite->dimension.height    = spriteJson["size"]["height"].get<unsigned>();
            sprite->boundingBox.top     = spriteJson["bounding"]["top"].get<unsigned>();
            sprite->boundingBox.left    = spriteJson["bounding"]["left"].get<unsigned>();
            sprite->boundingBox.bottom  = spriteJson["bounding"]["bottom"].get<unsigned>();
            sprite->boundingBox.right   = spriteJson["bounding"]["right"].get<unsigned>();
            sprite->boundingBoxMode     = spriteJson["bboxmode"].get<unsigned>();
            sprite->separateMasks       = spriteJson["sepmasks"].get<bool>();
            sprite->origin.x            = spriteJson["origin"]["x"].get<unsigned>();
            sprite->origin.y            = spriteJson["origin"]["y"].get<unsigned>();
            for (const auto & textureJson : spriteJson["textures"])
            {
                sprite->texpageIds.push_back(textureJson.get<unsigned>());
            }
        });
        // TODO: load and register sprite resources
    }

    void GMSAssetResourceManagementSystem::loadRoomAssets(void)
    {
        ke::Log::instance()->info("Scanning GM:S room assets...");
        const auto assetDirPath = ke::App::getCommandLineArgValue(pf::cli::ExecAssetsPath).as<ke::String>();
        const auto gmsRoomsRootDirPath = fs::path{ assetDirPath } / "rooms";
        const auto gmsRoomPaths = ke::FileSystemHelper::getFilePaths(gmsRoomsRootDirPath);
        std::hash<ke::String> hasher;
        std::for_each(std::execution::par_unseq, std::begin(gmsRoomPaths), std::end(gmsRoomPaths), [&](const auto & gmsRoomPath)
        {
            if (!gmsRoomPath.has_extension() || gmsRoomPath.extension() != ".json") return;

            ke::Log::instance()->info("Loading GM:S room asset: {}", gmsRoomPath.string());

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

            //
            // load background info
            //
            const auto & roomBackgroundsJson = roomJson["bgs"];
            for (const auto & backgroundJson : roomBackgroundsJson)
            {
                GMSRoomBackgroundInfo backgroundInfo;
                backgroundInfo.enabled    = backgroundJson["enabled"].get<bool>();
                backgroundInfo.foreground = backgroundJson["foreground"].get<bool>();
                backgroundInfo.pos        = { backgroundJson["pos"]["x"].get<int>(), -backgroundJson["pos"]["y"].get<int>() };
                backgroundInfo.tilex      = backgroundJson["tilex"].get<bool>();
                backgroundInfo.tiley      = backgroundJson["tiley"].get<bool>();
                backgroundInfo.speed      = { backgroundJson["speed"]["x"].get<int>(), -backgroundJson["speed"]["y"].get<int>() };
                backgroundInfo.stretch    = backgroundJson["stretch"].get<bool>();
                backgroundInfo.bg         = backgroundJson.value("bg", "");
                backgroundInfo.bg_hash    = hasher(backgroundInfo.bg);
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
                newTile.bg_hash   = hasher(newTile.bg);
                newTile.sourcepos = { tileJson["sourcepos"]["x"].get<int>(), tileJson["sourcepos"]["y"].get<int>() }; // sourcepos is y-down local image coordinates.
                newTile.size      = { tileJson["size"]["width"].get<int>(), tileJson["size"]["height"].get<int>() };
                newTile.scale     = { tileJson["scale"]["x"].get<float>(), tileJson["scale"]["y"].get<float>() };
                newTile.colour    = ::gmsColourStrToColour(tileJson["colour"].get<ke::String>());

                // Here convert GM:S' depth system to KEngine's depth system.
                // GM:S depth value: larger == further back.
                // KEngine depth value: larger == further in front.
                newTile.tiledepth = -tileJson["tiledepth"].get<ke::graphics::DepthType>();

                roomResource->addTile(newTile);
            }

            //
            // load object instances
            //
            const auto & roomObjsJson = roomJson["objs"];
            roomResource->objects.reserve(roomObjsJson.size());
            for (const auto & objJson : roomObjsJson)
            {
                pf::GMSRoomObjectInstance obj;
                obj.instanceid = objJson["instanceid"].get<ke::EntityId>();
                obj.obj        = objJson["obj"].get<ke::String>();
                obj.pos        = { objJson["pos"]["x"].get<int>(), -objJson["pos"]["y"].get<int>() };
                obj.scale      = { objJson["scale"]["x"].get<float>(), objJson["scale"]["y"].get<float>() };
                obj.rotation   = objJson["rotation"].get<float>();
                obj.colour     = ::gmsColourStrToColour(objJson["colour"].get<ke::String>());

                roomResource->addObject(obj);
            }

            ke::App::instance()->getResourceManager()->registerResource(roomResource);
        });
    }

    void GMSAssetResourceManagementSystem::loadObjectAssets(void)
    {
        ke::Log::instance()->info("Scanning GM:S object assets...");
        const auto assetDirPath = ke::App::getCommandLineArgValue(pf::cli::ExecAssetsPath).as<ke::String>();
        const auto gmsObjectRootDirPath = fs::path{ assetDirPath } / "object";
        const auto gmsObjectPaths = ke::FileSystemHelper::getFilePaths(gmsObjectRootDirPath);
        std::for_each(std::execution::par_unseq, std::begin(gmsObjectPaths), std::end(gmsObjectPaths), [&](const auto & gmsObjectPath)
        {
            if (!gmsObjectPath.has_extension() || gmsObjectPath.extension() != "json") return;

            ke::Log::instance()->info("Loading GM:S object asset: {}", gmsObjectPath.string());

            std::ifstream objectFileStream{ gmsObjectPath };
            ke::json objectJson;
            objectFileStream >> objectJson;

            auto objectResource = std::make_shared<pf::GMSObjectResource>(fs::path(gmsObjectPath).stem().string(), gmsObjectPath.string());
            objectResource->sprite   = objectJson["sprite"].get<ke::String>();
            objectResource->visible  = objectJson["visible"].get<bool>();
            objectResource->solid    = objectJson["solid"].get<bool>();
            objectResource->depth    = objectJson["depth"].get<decltype(objectResource->depth)>();
            objectResource->persist  = objectJson["persist"].get<bool>();
            objectResource->sensor   = objectJson["sensor"].get<bool>();
            objectResource->colshape = objectJson["colshape"].get<ke::String>();

            ke::App::instance()->getResourceManager()->registerResource(objectResource);
        });
    }
}