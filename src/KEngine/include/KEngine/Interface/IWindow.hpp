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
        virtual ~IWindow() = 0;

        /// <summary>
        /// Refresh the window surface to display new content.
        /// </summary>
        virtual void display() = 0;

        /// <summary>
        /// Set the window title.
        /// </summary>
        /// <param name="title">Title text</param>
        virtual void setTitle(ke::String title) = 0;
    };

    inline IWindow::~IWindow() {}
    
    using WindowUptr = std::unique_ptr<IWindow>;
    using WindowSptr = std::shared_ptr<IWindow>;
    using WindowWptr = std::weak_ptr<IWindow>;

}