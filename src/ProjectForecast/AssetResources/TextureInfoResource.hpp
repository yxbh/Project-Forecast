#pragma once

#include <KEngine/Interfaces/IResource.hpp>
#include <KEngine/Common/Dimension2D.hpp>

namespace pf
{

    class TextureInfoResource : public ke::IResource
    {
        KE_DEFINE_RESOURCE_COMMON_PROPERTIES(TextureInfoResource, "TextureInfoResource");
    
    public:
        using DimensionType = ke::Dimension2DUInt32;

        inline void setTextureId(size_t id) { this->textureId = id; }
        inline size_t getTextureId() const { return this->textureId; }

        inline void setTextureSize(const DimensionType & p_size) { this->textureSize = p_size; }
        inline const DimensionType & getTextureSize() const { return this->textureSize; }
    
    private:
        size_t textureId = std::numeric_limits<size_t>::max();  // max value as invalid value.
        DimensionType textureSize;

    };

}