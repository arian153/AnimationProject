#pragma once
#include <vector>

#include "../../Core/Utility/CoreDef.hpp"
#include "../../Math/Structure/VQSTransform.hpp"

namespace CS460
{
    class Bone
    {
    public:
        Bone();
        ~Bone();

        void Update(Real dt);
        void Shutdown();

    private:
        VQSTransform offset_transform;
        std::string  name;

        Bone* parent = nullptr;
        I64   parent_idx = -1;
        I64   my_idx     = -1;

        std::vector<I64>   sibling_idx;
        std::vector<I64>   children_idx;
        std::vector<Bone*> sibling;
        std::vector<Bone*> children;
    };
}
