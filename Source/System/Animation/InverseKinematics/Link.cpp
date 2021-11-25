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
        if (IsEndEffector())
        {
            //end effector
            m_to_child.SetZero();
            m_angle  = 0.0f;
            m_length = 0.0f;
        }
        else
        {
            m_to_child       = m_child->m_origin - m_origin;
            m_angle          = BetweenAngle(m_basis, m_to_child);
            m_length         = m_to_child.Length();
            m_child->m_basis = m_to_child.Unit();

            m_drawing_cone.radius = 0.2f;
            m_drawing_cone.height = m_length;

            m_child->SetUpData();
        }
    }

    void Link::SetAngle(Real angle, bool b_recursive)
    {
        m_angle = angle;
        UpdateAngle(b_recursive);
    }

    void Link::UpdateAngle(bool b_recursive)
    {
        if (m_child != nullptr)
        {
            Quaternion rotation(AxisRadian(m_rotation_axis, m_angle));
            m_to_child        = rotation.Rotate(m_basis) * m_length;
            m_child->m_origin = m_to_child + m_origin;
            m_child->m_basis  = m_to_child.Unit();

            if (b_recursive)
            {
                m_child->UpdateAngle(b_recursive);
            }
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
            Quaternion rotation(Vector3(0, 1, 0), m_child->m_basis);
            Vector3    position = m_origin + m_child->m_basis * m_length * 0.5f;

            renderer->DrawPrimitiveInstancing(m_drawing_cone, rotation, position, eRenderingMode::Face, color);

            renderer->DrawSegment(m_origin, m_child->m_origin, color);
        }
    }

    void Link::AddTranslation(const Vector3& translation)
    {
        m_origin += translation;
        if (m_child)
        {
            m_child->AddTranslation(translation);
        }
    }
}
