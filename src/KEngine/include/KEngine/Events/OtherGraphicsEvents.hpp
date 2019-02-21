#pragma once

#include "KEngine/Interfaces/IEvent.hpp"
#include "KEngine/Interfaces/ISceneNode.hpp"

#include "KEngine/Common/String.hpp"

#include <tuple>

namespace ke
{

    class CameraViewZoomDeltaRequestEvent : public IEvent
    {
        KE_DEFINE_EVENT_COMMON_PROPERTIES(CameraViewZoomDeltaRequestEvent, 0x6f01ac02);

    public:
        CameraViewZoomDeltaRequestEvent(float p_zoomDelta) : deltaZoom(p_zoomDelta) {}

        virtual ke::EventSptr makeCopy() const final
        {
            return ke::makeEvent<CameraViewZoomDeltaRequestEvent>(this->deltaZoom);
        }

        const float getDeltaZoom() const { return this->deltaZoom; }

    private:
        float deltaZoom;
    };


    class SetClearColourRequestEvent : public IEvent
    {
        KE_DEFINE_EVENT_COMMON_PROPERTIES(SetClearColourRequestEvent, 0x4bf2d20);

    public:
        SetClearColourRequestEvent(const ke::Colour & p_colour) : colour(p_colour) {}

        virtual ke::EventSptr makeCopy() const final
        {
            return ke::makeEvent<SetClearColourRequestEvent>(this->colour);
        }

        const ke::Colour & getColour() const { return this->colour; }

    private:
        ke::Colour colour;
    };


    class SceneNodeCreatedEvent : public IEvent
    {
        KE_DEFINE_EVENT_COMMON_PROPERTIES(SceneNodeCreatedEvent, 0x887f89b2);

    public:
        SceneNodeCreatedEvent(SceneNodeSptr scenenode) : scenenode(scenenode) {}

        virtual ke::EventSptr makeCopy() const final
        {
            return ke::makeEvent<SceneNodeCreatedEvent>(this->scenenode);
        }

        SceneNodeSptr getSceneNode() const { return this->scenenode; }

    private:
        SceneNodeSptr scenenode;
    };


    class SceneNodeDestroyRequestEvent : public IEvent
    {
        KE_DEFINE_EVENT_COMMON_PROPERTIES(SceneNodeDestroyRequestEvent, 0x5b4eb2a3);

    public:
        SceneNodeDestroyRequestEvent(SceneNodeSptr scenenode) : scenenode(scenenode) {}

        virtual ke::EventSptr makeCopy() const final
        {
            return ke::makeEvent<SceneNodeDestroyRequestEvent>(this->scenenode);
        }

        SceneNodeSptr getSceneNode() const { return this->scenenode; }

    private:
        SceneNodeSptr scenenode;
    };


    class TextureLoadViaFileRequestEvent : public IEvent
    {
        KE_DEFINE_EVENT_COMMON_PROPERTIES(TextureLoadViaFileRequestEvent, 0xa4f9a42a);

    public:
        TextureLoadViaFileRequestEvent(const ke::String & textureName, const size_t textureId, const ke::String & textureSourcePath)
         : textureName(textureName), textureSourcePath(textureSourcePath), textureId(textureId) {}

        virtual ke::EventSptr makeCopy() const final
        {
            return ke::makeEvent<TextureLoadViaFileRequestEvent>(this->textureName, this->textureId, this->textureSourcePath);
        }

        inline const ke::String & getTextureName() const
        {
            return this->textureName;
        }

        inline const ke::String & getTextureSourcePath() const
        {
            return this->textureSourcePath;
        }

        inline const size_t & getTextureId() const
        {
            return this->textureId;
        }

    private:
        ke::String textureName;
        ke::String textureSourcePath;
        size_t textureId;
    };


    class TexturesBulkLoadViaFilesRequestEvent : public IEvent
    {
        KE_DEFINE_EVENT_COMMON_PROPERTIES(TexturesBulkLoadViaFilesRequestEvent, 0x8f55bb0);

        static constexpr size_t VEC_RESERVE_MIN = 4096;

    public:
        using TextureInfo = std::tuple<ke::String, std::size_t, ke::String>; // name, texture ID & source path.

        TexturesBulkLoadViaFilesRequestEvent(void)
        {
            this->textureInfo.reserve(VEC_RESERVE_MIN);
        }
        TexturesBulkLoadViaFilesRequestEvent(const TexturesBulkLoadViaFilesRequestEvent & p_event)
            : textureInfo(p_event.textureInfo) {}

        virtual ke::EventSptr makeCopy() const final
        {
            return ke::makeEvent<TexturesBulkLoadViaFilesRequestEvent>(*this);
        }

        inline void addTextureInfo(const ke::String & p_name, const size_t p_id, const ke::String & p_sourcePath)
        {
            this->textureInfo.push_back({ p_name, p_id, p_sourcePath });
        }

        inline const std::vector<TextureInfo> & getAllTextureInfo(void) const
        {
            return this->textureInfo;
        }


    private:
        std::vector<TextureInfo> textureInfo;
    };


    class TextureUnloadRequestEvent : public IEvent
    {
        KE_DEFINE_EVENT_COMMON_PROPERTIES(TextureUnloadRequestEvent, 0x930b6c83);

    public:
        TextureUnloadRequestEvent(const ke::String & textureName, const size_t textureId)
            : textureName(textureName), textureId(textureId) {}

        virtual ke::EventSptr makeCopy() const final
        {
            return ke::makeEvent<TextureUnloadRequestEvent>(this->textureName, this->textureId);
        }

        inline const ke::String & getTextureName() const
        {
            return this->textureName;
        }

        inline const size_t & getTextureId() const
        {
            return this->textureId;
        }

    private:
        ke::String textureName;
        size_t textureId;
    };

}