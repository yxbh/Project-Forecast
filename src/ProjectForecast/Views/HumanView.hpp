#pragma once

#include "../InputControllers/InputControllers.hpp"

#include "KEngine/Interfaces/IEvent.hpp"
#include "KEngine/Views/HumanView.hpp"

namespace sf
{
    class Event;
}

namespace pf
{
    
    class HumanView : public ke::HumanView
    {
    public:
        HumanView();
        virtual ~HumanView();

        virtual void attachEntity(ke::EntityId entityId) override;
        virtual void update(ke::Time elapsedTime) override;

    private:
        void handleWindowEvent(ke::EventSptr event);


        std::string testTextBuffer;

        ke::MouseInputControllerUptr mouseController;
        ke::KeyboardInputControllerUptr keyboardController;
    };

}