#pragma once
#include <vector>
#include "PoseBone.hpp"

namespace CS460
{
    class KeyFrame
    {
    public:
        KeyFrame();
        ~KeyFrame();

    public:
        std::vector<PoseBone> pose;
    };
}
