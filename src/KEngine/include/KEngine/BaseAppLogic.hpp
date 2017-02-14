#pragma once

#include "KEngine/Common/Time.hpp"
#include "KEngine/Core/EntityFactory.hpp"
#include "KEngine/Core/EntityManager.hpp"
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

        virtual ~BaseAppLogic();

        /// <summary>
        /// Called by the application layer. This is a lifecycle memmber functions that
        /// runs all on going logics based on the given elasped time so far.
        /// This function is non-virtual and should not be reimplemented.
        /// </summary>
        /// <param name="elapsedTime"></param>
        inline void onUpdate(ke::Time elapsedTime) { this->update(elapsedTime); }

        virtual EntityFactory * getEntityFactory() { return &this->entityFactory; }
        virtual EntityManager * getEntityManager() { return &this->entityManager; }

        virtual void addView(ke::AppViewSptr view, bool setAsCurrent = false);
        virtual ke::IAppView * getCurrentView() { return this->currentAppView; }
        virtual AppViewList & getViews() { return this->appViews; }

    protected:
        /// <summary>
        /// Update logics. Reimplement this to create custom logic updates.
        /// This member function is called onUpdate() from the KEngine application layer. 
        /// </summary>
        /// <param name="elapsedTime"></param>
        virtual void update(ke::Time elapsedTime);

    protected:
        ke::EntityFactory entityFactory;
        ke::EntityManager entityManager;

        AppViewList appViews;
        ke::IAppView * currentAppView;
    };

    inline BaseAppLogic::~BaseAppLogic() {}

    using AppLogicUptr = std::unique_ptr<BaseAppLogic>;
    using AppLogicSptr = std::shared_ptr<BaseAppLogic>;
    using AppLogicWptr = std::weak_ptr<BaseAppLogic>;

}