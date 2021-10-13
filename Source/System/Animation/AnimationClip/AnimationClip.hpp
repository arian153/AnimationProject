#pragma once
#include <vector>

#include "KeyFrame.hpp"
#include "../../Math/Utility/MathDef.hpp"

namespace CS460
{
    class Bone;

    class AnimationClip
    {
    public:
        AnimationClip();
        ~AnimationClip();

        void     Update(Bone* root_bone, Real dt);
        void     Shutdown();
        KeyFrame Interpolate(Real t);

    private:
        friend class Skeleton;
        friend class AniMeshResource;

    private:
        std::string           name;
        Real                  duration   = -1.0f;
        size_t                bone_count = 0;
        std::vector<KeyFrame> track;
    };
}
