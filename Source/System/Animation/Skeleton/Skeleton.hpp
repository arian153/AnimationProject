#pragma once
#include <vector>

#include "AnimationClip.hpp"
#include "../../Math/Utility/MathDef.hpp"

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

    private:
        Bone*                      m_root_bone = nullptr;
        std::vector<AnimationClip> m_animation_clips;
        std::vector<Bone*>         m_bones;
    };
}
