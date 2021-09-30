#pragma once
#include <vector>

#include "../../Math/Utility/MathDef.hpp"
#include "../AnimationClip/AnimationClip.hpp"

namespace CS460
{
    class Bone;

    class Skeleton
    {
    public:
        Skeleton();
        ~Skeleton();

        void Initialize();
        void Update(Real dt);
        void Draw();
        void Shutdown();

        void CreateSample();

    private:
        Bone*                      m_root_bone = nullptr;
        std::vector<AnimationClip> m_animation_clips;
        std::vector<Bone*>         m_bones;

        I64 m_clip_id = Core::I64_MAX;

    };
}
