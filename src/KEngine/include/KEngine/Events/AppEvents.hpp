#pragma once

#include "KEngine/Interfaces/IEvent.hpp"
#include "KEngine/Interfaces/IWindow.hpp"
#include "KEngine/Interfaces/IInputController.hpp"

#include "KEngine/Common/Dimension2D.hpp"
#include "KEngine/Common/Point2D.hpp"

namespace ke
{

    class KeyboardKeyEvent : public ke::IEvent
    {
        KE_DEFINE_EVENT_COMMON_PROPERTIES(KeyboardKeyEvent, 0xE1D2E239)

    public:
        KeyboardKeyEvent(const ke::Keyboard::KeyInfo & keyInfo) : detail(keyInfo) {}

        inline const ke::Keyboard::KeyInfo & getDetail() const { return this->detail; }

    protected:
        ke::Keyboard::KeyInfo detail;
    };

    class KeyboardKeyPressedEvent : public KeyboardKeyEvent
    {
        KE_DEFINE_EVENT_COMMON_PROPERTIES(KeyboardKeyPressedEvent, 0x8B492E75)

    public:
        using KeyboardKeyEvent::KeyboardKeyEvent;

        virtual ke::EventSptr makeCopy() const final
        {
            return ke::makeEvent<KeyboardKeyPressedEvent>(this->detail);
        }
    };

    class KeyboardKeyReleasedEvent : public KeyboardKeyEvent
    {
        KE_DEFINE_EVENT_COMMON_PROPERTIES(KeyboardKeyReleasedEvent, 0xABCAB2CA)

    public:
        using KeyboardKeyEvent::KeyboardKeyEvent;

        virtual ke::EventSptr makeCopy() const final
        {
            return ke::makeEvent<KeyboardKeyReleasedEvent>(this->detail);
        }
    };

    class KeyboardTextEvent : public ke::IEvent
    {
        KE_DEFINE_EVENT_COMMON_PROPERTIES(KeyboardTextEvent, 0x795AF2F1)

    public:
        KeyboardTextEvent(const ke::Keyboard::TextInfo & textInfo) : detail(textInfo) {}

        inline const ke::Keyboard::TextInfo & getDetail() const { return this->detail; }

        virtual ke::EventSptr makeCopy() const final
        {
            return ke::makeEvent<KeyboardTextEvent>(this->detail);
        }

    protected:
        ke::Keyboard::TextInfo detail;
    };


    class MouseButtonEvent : public ke::IEvent
    {
        KE_DEFINE_EVENT_COMMON_PROPERTIES(MouseButtonEvent, 0xA16F2D57)

    public:
        using ScreenPositionType = Point2DInt32;
        using WorldPositionType = Point2DFloat;

        MouseButtonEvent(const ke::Mouse::ButtonInfo & eventInfo) : detail(eventInfo) {}

        inline void setDetail(const ke::Mouse::ButtonInfo & eventInfo) { this->detail = eventInfo; }
        inline const ke::Mouse::ButtonInfo & getDetail() const { return this->detail; }

        inline const ScreenPositionType & getScreenPosition() const { return this->detail.screenPosition; }
        inline const WorldPositionType & getWorldnPosition() const { return this->detail.worldPosition; }

    protected:
        ke::Mouse::ButtonInfo detail;
    };

    class MouseButtonPressedEvent : public MouseButtonEvent
    {
        KE_DEFINE_EVENT_COMMON_PROPERTIES(MouseButtonPressedEvent, 0x0F049484)

    public:
        using MouseButtonEvent::MouseButtonEvent;

        virtual ke::EventSptr makeCopy() const final
        {
            return ke::makeEvent<MouseButtonPressedEvent>(this->detail);
        }
    };

    class MouseButtonReleasedEvent : public MouseButtonEvent
    {
        KE_DEFINE_EVENT_COMMON_PROPERTIES(MouseButtonReleasedEvent, 0x44636E5A)

    public:
        using MouseButtonEvent::MouseButtonEvent;

        virtual ke::EventSptr makeCopy() const final
        {
            return ke::makeEvent<MouseButtonReleasedEvent>(this->detail);
        }
    };

    class MouseWheelScrolledEvent : public IEvent
    {
        KE_DEFINE_EVENT_COMMON_PROPERTIES(MouseWheelScrolledEvent, 0x5b2703ac)

    public:
        MouseWheelScrolledEvent(const ke::Mouse::ScrollWheelInfo & eventInfo) : detail(eventInfo) {}

        inline void setDetail(const ke::Mouse::ScrollWheelInfo & eventInfo) { this->detail = eventInfo; }
        inline const ke::Mouse::ScrollWheelInfo & getDetail() const { return this->detail; }

