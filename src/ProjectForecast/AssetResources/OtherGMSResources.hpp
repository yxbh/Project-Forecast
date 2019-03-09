#pragma once

#include <KEngine/Interfaces/IResource.hpp>
#include <KEngine/Common/Point2D.hpp>
#include <KEngine/Common/Dimension2D.hpp>

#include <memory>
#include <unordered_map>
#include <vector>

namespace pf
{
    /// <summary>
    /// A resource representing a GM:S texpage object.
    /// </summary>
    class GMSTexpageResource : public ke::IResource
    {
        KE_DEFINE_RESOURCE_COMMON_PROPERTIES(GMSTexpageResource, "GMSTexpageResource");

    public:
        GMSTexpageResource(const ke::String & p_name, const ke::String & p_sourcePath)
        {
            this->name = p_name;
            this->sourcePath = p_sourcePath;
        };

        ke::Point2DUInt32 sourcePosition; // texture sheet coordinate.
        ke::Dimension2DUInt32 sourceDimension;

        ke::Point2DUInt32 destinationPosition;
        ke::Dimension2DUInt32 destinationDimension;

        ke::Dimension2DUInt32 dimension;

        unsigned sheetid; // the GM:S texture sheet name (i.e. "textures" folder content).
        unsigned id;


        friend class GMSAssetResourceManagementSystem;
    };


    /// <summary>
    /// A container class which holds a mapping of GM:S texpage ID to <see cref="GMSTexpageResource"/>.
    /// </summary>
    class TexpageMapResource : public ke::IResource
    {
        KE_DEFINE_RESOURCE_COMMON_PROPERTIES(TexpageMapResource, "TexpageMapResource");

    public:
        TexpageMapResource(const ke::String & p_name, const ke::String & p_sourcePath)
        {
            this->name = p_name;
            this->sourcePath = p_sourcePath;
        };

        std::unordered_map<unsigned, std::shared_ptr<GMSTexpageResource>> texpages; // <texpage_id, texpage>
        friend class GMSAssetResourceManagementSystem;
    };


    /// <summary>
    /// A resource representing a GM:S texture object.
    /// </summary>
    class GMSTextureResource : public ke::IResource
    {
        KE_DEFINE_RESOURCE_COMMON_PROPERTIES(GMSTextureResource, "GMSTextureResource");

    public:
        GMSTextureResource(const ke::String & p_name, const ke::String & p_sourcePath)
        {
            this->name = p_name;
            this->sourcePath = p_sourcePath;
        };


        unsigned id;

        friend class GMSAssetResourceManagementSystem;
    };


    /// <summary>
    /// A resource representing a GM:S bg object.
    /// </summary>
    class GMSBgResource : public ke::IResource
    {
        KE_DEFINE_RESOURCE_COMMON_PROPERTIES(GMSBgResource, "GMSBgResource");

    public:
        GMSBgResource(const ke::String & p_name, const ke::String & p_sourcePath)
        {
            this->name = p_name;
            this->sourcePath = p_sourcePath;
        };

        unsigned texture; // it is really the texpage name/id.

        std::weak_ptr<GMSTexpageResource> texpageResource;
    };


    /// <summary>
    /// A resource representing a GM:S sprite object.
    /// </summary>
    class GMSSpriteResource : public ke::IResource
    {
        KE_DEFINE_RESOURCE_COMMON_PROPERTIES(GMSSpriteResource, "GMSSpriteResource");

    public:
        struct BoundingBox2D
        {
            unsigned top = 0;
            unsigned left = 0;
            unsigned bottom = 0;
            unsigned right = 0;
        };


        GMSSpriteResource(const ke::String & p_name, const ke::String & p_sourcePath)
        {
            this->name = p_name;
            this->sourcePath = p_sourcePath;
        };


        ke::Dimension2DUInt32 dimension;
        BoundingBox2D boundingBox;

        unsigned boundingBoxMode = 0;

        ke::Point2DUInt32 origin;

        bool separateMasks = true;

        ke::String name;

        std::vector<unsigned> texpageIds;



        friend class GMSAssetResourceManagementSystem;
    };

}
