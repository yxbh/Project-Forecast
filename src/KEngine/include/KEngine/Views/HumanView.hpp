#pragma once

#include "KEngine/Interfaces/IAppView.hpp"
#include "KEngine/Interfaces/IEntity.hpp"
#include "KEngine/Interfaces/IScene.hpp"

namespace ke
{

    /// <summary>
    /// 
    /// </summary>
    class HumanView : public ke::IAppView
    {
    public:
        virtual ~HumanView();

        virtual void attachEntity(ke::EntityId entityId) override;

        virtual ke::Scene* getScene() const override { return this->scene.get(); }

        virtual bool isHumanView() const final { return true; };

    protected:
        ke::SceneUptr scene;

        ke::EntityId currentEntityId = ke::INVALID_ENTITY_ID;

    };

    inline HumanView::~HumanView() {}

}