#pragma once

#include "KEngine/Interface/IEvent.hpp"
#include "KEngine/Interface/IInputController.hpp"

#include "KEngine/Common/Point2D.hpp"

namespace ke
{

    class KeyboardEvent : public ke::IEvent
    {
        KE_DEFINE_EVENT_COMMON_PROPERTIES(KeyboardEvent, 0xE1D2E239)

    public:
        KeyboardEvent(const ke::Keyboard::KeyInfo & keyInfo) : detail(keyInfo) {}

        inline const ke::Keyboard::KeyInfo & getDetail() const { return this->detail; }

    protected:
        ke::Keyboard::KeyInfo detail;
    };
    
    class KeyboardKeyPressedEvent : public KeyboardEvent
    {
        KE_DEFINE_EVENT_COMMON_PROPERTIES(KeyboardKeyPressedEvent, 0x8B492E75)

    public:
        using KeyboardEvent::KeyboardEvent;

        virtual ke::EventSptr makeCopy() const final
        {
            return ke::makeEvent<KeyboardKeyPressedEvent>(this->detail);
        }
    };

    class KeyboardKeyReleasedEvent : public KeyboardEvent
    {
        KE_DEFINE_EVENT_COMMON_PROPERTIES(KeyboardKeyReleasedEvent, 0xABCAB2CA)

    public:
        using KeyboardEvent::KeyboardEvent;

        virtual ke::EventSptr makeCopy() const final
        {
            return ke::makeEvent<KeyboardKeyReleasedEvent>(this->detail);
        }
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



}