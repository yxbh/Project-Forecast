#include "GMSAssetResourceManagementSystem.hpp"

#include "../AssetResources/TextureInfoResource.hpp"
#include "../AssetResources/GMSRoomResource.hpp"

#include <KEngine/Events/OtherGraphicsEvents.hpp>
#include <KEngine/App.hpp>
#include <KEngine/Core/EventManager.hpp>
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

namespace
{
    static const auto ProjectForecastExecAssetsRoot = "D:/workspace/ProjectForecastExecAssetsRoot";
    static const auto ProjectForecastExecAssetPath = "D:/workspace/ProjectForecastExecAssetsRoot/assets";
}

namespace pf
{

    bool GMSAssetResourceManagementSystem::initialise()
    {
        ke::Log::instance()->info("Scanning assets...");
        namespace fs = std::filesystem;

        ke::Log::instance()->info("Scanning texture assets...");
        fs::path texturesRootDirPath = fs::path{ ProjectForecastExecAssetPath } / "textures";
        sf::Image tempImage;
        std::hash<ke::String> hasher;
        for (const auto & texDirPath : ke::FileSystemHelper::getChildPaths(texturesRootDirPath))
        {
            ke::Log::instance()->info("Discovered texture asset: {}", texDirPath.string());

            auto textureFilePaths = ke::FileSystemHelper::getFilePaths(texDirPath);
            if (textureFilePaths.size() == 1)
            {
                auto texPath         = textureFilePaths[0];
                auto textureResource = std::make_shared<TextureInfoResource>();                
                textureResource->setName(texPath.stem().string());
                textureResource->setTextureId(hasher(textureResource->getName()));
                textureResource->setSourcePath(texPath.string());

                // retrieve size
                bool ret = tempImage.loadFromFile(texPath.string());
                assert(ret);
                TextureInfoResource::DimensionType dimension;
                dimension.width  = tempImage.getSize().x;
                dimension.height = tempImage.getSize().y;
                textureResource->setTextureSize(dimension);

                ke::App::instance()->getResourceManager()->registerResource(textureResource);
            }
            else
            {
                // ignore when there're multiple texture files in a single dir for now.
            }
            
        }

        ke::Log::instance()->info("Scanning GM:S room assets...");
        const auto gmsRoomsRootDirPath = fs::path{ ProjectForecastExecAssetPath } / "rooms";
        const auto gmsRoomPaths = ke::FileSystemHelper::getFilePaths(gmsRoomsRootDirPath);
        std::mutex gmsRoomLoadMutex;
        std::for_each(std::execution::par_unseq, std::begin(gmsRoomPaths), std::end(gmsRoomPaths), [&](const auto & gmsRoomPath) {
            ke::Log::instance()->info("Discovered GM:S room asset: {}", gmsRoomPath.string());
            auto roomResource = std::make_shared<GMSRoomResource>();
            roomResource->setName(gmsRoomPath.stem().string());
            roomResource->setSourcePath(gmsRoomPath.string());

            std::ifstream roomFileStream{ gmsRoomPath.string() };
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
            assert(roomColourStr.length() == 9);
            assert(roomColourStr[0] == '#');
            ke::Colour roomColour = {
                // assume colour is in hex RGBA starting with the '#' symbol.
                static_cast<uint8_t>(std::stol(roomColourStr.substr(1, 2), nullptr, 16)),
                static_cast<uint8_t>(std::stol(roomColourStr.substr(3, 2), nullptr, 16)),
                static_cast<uint8_t>(std::stol(roomColourStr.substr(5, 2), nullptr, 16)),
                static_cast<uint8_t>(std::stol(roomColourStr.substr(7, 2), nullptr, 16)) };
            roomResource->setColour(roomColour);

            //
            // load background info
            //
            auto roomBackgroundsJson = roomJson["bgs"];
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
            auto roomTilesJson = roomJson["tiles"];
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

                auto colourStr    = tileJson["colour"].get<ke::String>();
                assert(colourStr.length() == 9);
                assert(colourStr[0] == '#');
                newTile.colour    = {
                    // assume colour is in hex RGBA starting with the '#' symbol.
                    static_cast<uint8_t>(std::stol(colourStr.substr(1, 2), nullptr, 16)),
                    static_cast<uint8_t>(std::stol(colourStr.substr(3, 2), nullptr, 16)),
                    static_cast<uint8_t>(std::stol(colourStr.substr(5, 2), nullptr, 16)),
                    static_cast<uint8_t>(std::stol(colourStr.substr(7, 2), nullptr, 16)) };

                // Here convert GM:S' depth system to KEngine's depth system.
                // GM:S depth value: larger == further back.
                // KEngine depth value: larger == further in front.
                newTile.tiledepth = -tileJson["tiledepth"].get<ke::graphics::DepthType>();

                roomResource->addTile(newTile);
            }

            std::scoped_lock lock(gmsRoomLoadMutex);
            ke::App::instance()->getResourceManager()->registerResource(roomResource);
        });

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

}