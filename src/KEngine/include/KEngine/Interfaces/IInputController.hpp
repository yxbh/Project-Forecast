#pragma once

#include "KEngine/Common/Time.hpp"
#include "KEngine/Common/Point2D.hpp"
#include "KEngine/Core/Mouse.hpp"
#include "KEngine/Core/Keyboard.hpp"

#include <cstdint>
#include <memory>


namespace ke
{

    /// <summary>
    /// Interface for implementing a mouse input controller that converts mouse input into control command events.
    /// </summary>
    class IMouseInputController
    {
    public:
        virtual ~IMouseInputController(void) {}

        virtual void update(ke::Time) {};
        /// <summary>
        /// 
        /// </summary>
        /// <param name="button"></param>
        /// <returns>true if button press was handled.</returns>
        virtual bool onButtonPressed(const ke::Mouse::ButtonInfo & buttonInfo) = 0;
        /// <summary>
        /// 
        /// </summary>
        /// <param name="button"></param>
        /// <returns>true if the button release was handled.</returns>
        virtual bool onButtonReleased(const ke::Mouse::ButtonInfo & buttonInfo) = 0;
        virtual bool onWheelScrolled(const ke::Mouse::ScrollWheelInfo & scrollWheelInfo) = 0;
        virtual bool onPointerMoved(const ke::Mouse::MouseInfo & mouseInfo) = 0;
    };

    using MouseInputControllerUptr = std::unique_ptr<IMouseInputController>;
    using MouseInputControllerSptr = std::shared_ptr<IMouseInputController>;
    using MouseInputControllerWptr = std::weak_ptr<IMouseInputController>;


    /// <summary>
    /// Interface for implementing a keyboard input controller that converts keyboard input into control command events.
    /// </summary>
    class IKeyboardInputController
    {
    public:
        virtual ~IKeyboardInputController(void) {}

        virtual void update(ke::Time elapsedTime) { KE_UNUSED(elapsedTime); };
        /// <summary>
        /// 
        /// </summary>
        /// <returns>true if key press was handled.</returns>
        virtual bool onKeyPressed(const ke::Keyboard::KeyInfo & keyInfo) = 0;
        /// <summary>
        /// 
        /// </summary>
        /// <returns>true if key release was handled.</returns>
        virtual bool onKeyReleased(const ke::Keyboard::KeyInfo & keyInfo) = 0;
    };

    using KeyboardInputControllerUptr = std::unique_ptr<IKeyboardInputController>;
    using KeyboardInputControllerSptr = std::shared_ptr<IKeyboardInputController>;
    using KeyboardInputControllerWptr = std::weak_ptr<IKeyboardInputController>;


    /// <summary>
    /// Interface for implementing a gamepad input controller that converts gamepad input into control command events.
    /// </summary>
    class IGamepadInputController
    {
    public:
        virtual void update(ke::Time) {};
    };

    using GamepadInputControllerUptr = std::unique_ptr<IGamepadInputController>;
    using GamepadInputControllerSptr = std::shared_ptr<IGamepadInputController>;
    using GamepadInputControllerWptr = std::weak_ptr<IGamepadInputController>;


}