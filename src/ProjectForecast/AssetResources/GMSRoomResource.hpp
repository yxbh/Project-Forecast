#pragma once

#include <KEngine/Interfaces/IResource.hpp>
#include <KEngine/Common/Point2D.hpp>
#include <KEngine/Common/Size2D.hpp>
#include <KEngine/Common/Color.hpp>

#include <vector>

namespace pf
{

    struct GMSRoomTileInstance
    {
    public:
        ke::Point2DInt32 pos;
        ke::Point2DInt32 sourcepos;
        ke::Size2DInt32 size;
        ke::Point2DFloat scale{ 1.0f, 1.0f };
        ke::Colour colour{ke::Colour::WHITE};
        ke::String bg;
        size_t bg_hash;
        std::int16_t tiledepth;
        unsigned instanceid;
    };

    struct GMSRoomBackgroundInfo
    {
    public:
        bool enabled;
        bool foreground;
        ke::Point2DInt32 pos{ 0,0 };
        bool tilex;
        bool tiley;
        ke::Point2DInt32 speed{ 0,0 };
        bool stretch{ false };
        ke::String bg;
        size_t bg_hash;
    };

    class GMSRoomResource : public ke::IResource
    {
        KE_DEFINE_RESOURCE_COMMON_PROPERTIES(GMSRoomResource, "GMSRoomResource");

    public:
        using BackgroundInfoContainer = std::vector<GMSRoomBackgroundInfo>;
        using TileInstanceContainer = std::vector<GMSRoomTileInstance>;

        inline const BackgroundInfoContainer & getBackgroundInfos() const { return this->backgrounds; }
        inline void addBackgroundInfo(const GMSRoomBackgroundInfo & backgroundInfo) { this->backgrounds.push_back(backgroundInfo); }

        inline const TileInstanceContainer & getTiles() const { return this->tiles; }
        inline void addTile(const GMSRoomTileInstance & tile) { this->tiles.push_back(tile); }

    private:
        BackgroundInfoContainer backgrounds;
        TileInstanceContainer tiles;

    };

}
