#include "PointConstraint.hpp"
#include "../../../Core/Utility/CoreUtility.hpp"
#include "../../Utility/ConstraintUtility.hpp"
#include "../../Dynamics/RigidBody.hpp"
#include "../../../Math/Algebra/Matrix33.hpp"
#include "../../../../Manager/Component/EngineComponent/PointConstraintComponent.hpp"

namespace CS460
{
    PointConstraint::PointConstraint(RigidBody* body, ConstraintUtility* utility)
        : m_constraint_utility(utility), m_body(body)
    {
        m_target = &m_local_anchor;
    }

    PointConstraint::~PointConstraint()
    {
    }

    void PointConstraint::Shutdown()
    {
        //disconnect rigid body
        if (m_component != nullptr)
        {
            m_component->m_point_constraint = nullptr;
        }
    }

    void PointConstraint::GenerateVelocityConstraints(Real dt)
    {
        m_bias        = m_constraint_utility->GenerateConstraintBias(m_body->InverseMass(), m_frequency, m_damping_ratio, m_mode, dt);
        m_r           = m_body->LocalToWorldVector(m_local_anchor - m_body->GetLocalCentroid());
        Vector3 c_pos = (m_body->GetCentroid() + m_r) - *m_target;
        Vector3 c_vel = m_body->GetLinearVelocity() + CrossProduct(m_body->GetAngularVelocity(), m_r);
        m_cross.SetSkew(-m_r);
        Matrix33 k = m_body->InverseMassMatrix();
        if (m_b_rotation)
        {
            k += m_cross * m_body->InverseInertia() * m_cross.Transpose();
        }
        k += m_bias.softness_bias * Matrix33();
        m_position_error_bias = m_bias.position_bias * c_pos;
        m_effective_mass      = k.Inverse();
        m_total_lambda.SetZero();
    }

    void PointConstraint::SolveVelocityConstraints(Real dt)
    {
        E5_UNUSED_PARAM(dt);
        Vector3 v      = m_body->GetLinearVelocity();
        Vector3 w      = m_body->GetAngularVelocity();
        Vector3 c_vel  = v + CrossProduct(w, m_r);
        Vector3 jvb    = c_vel + m_position_error_bias + m_bias.softness_bias * m_total_lambda;
        Vector3 lambda = m_effective_mass * (-jvb);
        m_total_lambda += lambda;
        v += m_body->InverseMass() * lambda;
        m_body->SetLinearVelocity(v);
        if (m_b_rotation)
        {
            w += m_body->InverseInertia() * m_cross.Transpose() * lambda;
            m_body->SetAngularVelocity(w);
        }
    }

    void PointConstraint::ApplyVelocityConstraints()
    {
    }

    void PointConstraint::GeneratePositionConstraints(Real dt)
    {
        E5_UNUSED_PARAM(dt);
    }

    void PointConstraint::SolvePositionConstraints(Real dt)
    {
        E5_UNUSED_PARAM(dt);
    }

    void PointConstraint::ApplyPositionConstraints()
    {
    }

    void PointConstraint::SetConstraintMode(eConstraintMode mode)
    {
        m_mode = mode;
    }

    void PointConstraint::SetFrequency(Real frequency)
    {
        m_frequency = Math::Clamp(frequency, 0.001f, 10.0f);
    }

    void PointConstraint::SetDampingRatio(Real damping_ratio)
    {
        m_damping_ratio = Math::Clamp(damping_ratio, 0.0f, 10.0f);;
    }

    void PointConstraint::EnableRotation(bool b_rotation)
    {
        m_b_rotation = b_rotation;
    }

    void PointConstraint::SetTargetPoint(const Vector3& target)
    {
        m_local_target = target;
    }

    void PointConstraint::SetAnchorPoint(const Vector3& anchor)
    {
        m_local_anchor = anchor;
    }
}
