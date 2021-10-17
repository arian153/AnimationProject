#include "AnimationClip.hpp"

#include "../Skeleton/Skeleton.hpp"

namespace CS460
{
    AnimationClip::AnimationClip()
    {
    }

    AnimationClip::~AnimationClip()
    {
    }

    void AnimationClip::Update(Real dt)
    {
        for (size_t i = 0; i < bone_count; ++i)
        {
            tracks[i].Update(speed * dt, key_frames[i]);
        }
        //find final result & matrix
        skeleton->UpdateKeyFrame(key_frames);
    }

    void AnimationClip::Shutdown()
    {
        tracks.clear();
    }
}
