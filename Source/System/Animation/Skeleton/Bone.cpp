#include "Bone.hpp"

namespace CS460
{
    Bone::Bone()
    {
    }

    Bone::~Bone()
    {
    }

    void Bone::Update(Real dt)
    {
    }

    void Bone::Shutdown()
    {
        parent     = nullptr;
        parent_idx = -1;
        my_idx     = -1;

        sibling_idx.clear();
        children_idx.clear();
        sibling.clear();
        children.clear();
    }
}
