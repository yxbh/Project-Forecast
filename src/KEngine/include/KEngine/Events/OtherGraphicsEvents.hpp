#pragma once

#include "KEngine/Interfaces/IEvent.hpp"
#include "KEngine/Interfaces/ISceneNode.hpp"

#include "KEngine/Common/String.hpp"

namespace ke
{

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