        inline const decltype(ke::Mouse::ScrollWheelInfo::screenPosition) & getScreenPosition() const { return this->detail.screenPosition; }
        inline const decltype(ke::Mouse::ScrollWheelInfo::worldPosition) & getWorldnPosition() const { return this->detail.worldPosition; }

        virtual ke::EventSptr makeCopy() const final
        {
            return ke::makeEvent<MouseWheelScrolledEvent>(this->detail);
        }

    protected:
        ke::Mouse::ScrollWheelInfo detail;
    };


    class MouseMovedEvent : public IEvent
    {
        KE_DEFINE_EVENT_COMMON_PROPERTIES(MouseMovedEvent, 0x99A78BB9)

    public:
        MouseMovedEvent(const ke::Mouse::MouseInfo & eventInfo) : detail(eventInfo) {}

        inline void setDetail(const ke::Mouse::MouseInfo & eventInfo) { this->detail = eventInfo; }
        inline const ke::Mouse::MouseInfo & getDetail() const { return this->detail; }

        inline const decltype(ke::Mouse::MouseInfo::screenPosition) & getScreenPosition() const { return this->detail.screenPosition; }
        inline const decltype(ke::Mouse::MouseInfo::worldPosition) & getWorldnPosition() const { return this->detail.worldPosition; }

        virtual ke::EventSptr makeCopy() const final
        {
            return ke::makeEvent<MouseMovedEvent>(this->detail);
        }

    protected:
        ke::Mouse::MouseInfo detail;
    };


    class MouseEnteredEvent : public IEvent
    {
        KE_DEFINE_EVENT_COMMON_PROPERTIES(MouseEnteredEvent, 0xC854B15A)

    public:
        MouseEnteredEvent() {}

        virtual ke::EventSptr makeCopy() const final
        {
            return ke::makeEvent<MouseEnteredEvent>();
        }
    };


    class MouseLeftEvent : public IEvent
    {
        KE_DEFINE_EVENT_COMMON_PROPERTIES(MouseLeftEvent, 0x839746FF)

    public:
        MouseLeftEvent() {}

        virtual ke::EventSptr makeCopy() const final
        {
            return ke::makeEvent<MouseLeftEvent>();
        }
    };


    class WindowCreatedEvent : public ke::IEvent
    {
        KE_DEFINE_EVENT_COMMON_PROPERTIES(WindowCreatedEvent, 0xD48FCEDF)

    public:
        WindowCreatedEvent(ke::IWindow * newWindow) : window(newWindow) {}

        inline ke::IWindow * get() const { return this->window; }

        virtual ke::EventSptr makeCopy() const final
        {
            return ke::makeEvent<WindowCreatedEvent>(this->window);
        }

    private:
        ke::IWindow * window;
    };


    class WindowDestroyedEvent : public ke::IEvent
    {
        KE_DEFINE_EVENT_COMMON_PROPERTIES(WindowDestroyedEvent, 0xAC768EC4)

    public:
        WindowDestroyedEvent(ke::IWindow * p_window) : window(p_window) {}

        inline ke::IWindow * get() const { return this->window; }

        virtual ke::EventSptr makeCopy() const final
        {
            return ke::makeEvent<WindowDestroyedEvent>(this->window);
        }

    private:
        ke::IWindow * window;
    };


    class WindowResizedEvent : public ke::IEvent
    {
        KE_DEFINE_EVENT_COMMON_PROPERTIES(WindowResizedEvent, 0x1CFE21D7)

    public:
        WindowResizedEvent(const ke::Dimension2DUInt32 & p_newSize)
            : newSize(p_newSize)
        {}

        inline const ke::Dimension2DUInt32 & getNewSize() const { return this->newSize; }

        virtual ke::EventSptr makeCopy() const final
        {
            return ke::makeEvent<WindowResizedEvent>(this->newSize);
        }

    private:
        ke::Dimension2DUInt32 newSize;

    };


    class WindowFocusGainedEvent : public ke::IEvent
    {
        KE_DEFINE_EVENT_COMMON_PROPERTIES(WindowFocusGainedEvent, 0xb71bddfc)

    public:
        WindowFocusGainedEvent()
        {}

        virtual ke::EventSptr makeCopy() const final
        {
            return ke::makeEvent<WindowFocusGainedEvent>();
        }

    private:

    };


    class WindowFocusLostEvent : public ke::IEvent
    {
        KE_DEFINE_EVENT_COMMON_PROPERTIES(WindowFocusLostEvent, 0x25eb40d9)

    public:
        WindowFocusLostEvent()
        {}

        virtual ke::EventSptr makeCopy() const final
        {
            return ke::makeEvent<WindowFocusLostEvent>();
        }

    private:

    };

}