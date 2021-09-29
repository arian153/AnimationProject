#pragma once
#include <vector>
#include "BoneData.hpp"

namespace CS460
{
    class KeyFrame
    {
    public:
        KeyFrame();
        ~KeyFrame();

    private:
        std::vector<BoneData> bone_data;
    };
}
