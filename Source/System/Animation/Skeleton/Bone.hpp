#pragma once
#include <vector>

namespace CS460
{
    class Bone
    {
    public:
        Bone();
        ~Bone();

    private:
        Bone*              parent = nullptr;
        std::vector<Bone*> children;
    };
}
