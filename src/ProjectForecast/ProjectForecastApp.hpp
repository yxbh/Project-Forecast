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
    protected:
        virtual void createLogicAndViews() final;
    };

    /// <summary>
    /// 
    /// </summary>
    class ProjectForecastAppLogic : public ke::BaseAppLogic
    {
    public:

    };

}