#include "Link.hpp"

#include "../../Graphics/Utility/PrimitiveRenderer.hpp"
#include "../../Math/Algebra/Quaternion.hpp"

namespace CS460
{
    Link::Link()
    {
    }

    Link::~Link()
    {
    }

    Link::Link(const Vector3& origin)
        : m_origin(origin)
    {
    }

    void Link::SetUpData()
    {
        if (m_child != nullptr)
        {
            m_to_child       = m_child->m_origin - m_origin;
            m_angle          = BetweenAngle(m_basis, m_to_child);
            m_child->m_basis = m_to_child.Unit();
            m_child->SetUpData();
        }
    }

    void Link::Forward(Real angle)
    {
        m_angle = angle;
        if (m_child != nullptr)
        {
            UpdateChild(Quaternion(AxisRadian(m_polar_axis, m_angle)));
        }
    }

    void Link::UpdateChild(const Quaternion& rotation)
    {
        if (m_child != nullptr)
        {
            m_to_child        = rotation.Rotate(m_to_child);
            m_child->m_origin = m_to_child + m_origin;
            m_child->m_basis  = m_to_child.Unit();
            m_child->UpdateChild(rotation);
        }
    }

    bool Link::IsEndEffector() const
    {
        return m_child == nullptr;
    }

    void Link::Draw(PrimitiveRenderer* renderer, const Color& color) const
    {
        if (m_child)
        {
            renderer->DrawSegment(m_origin, m_child->m_origin, color);
            m_child->Draw(renderer, color);
        }
    }
}
