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
        if (m_clip_id != Core::I64_MAX)
        {
            auto& clip = m_animation_clips[m_clip_id];
            clip.Update(m_root_bone, dt);
        }
    }

    void Skeleton::Draw()
    {
    }

    void Skeleton::Shutdown()
    {
        for (auto& bone : m_bones)
        {
            bone->Shutdown();
            delete bone;
        }
        m_bones.clear();
    }

    void Skeleton::CreateSample()
    {
        Bone* bone  = new Bone();
        m_root_bone = bone;
    }
}
