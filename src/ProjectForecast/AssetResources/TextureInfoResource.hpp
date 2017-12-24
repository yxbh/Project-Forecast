#pragma once

#include <KEngine/Interfaces/IResource.hpp>

namespace pf
{

    class TextureInfoResource : public ke::IResource
    {
        KE_DEFINE_RESOURCE_COMMON_PROPERTIES(TextureInfoResource, "TextureInfoResource"/*0x9bd52b62*/);
    
    public:
        inline void setTextureId(size_t id) { this->textureId = id; }
        inline size_t getTextureId() const { return this->textureId; }
    
    private:
        size_t textureId;

    };

}