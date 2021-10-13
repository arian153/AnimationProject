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

    void Bone::Draw(PrimitiveRenderer* renderer, const Color& color) const
    {
        renderer->DrawPrimitive(Sphere(m_to_bone.position, m_to_bone.rotation, 0.05f), eRenderingMode::Face, color);
    }

    void Bone::Draw(PrimitiveRenderer* renderer, const Color& color, const VQSTransform& parent) const
    {
        VQSTransform offset = Multiply(parent, m_to_bone);
        renderer->DrawPrimitive(Sphere(offset.position, offset.rotation, 0.05f), eRenderingMode::Face, color);
    }

    void Bone::Push(Bone* child)
    {
        m_children.push_back(child);
        m_chi_idx.push_back(child->m_own_idx);
    }
}
