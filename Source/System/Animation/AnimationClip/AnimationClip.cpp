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
        //elapsed time for animation track
        elapsed_time += speed * dt;
        if (elapsed_time > duration)
        {
            //if finished, reset timer for animation cycle
            ResetTracks();
        }

        //update track
        UpdateTracks();
    }

    void AnimationClip::UpdateTracks()
    {
        for (size_t i = 0; i < bone_count; ++i)
        {
            tracks[i].Update(elapsed_time, key_frames[i]);
        }
        //apply final result
        skeleton->UpdateKeyFrame(key_frames);
    }

    void AnimationClip::ResetTracks()
    {
        elapsed_time = 0.0f;
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
