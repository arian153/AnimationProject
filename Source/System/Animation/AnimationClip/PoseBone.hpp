#pragma once
#include "../../Core/Utility/CoreDef.hpp"
#include "../../Math/Structure/VQSTransform.hpp"
#include "../../Math/Utility/MathDef.hpp"

namespace CS460
{
    class PoseBone
    {
    public:
        PoseBone();
        ~PoseBone();

    public:
        I32          frame = -1;
        Real         time  = 0.0f;
        VQSTransform to_parent;
    };
}
