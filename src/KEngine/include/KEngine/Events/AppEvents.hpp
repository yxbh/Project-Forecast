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
        inline const ke::Keyboard::KeyInfo & getDetail() const { return this->detail; }

    protected:
        ke::Keyboard::KeyInfo detail;
    };
    
    class KeyPressedEvent : public KeyboardEvent
    {
        KE_DEFINE_EVENT_COMMON_PROPERTIES(KeyPressedEvent, 0x8B492E75)

    public:
    };

    class KeyReleasedEvent : public KeyboardEvent
    {
        KE_DEFINE_EVENT_COMMON_PROPERTIES(KeyReleasedEvent, 0xABCAB2CA)

    public:
    };


    struct MouseEventInfo
    {
        Point2DInt32 screenPosition;
        Point2DFloat worldPosition;
    };

    class MouseEvent : public ke::IEvent
    {
        KE_DEFINE_EVENT_COMMON_PROPERTIES(KeyReleasedEvent, 0xA16F2D57)

    public:
        using ScreenPositionType = Point2DInt32;
        using WorldPositionType = Point2DFloat;

        MouseEvent(const MouseEventInfo & eventInfo) : detail(eventInfo) {}

        inline void setDetail(const MouseEventInfo & eventInfo) { this->detail = eventInfo; }
        inline const MouseEventInfo & getDetail() const { return this->detail; }

        inline const ScreenPositionType & getScreenPosition() const { return this->detail.screenPosition; }
        inline const WorldPositionType & getWorldnPosition() const { return this->detail.worldPosition; }

    protected:
        MouseEventInfo detail;
    };

    class MouseButtonPressedEvent : public MouseEvent
    {
        KE_DEFINE_EVENT_COMMON_PROPERTIES(MouseButtonPressedEvent, 0x0F049484)

    public:
        using MouseEvent::MouseEvent;

        virtual ke::EventSptr makeCopy() const final
        {
            return ke::makeEvent<MouseButtonPressedEvent>(this->detail);
        }
    };

    class MouseButtonReleasedEvent : public MouseEvent
    {
        KE_DEFINE_EVENT_COMMON_PROPERTIES(MouseButtonReleasedEvent, 0x44636E5A)

    public:
        using MouseEvent::MouseEvent;

        virtual ke::EventSptr makeCopy() const final
        {
            return ke::makeEvent<MouseButtonReleasedEvent>(this->detail);
        }
    };



}