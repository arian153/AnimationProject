#pragma once
#include <vector>

#include "../../Math/Structure/VQSTransform.hpp"

namespace CS460
{
    class Bone
    {
    public:
        Bone();
        ~Bone();

        void Update(Real dt);

    private:
        VQSTransform       transform;

        Bone*              parent = nullptr;
        std::vector<Bone*> children;
    };
}
