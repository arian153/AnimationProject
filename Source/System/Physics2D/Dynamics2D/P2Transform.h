#pragma once
#include "../Math2D/P2Math.h"

namespace P2D
{
    class Transform
    {
    public:
        Transform();
        ~Transform();

    public:
        Vector2 position;
        Real    rotation = 0;
    };
}
