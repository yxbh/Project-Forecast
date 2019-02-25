#pragma once

#include "KEngine/Common/String.hpp"
#include "KEngine/Common/Dimension2D.hpp"

#include <memory>

namespace ke
{
    /// <summary>
    /// Interface for implementing application display windows.
    /// </summary>
    class IWindow
    {
    public:
        IWindow() = default;
        IWindow(IWindow&) = delete;
        IWindow(IWindow&&) = delete;
        virtual ~IWindow() = 0;
        IWindow & operator=(IWindow &) = delete;
        IWindow & operator=(IWindow &&) = delete;

        /// <summary>
        /// Refresh the window surface to display new content.
        /// </summary>
        virtual void display() = 0;

        /// <summary>
        /// Set the window title.
        /// </summary>
        /// <param name="title">Title text</param>
        virtual void setTitle(const ke::String & title) = 0;

        virtual bool setThreadCurrent(bool threadCurrent) = 0;

        /// <summary>
        /// Get the native handle or underlying window API object.
        /// </summary>
        /// <returns></returns>
        virtual void * get() = 0;

        virtual bool hasFocus() const = 0;

        inline bool isInFocus() const { return this->hasFocus(); }

        virtual void requestFocus() = 0;

        virtual ke::Dimension2DUInt32 getDimension() const = 0;

        virtual std::uint32_t getWidth() const = 0;
        virtual std::uint32_t getHeight() const = 0;
        virtual int getPositionX() const = 0;
        virtual int getPositionY() const = 0;
    };

    inline IWindow::~IWindow() {}
    
    using WindowUptr = std::unique_ptr<IWindow>;
    using WindowSptr = std::shared_ptr<IWindow>;
    using WindowWptr = std::weak_ptr<IWindow>;

}