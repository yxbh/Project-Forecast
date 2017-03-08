#pragma once

#include "KEngine/Interface/IEntity.hpp"
#include "KEngine/Interface/IInputController.hpp"

namespace pf
{

    class MouseInputController : public ke::IMouseInputController
    {
    public:
        virtual void update(ke::Time elapsedTime) override;
        virtual bool onButtonPressed(ke::Mouse::Button button, const ke::Point2DInt32 & position) override;
        virtual bool onButtonReleased(ke::Mouse::Button button, const ke::Point2DInt32 & position) override;
        virtual bool onPointerMoved() override;
    };

    class KeyboardInputController : public ke::IKeyboardInputController
    {
    public:
        KeyboardInputController(ke::EntityId p_entityId = ke::INVALID_ENTITY_ID)
            : entityId(p_entityId) {}

        inline void attachEntity(ke::EntityId p_entityId)
        {
            this->entityId = p_entityId;
        }

        virtual void update(ke::Time elapsedTime) override;
        virtual bool onKeyPressed(const ke::Keyboard::KeyInfo & keyInfo) override;
        virtual bool onKeyReleased(const ke::Keyboard::KeyInfo & keyInfo) override;

    private:
        ke::EntityId entityId;
    };


    
}