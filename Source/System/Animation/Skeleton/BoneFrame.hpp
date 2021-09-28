#pragma once
#include "../../Core/Utility/CoreDef.hpp"
#include "../../Math/Structure/VQSTransform.hpp"
#include "../../Math/Utility/MathDef.hpp"

namespace CS460
{
    class BoneFrame
    {
    public:
        BoneFrame();
        ~BoneFrame();

    private:
        I64          frame = -1;
        Real         time  = 0.0f;
        VQSTransform transform;
    };

}
