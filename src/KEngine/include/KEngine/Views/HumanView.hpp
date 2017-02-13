#pragma once

#include "KEngine/Interface/IAppView.hpp"
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

    };

    inline HumanView::~HumanView() {}

}