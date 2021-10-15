#pragma once
#include <vector>
#include "../../Math/Structure/VQSTransform.hpp"

namespace CS460
{
    class KeyFrame
    {
    public:
        KeyFrame();
        ~KeyFrame();

        VQSTransform ToParent(size_t bone_idx);

    public:
        //to parent transform per bone
        std::vector<VQSTransform> to_parents;
        Real                      time = -1.0f;
    };
}
