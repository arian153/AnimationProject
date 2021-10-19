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
        elapsed_time += speed * dt;

        if (elapsed_time > duration)
        {
            elapsed_time = 0.0f;
            ResetTracks();
        }

        UpdateTracks();
    }

    void AnimationClip::UpdateTracks()
    {
        for (size_t i = 0; i < bone_count; ++i)
        {
            tracks[i].Update(elapsed_time, key_frames[i]);
        }
        //find final result & matrix
        skeleton->UpdateKeyFrame(key_frames);
    }

    void AnimationClip::ResetTracks()
    {
        for(size_t i = 0; i < bone_count; ++i)
        {
            tracks[i].current_idx = 0;
        }
    }

    void AnimationClip::Shutdown()
    {
        tracks.clear();
    }
}
