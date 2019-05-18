#pragma once

#include <KEngine/Interfaces/IEntity.hpp>
#include <KEngine/Interfaces/IResource.hpp>
#include <KEngine/Common/Point2D.hpp>
#include <KEngine/Common/Size2D.hpp>
#include <KEngine/Common/Color.hpp>
#include <KEngine/Common/Dimension2D.hpp>

#include <memory>
#include <vector>

namespace pf
{
    // forward declarations.

    class GMSCodeResource;


    struct GMSRoomTileInstance
    {
        // GM:S info

        ke::Point2DInt32 pos;
        ke::Point2DInt32 sourcepos;
        ke::Size2DInt32 size;
        ke::Point2DFloat scale{ 1.0f, 1.0f };
        ke::Colour colour{ke::Colour::WHITE};
        ke::String bg;
        int32_t tiledepth = 0;
        size_t instanceid = 0;


        // custom info
    };

    struct GMSRoomBackgroundInfo
    {
        // GM:S info

        bool enabled = true;
        bool foreground = false;
        ke::Point2DInt32 pos{ 0,0 };
        bool tilex = false;
        bool tiley = false;
        ke::Point2DInt32 speed{ 0,0 };
        bool stretch{ false };
        ke::String bg;

        
        // custom info

        ke::Point2DInt32 sourcepos;
        ke::Dimension2DUInt32 size;
    };

    struct GMSRoomObjectInstance
    {
        // GM:S info

        ke::String obj;
        ke::EntityId instanceid = ke::INVALID_ENTITY_ID;
        unsigned createcodeid = 0;  // 0 for invalid.
        ke::Point2DInt32 pos;
        ke::Point2DFloat scale{ 1.0f, 1.0f };
        float rotation{ 0.0f };
        ke::Colour colour = ke::Colour::WHITE;


        // custom info

        std::shared_ptr<GMSCodeResource> createcodeResource;
    };

    class GMSRoomResource : public ke::IResource
    {
        KE_DEFINE_RESOURCE_COMMON_PROPERTIES(GMSRoomResource, "GMSRoomResource");

    public:
        using BackgroundInfoContainer = std::vector<GMSRoomBackgroundInfo>;
        using TileInstanceContainer   = std::vector<GMSRoomTileInstance>;
        using ObjectInstanceContainer = std::vector<GMSRoomObjectInstance>;
        using SizeType                = ke::Dimension2DInt32;
        using SpeedType               = int32_t;

        inline const BackgroundInfoContainer & getBackgroundInfos() const { return this->backgrounds; }
        inline void addBackgroundInfo(const GMSRoomBackgroundInfo & backgroundInfo) { this->backgrounds.push_back(backgroundInfo); }

        inline const TileInstanceContainer & getTiles() const { return this->tiles; }
        inline void addTile(const GMSRoomTileInstance & tile) { this->tiles.push_back(tile); }

        inline const ObjectInstanceContainer & getObjects() const { return this->objects; }
        inline void addObject(const GMSRoomObjectInstance & object) { this->objects.push_back(object); }

        inline const SizeType getSize() const { return this->size; }
        inline void setSize(const SizeType & p_size) { this->size = p_size; }

        inline const SpeedType getSpeed() const { return this->speed; }
        inline void setSpeed(const SpeedType & p_speed) { this->speed = p_speed; }

        inline const ke::Colour & getColour() const { return this->colour; }
        inline void setColour(const ke::Colour & p_colour) { this->colour = p_colour; }

    private:
        BackgroundInfoContainer backgrounds;
        TileInstanceContainer tiles;
        ObjectInstanceContainer objects;
        SizeType size;
        SpeedType speed = 60; // RoR default seems to be 60.
        ke::Colour colour = ke::Colour::WHITE;

        friend class GMSAssetResourceManagementSystem;
    };

}
