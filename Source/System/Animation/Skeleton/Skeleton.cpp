#include "Skeleton.hpp"

#include "Bone.hpp"

namespace CS460
{
    Skeleton::Skeleton()
    {
    }

    Skeleton::~Skeleton()
    {
    }

    void Skeleton::Initialize()
    {
    }

    void Skeleton::Update(Real dt)
    {
        //update by keyframe

    }

    void Skeleton::Draw()
    {
    }

    void Skeleton::Shutdown()
    {
        for(auto& bone : m_bones)
        {
            bone->Shutdown();
            delete bone;
        }
        m_bones.clear();

    }
}
