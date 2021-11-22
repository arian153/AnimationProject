#pragma once
#include <vector>

#include "../../Core/Utility/CoreDef.hpp"
#include "../../Math/Structure/VQSTransform.hpp"

namespace CS460
{
    class Color;
    class PrimitiveRenderer;

    class Bone
    {
    public:
        Bone();
        ~Bone();

        void Update(Real dt);
        void Shutdown();
        void Push(Bone* child);

        bool  IsEndEffector() const;
        Bone* GetParent() const;

        Vector3 GetPosition() const;
    private:
        friend class Skeleton;

    private:
        Skeleton*   m_skeleton = nullptr;
        std::string m_name     = "";

        VQSTransform m_to_root;
        VQSTransform m_to_bone;

        Bone* m_parent     = nullptr;
        I32   m_parent_idx = -1;
        I32   m_own_idx    = -1;

        std::vector<I32>   m_sib_idx;
        std::vector<I32>   m_chi_idx;
        std::vector<Bone*> m_sibling;
        std::vector<Bone*> m_children;
    };
}
