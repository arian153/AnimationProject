#pragma once
#include <vector>
#include "../../Math/Structure/VQSTransform.hpp"

namespace CS460
{
    class KeyFrame
    {
    public:
        KeyFrame() = default;
        ~KeyFrame() = default;

    public:
        VQSTransform to_parent;
        Real         time = -1.0f;
    };
}
