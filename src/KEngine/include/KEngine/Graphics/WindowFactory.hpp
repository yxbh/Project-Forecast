#pragma once

#include "KEngine/Interfaces/IWindow.hpp"

#include <cstddef>

namespace ke
{
    
    class WindowFactory
    {
    public:
        /// <summary>
        /// Create and return a new application window.
        /// </summary>
        /// <param name="p_width"></param>
        /// <param name="p_height"></param>
        /// <param name="p_posX"></param>
        /// <param name="p_posY"></param>
        /// <returns></returns>
        static ke::WindowSptr newWindow(
            unsigned p_width, unsigned p_height,
            int p_posX, int p_posY,
            const ke::String & p_title);

    private:
        static ke::WindowSptr createNewSdl2Window(
            unsigned p_width, unsigned p_height,
            int p_posX, int p_posY,
            const ke::String & p_title);
        static ke::WindowSptr createNewSfmlWindow(
            unsigned p_width, unsigned p_height,
            int p_posX, int p_posY,
            const ke::String & p_title);

    };

}