#pragma once

#include "KEngine/Interface/IAppView.hpp"
#include "KEngine/Interface/IEntity.hpp"
#include "KEngine/Graphics/Scene.hpp"

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

        virtual ke::Scene * getScene() const override { return this->scene.get(); }

    private:
        ke::SceneUptr scene;

        ke::EntityId currentEntityId = ke::INVALID_ENTITY_ID;

    };

    inline HumanView::~HumanView() {}

}