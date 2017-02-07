#pragma once

#include "KEngine/Common/String.hpp"

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

        virtual bool setThreadCurrent() = 0;

        /// <summary>
        /// Get the native handle or underlying window API object.
        /// </summary>
        /// <returns></returns>
        virtual void * get() = 0;
    };

    inline IWindow::~IWindow() {}
    
    using WindowUptr = std::unique_ptr<IWindow>;
    using WindowSptr = std::shared_ptr<IWindow>;
    using WindowWptr = std::weak_ptr<IWindow>;

}