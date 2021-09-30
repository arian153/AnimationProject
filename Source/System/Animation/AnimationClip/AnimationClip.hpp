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

        void Update(Bone* root_bone, Real dt);

    private:
        std::string name;
        std::vector<KeyFrame> key_frames;

    };

}
