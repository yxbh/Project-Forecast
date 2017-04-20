#pragma once

#include "KEngine/Interfaces/IEvent.hpp"
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

}