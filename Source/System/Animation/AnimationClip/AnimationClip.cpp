#include "AnimationClip.hpp"

namespace CS460
{
    AnimationClip::AnimationClip()
    {
    }

    AnimationClip::~AnimationClip()
    {
    }

    void AnimationClip::Update(Bone* root_bone, Real dt)
    {
    }

    void AnimationClip::Shutdown()
    {
        key_frames.clear();
    }
}
