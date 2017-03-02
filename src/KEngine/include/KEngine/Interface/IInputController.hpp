#pragma once

#include "KEngine/Common/Time.hpp"
#include "KEngine/Common/Point2D.hpp"

#include <cstdint>
#include <memory>


namespace ke
{

    enum MouseButton : std::uint32_t
    {
        Left   = 1,
        Right  = 2,
        Middle = 3
    };


    /// <summary>
    /// Interface for implementing a mouse input controller that converts mouse input into control command events.
    /// </summary>
    class IMouseInputController
    {
    public:
        virtual void update(ke::Time) {};
        /// <summary>
        /// 
        /// </summary>
        /// <param name="button"></param>
        /// <returns>true if button press was handled.</returns>
        virtual bool onButtonPressed(ke::MouseButton button, const Point2D & position) = 0;
        /// <summary>
        /// 
        /// </summary>
        /// <param name="button"></param>
        /// <returns>true if the button release was handled.</returns>
        virtual bool onButtonReleased(ke::MouseButton button, const Point2D & position) = 0;
        virtual bool onPointerMoved() = 0;
    };

    using MouseInputControllerUptr = std::unique_ptr<IMouseInputController>;
    using MouseInputControllerSptr = std::shared_ptr<IMouseInputController>;
    using MouseInputControllerWptr = std::weak_ptr<IMouseInputController>;


    struct KeyBoardKeyInfo
    {
        int32_t keyCode;
        int32_t scanCode;
    };

    /// <summary>
    /// Interface for implementing a keyboard input controller that converts keyboard input into control command events.
    /// </summary>
    class IKeyboardInputController
    {
    public:
        virtual void update(ke::Time elapsedTime) { KE_UNUSED(elapsedTime); };
        /// <summary>
        /// 
        /// </summary>
        /// <returns>true if key press was handled.</returns>
        virtual bool onKeyPressed(const ke::KeyBoardKeyInfo & keyInfo) = 0;
        /// <summary>
        /// 
        /// </summary>
        /// <returns>true if key release was handled.</returns>
        virtual bool onKeyReleased(const ke::KeyBoardKeyInfo & keyInfo) = 0;
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