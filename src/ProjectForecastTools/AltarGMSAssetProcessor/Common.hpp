#pragma once

#include <QList>

namespace ke
{

    struct Position
    {
        unsigned x = 0;
        unsigned y = 0;
    };

    struct Dimension2D
    {
        unsigned width = 0;
        unsigned height = 0;
    };

    struct BoundingBox2D
    {
        unsigned top    = 0;
        unsigned left   = 0;
        unsigned bottom = 0;
        unsigned right  = 0;
    };

    using SpriteId = unsigned;
    using TexpageId = unsigned;
    using TextureId = unsigned;

}
