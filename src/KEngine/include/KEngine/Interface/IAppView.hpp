#pragma once

#include "KEngine/Interface/IEntity.hpp"
#include "KEngine/Interface/IScene.hpp"

#include <memory>

namespace ke
{

    using AppViewId = std::uint32_t;

    /// <summary>
    /// Interface for implementing views that represents an entity's (a player, AI, network, etc) view of the app/game.
    /// </summary>
    class IAppView
    {
    public:
        virtual ~IAppView() = 0;

        virtual void attachEntity(ke::EntityId entityId) = 0;

        virtual void update(ke::Time elapsedTime) = 0;

        inline void setId(AppViewId newId) { this->id = newId; }
        inline AppViewId getId() const { return this->id; }

        virtual ke::Scene * getScene() const { return nullptr; }

    protected:
        AppViewId id;

    };

    inline IAppView::~IAppView() {}


    using AppViewUptr = std::unique_ptr<IAppView>;
    using AppViewSptr = std::shared_ptr<IAppView>;
    using AppViewWptr = std::weak_ptr<IAppView>;

}
