#pragma once

#include "KEngine/App.hpp"
#include "KEngine/BaseAppLogic.hpp"

namespace pf
{

    /// <summary>
    /// 
    /// </summary>
    class ProjectForecastApp : public ke::App
    {
    public:
        ProjectForecastApp(const int p_argc, char ** const p_argv);

        virtual void reloadConfigsFromDisk() override;

    protected:
        virtual void createLogicAndViews() final;
    };

    /// <summary>
    /// 
    /// </summary>
    class ProjectForecastAppLogic : public ke::BaseAppLogic
    {
    public:
        ProjectForecastAppLogic();
        virtual ~ProjectForecastAppLogic();

    private:
        void autoAddNewSceneNodeToCurrentViewSceneOnNodeCreate(ke::EventSptr event);
    };

}