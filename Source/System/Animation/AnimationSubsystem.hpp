#pragma once
#include <vector>

#include "../Math/Utility/MathDef.hpp"

namespace CS460
{
    class AnimationSubsystem
    {
    public:
        AnimationSubsystem();
        ~AnimationSubsystem();

        void Initialize();
        void Update(Real dt);
        void Render() const;
        void Shutdown();


    private:
    };
}
