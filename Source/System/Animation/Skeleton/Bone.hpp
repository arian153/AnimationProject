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

        void Draw(PrimitiveRenderer* renderer, const Color& color) const;
        void Draw(PrimitiveRenderer* renderer, const Color& color, const VQSTransform& parent) const;
        void Push(Bone* child);

    private:
        friend class Skeleton;

    private:
        I32          m_parent_idx = -1;
        VQSTransform m_to_root;
        VQSTransform m_to_bone;

        Skeleton*    m_skeleton   = nullptr;
       
      
        std::string  m_name = "";

        Bone* m_parent = nullptr;

        I32 m_own_idx = -1;

        std::vector<I32>   m_sib_idx;
        std::vector<I32>   m_chi_idx;
        std::vector<Bone*> m_sibling;
        std::vector<Bone*> m_children;
    };
}
