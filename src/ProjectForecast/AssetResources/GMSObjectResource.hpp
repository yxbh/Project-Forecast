#pragma once

#include <KEngine/Interfaces/IResource.hpp>

namespace pf
{
    // forward declaration
    class GMSSpriteResource;


    /// <summary>
    /// A resource class that represents a GM:S object.
    /// This resource is used to instantiate KEngine entities (i.e. GM:S object instances).
    /// </summary>
    class GMSObjectResource : public ke::IResource
    {
        KE_DEFINE_RESOURCE_COMMON_PROPERTIES(GMSObjectResource, "GMSOjectResource");

    public:
        GMSObjectResource(const ke::String & p_name, const ke::String & p_sourcePath)
        {
            this->name = p_name;
            this->sourcePath = p_sourcePath;
        };


        // GMS:S asset info

        ke::String sprite;
        bool visible;
        bool solid;
        int depth;
        bool persist;
        bool sensor;
        ke::String colshape;


        // custom asset info

        std::shared_ptr<GMSSpriteResource> spriteResource;


        friend class GMSAssetResourceManagementSystem;
    };

}
