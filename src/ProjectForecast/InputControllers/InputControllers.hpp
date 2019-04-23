#pragma once

#include "KEngine/Interfaces/IEntity.hpp"
#include "KEngine/Interfaces/IEvent.hpp"
#include "KEngine/Interfaces/IInputController.hpp"

namespace pf
{

    class MouseInputController : public ke::IMouseInputController
    {
    public:
        virtual void update(ke::Time elapsedTime) override;
        virtual bool onButtonPressed(const ke::Mouse::ButtonInfo & buttonInfo) override;
        virtual bool onButtonReleased(const ke::Mouse::ButtonInfo & buttonInfo) override;
        virtual bool onWheelScrolled(const ke::Mouse::ScrollWheelInfo & scrollWheelInfo) override;
        virtual bool onPointerMoved(const ke::Mouse::MouseInfo & mouseInfo) override;
    };

    class KeyboardInputController : public ke::IKeyboardInputController
    {
    public:
        KeyboardInputController(ke::EntityId p_entityId = ke::INVALID_ENTITY_ID);
        virtual ~KeyboardInputController();

        inline void attachEntity(ke::EntityId p_entityId)
        {
            this->entityId = p_entityId;
        }

        virtual void update(ke::Time elapsedTime) override;
        virtual bool onKeyPressed(const ke::Keyboard::KeyInfo & keyInfo) override;
        virtual bool onKeyReleased(const ke::Keyboard::KeyInfo & keyInfo) override;

        void processEvent(ke::EventSptr event);

    private:
        ke::EntityId entityId;
        bool isWindowInFocus = true;
    };


    
}