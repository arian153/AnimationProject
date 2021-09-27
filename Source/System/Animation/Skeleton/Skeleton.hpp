#pragma once
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
        Bone* m_root_bone = nullptr;
    };
}
