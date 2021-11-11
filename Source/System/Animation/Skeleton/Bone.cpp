#include "Bone.hpp"

#include "../../Graphics/Utility/PrimitiveRenderer.hpp"

namespace CS460
{
    Bone::Bone()
    {
    }

    Bone::~Bone()
    {
    }

    void Bone::Update(Real dt)
    {
    }

    void Bone::Shutdown()
    {
        m_parent     = nullptr;
        m_parent_idx = -1;
        m_own_idx    = -1;

        m_sib_idx.clear();
        m_chi_idx.clear();
        m_sibling.clear();
        m_children.clear();
    }

    void Bone::Push(Bone* child)
    {
        m_children.push_back(child);
        m_chi_idx.push_back(child->m_own_idx);
    }

    bool Bone::IsEndEffector() const
    {
        return m_children.empty();
    }

    Bone* Bone::GetParent() const
    {
        return m_parent;
    }
}
