#pragma once

#include <KEngine/Interfaces/IResource.hpp>
#include <KEngine/Common/Point2D.hpp>
#include <KEngine/Common/Size2D.hpp>
#include <KEngine/Common/Color.hpp>
#include <KEngine/Common/Dimension2D.hpp>

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
        int32_t tiledepth;
        size_t instanceid;
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
        using TileInstanceContainer   = std::vector<GMSRoomTileInstance>;
        using SizeType                = ke::Dimension2DInt32;
        using SpeedType               = int32_t;

        inline const BackgroundInfoContainer & getBackgroundInfos() const { return this->backgrounds; }
        inline void addBackgroundInfo(const GMSRoomBackgroundInfo & backgroundInfo) { this->backgrounds.push_back(backgroundInfo); }

        inline const TileInstanceContainer & getTiles() const { return this->tiles; }
        inline void addTile(const GMSRoomTileInstance & tile) { this->tiles.push_back(tile); }

        inline const SizeType getSize() const { return this->size; }
        inline void setSize(const SizeType & p_size) { this->size = p_size; }

        inline const SpeedType getSpeed() const { return this->speed; }
        inline void setSpeed(const SpeedType & p_speed) { this->speed = p_speed; }

        inline const ke::Colour & getColour() const { return this->colour; }
        inline void setColour(const ke::Colour & p_colour) { this->colour = p_colour; }

    private:
        BackgroundInfoContainer backgrounds;
        TileInstanceContainer tiles;
        SizeType size;
        SpeedType speed;
        ke::Colour colour;

    };

}
