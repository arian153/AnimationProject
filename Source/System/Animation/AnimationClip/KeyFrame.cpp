#include "KeyFrame.hpp"

namespace CS460
{
    KeyFrame::KeyFrame()
    {
    }

    KeyFrame::~KeyFrame()
    {
    }

    VQSTransform KeyFrame::ToParent(size_t bone_idx)
    {
        return to_parents[bone_idx];
    }
}
