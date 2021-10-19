#pragma once
#include <vector>

#include "KeyFrame.hpp"
#include "Track.hpp"
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
        void UpdateTracks();
        void ResetTracks();
        void Shutdown();

    private:
        friend class Skeleton;
        friend class AniMeshResource;
        friend class AniMeshComponent;

    private:
        Skeleton*   skeleton = nullptr;
        std::string name;
        Real        duration = -1.0f;
        Real        speed    = 1.0f;
        Real        elapsed_time = 0.0f;
        size_t      bone_count = 0;

        std::vector<Track>    tracks;
        std::vector<KeyFrame> key_frames;
    };
}
