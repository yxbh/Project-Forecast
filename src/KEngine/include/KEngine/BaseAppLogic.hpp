#pragma once

#include "KEngine/Common/Time.hpp"
#include "KEngine/Core/EntityFactory.hpp"
#include "KEngine/Core/EntityManager.hpp"
#include "KEngine/Core/SystemManager.hpp"
#include "KEngine/Interface/IAppView.hpp"

#include <memory>
#include <vector>

namespace ke
{

    /// <summary>
    /// A base implementation of the application logic interface.
    /// This class controls the transitions between different base states of a game implementation.
    /// E.g. main menu, game level, etc...
    /// </summary>
    class BaseAppLogic
    {
    public:
        using AppViewList = std::vector<ke::AppViewSptr>;

        BaseAppLogic();
        virtual ~BaseAppLogic();

        /// <summary>
        /// Called by the application layer. This is a lifecycle memmber functions that
        /// runs all on going logics based on the given elasped time so far.
        /// This function is non-virtual and should not be reimplemented.
        /// This function should not be called by you.
        /// </summary>
        /// <param name="elapsedTime"></param>
        void update(ke::Time elapsedTime) { this->onUpdate(elapsedTime); }

        virtual void addView(ke::AppViewSptr view, bool setAsCurrent = false);
        virtual ke::IAppView * getCurrentView() { return this->currentAppView; }
        virtual AppViewList & getViews() { return this->appViews; }

        inline EntityFactory * getEntityFactory() { return &this->entityFactory; }
        inline EntityManager * getEntityManager() { return &this->entityManager; }
        inline SystemManager * getSystemManager() { return &this->systemManager; }

    protected:
        /// <summary>
        /// Update logics. Reimplement this to create custom logic updates.
        /// This member function is called by update() from the KEngine application layer. 
        /// </summary>
        /// <param name="elapsedTime"></param>
        virtual void onUpdate(ke::Time elapsedTime);

    protected:
        ke::EntityFactory entityFactory;
        ke::EntityManager entityManager;
        ke::SystemManager systemManager;

        AppViewList appViews;
        ke::IAppView * currentAppView;
    };

    inline BaseAppLogic::~BaseAppLogic() {}

    using AppLogicUptr = std::unique_ptr<BaseAppLogic>;
    using AppLogicSptr = std::shared_ptr<BaseAppLogic>;
    using AppLogicWptr = std::weak_ptr<BaseAppLogic>;

}