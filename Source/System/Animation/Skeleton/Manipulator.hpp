#pragma once
#include <vector>

namespace CS460
{
    class Bone;

    class Manipulator
    {
    public:
         Manipulator();
        ~Manipulator();

        void SetUpLinks(Bone* end_effector);

    public:
        std::vector<Bone*> links;
    };

    
}
