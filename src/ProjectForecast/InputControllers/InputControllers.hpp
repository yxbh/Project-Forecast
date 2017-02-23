#pragma once

#include "KEngine/Interface/IInputController.hpp"

namespace pf
{

    class MouseInputController : public ke::IMouseInputController
    {
    public:
        virtual void update(ke::Time elapsedTime) override;
        virtual bool onButtonPressed(ke::MouseButton button, const ke::Point2D & position) override;
        virtual bool onButtonReleased(ke::MouseButton button, const ke::Point2D & position) override;
        virtual bool onPointerMoved() override;
    };

    class KeyboardInputController : public ke::IKeyboardInputController
    {
    public:
        virtual void update(ke::Time elapsedTime) override;
        virtual bool onKeyPressed() override;
        virtual bool onKeyReleased() override;
    };


    
}