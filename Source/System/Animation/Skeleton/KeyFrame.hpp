#pragma once
#include <vector>
#include "BoneFrame.hpp"

namespace CS460
{
   

    class KeyFrame
    {
    public:
        KeyFrame();
        ~KeyFrame();

    private:
        std::vector<BoneFrame> frame_data;
    };
}
