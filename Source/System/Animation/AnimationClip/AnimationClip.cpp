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
        track.clear();
    }

    KeyFrame AnimationClip::Interpolate(Real t)
    {
        return KeyFrame();
    }
}
