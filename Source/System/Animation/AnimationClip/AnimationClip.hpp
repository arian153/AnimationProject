#pragma once
#include <vector>

#include "KeyFrame.hpp"
#include "../../Math/Utility/MathDef.hpp"

namespace CS460
{
    class Skeleton;
    class Bone;

    class AnimationClip
    {
    public:
        AnimationClip();
        ~AnimationClip();

        void Update(Real dt);
        void Shutdown();
        void Interpolate(Real t, const KeyFrame& start, const KeyFrame& end, KeyFrame& result) const;

    private:
        friend class Skeleton;
        friend class AniMeshResource;

    private:
        Skeleton*   skeleton = nullptr;
        std::string name;
        Real        duration       = -1.0f;
        Real        track_scale    = 1.0f;
        Real        track_position = 0.0f;
        size_t      bone_count     = 0;
        size_t      current_idx    = 0;
        size_t      track_size     = 0;

        std::vector<KeyFrame> track;
    };
}
