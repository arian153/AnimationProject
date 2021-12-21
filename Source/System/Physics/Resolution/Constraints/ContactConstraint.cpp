#include "ContactConstraint.hpp"
#include "../../Dynamics/RigidBody.hpp"
#include "../Contact/ContactManifold.hpp"
#include "../Contact/ContactPoint.hpp"
#include "../../ColliderPrimitive/ColliderPrimitive.hpp"
#include "../../../Core/Utility/CoreUtility.hpp"
#include "../../../Graphics/DataType/Color.hpp"
#include "../../../Graphics/Utility/PrimitiveRenderer.hpp"

namespace CS460
{
    ContactConstraint::ContactConstraint(ContactManifold* input, FrictionUtility* friction_utility, bool enable_baum, Real tangent_speed)
        : m_friction_utility(friction_utility), m_manifold(input), m_motion_a(), m_motion_b(), m_b_enable_baumgarte(enable_baum), m_tangent_speed(tangent_speed)
    {
        m_b_ghost_constraints = input->m_set_a->IsGhost() || input->m_set_b->IsGhost();
    }

    ContactConstraint::~ContactConstraint()
    {
    }

    void ContactConstraint::Shutdown()
    {
    }

    void ContactConstraint::GenerateVelocityConstraints(Real dt)
    {
        if (m_b_ghost_constraints)
            return;
        m_body_a   = m_manifold->m_set_a->GetRigidBody();
        m_body_b   = m_manifold->m_set_b->GetRigidBody();
        m_motion_a = m_body_a->GetMotionMode();
        m_motion_b = m_body_b->GetMotionMode();
        //set mass
        m_mass_term.m_a = m_body_a->InverseMass();
        m_mass_term.i_a = m_body_a->InverseInertia();
        m_mass_term.m_b = m_body_b->InverseMass();
        m_mass_term.i_b = m_body_b->InverseInertia();
        //velocity term
        m_velocity_term.v_a = m_body_a->GetLinearVelocity();
        m_velocity_term.w_a = m_body_a->GetAngularVelocity();
        m_velocity_term.v_b = m_body_b->GetLinearVelocity();
        m_velocity_term.w_b = m_body_b->GetAngularVelocity();

        if (!m_velocity_term.v_a.IsValid())
        {
            m_velocity_term.v_a.SetZero();
        }
        if (!m_velocity_term.w_a.IsValid())
        {
            m_velocity_term.w_a.SetZero();
        }
        if (!m_velocity_term.v_b.IsValid())
        {
            m_velocity_term.v_b.SetZero();
        }
        if (!m_velocity_term.w_b.IsValid())
        {
            m_velocity_term.w_b.SetZero();
        }

        m_count = m_manifold->contacts.size();
        Basis basis;
        for (size_t i = 0; i < m_count; ++i)
        {
            basis.CalculateBasisApprox(m_manifold->contacts[i].normal);
            m_manifold->contacts[i].r_a = m_manifold->contacts[i].global_position_a - m_body_a->GetCentroid();
            m_manifold->contacts[i].r_b = m_manifold->contacts[i].global_position_b - m_body_b->GetCentroid();
            InitializeJacobian(m_manifold->contacts[i], basis.i, m_normal[i]);
            InitializeJacobian(m_manifold->contacts[i], basis.j, m_tangent[i]);
            InitializeJacobian(m_manifold->contacts[i], basis.k, m_bitangent[i]);
        }
    }

    void ContactConstraint::SolveVelocityConstraints(Real dt)
    {
        if (m_b_ghost_constraints)
            return;
        for (size_t i = 0; i < m_count; ++i)
        {
            SolveJacobian(m_manifold->contacts[i], m_tangent[i], i, dt);
            SolveJacobian(m_manifold->contacts[i], m_bitangent[i], i, dt);
        }
        for (size_t i = 0; i < m_count; ++i)
        {
            // Solve normal constraints
            SolveJacobian(m_manifold->contacts[i], m_normal[i], i, dt, true);
        }
    }

    void ContactConstraint::ApplyVelocityConstraints()
    {
        if (m_b_ghost_constraints)
            return;
        for (size_t i = 0; i < m_count; ++i)
        {
            m_manifold->contacts[i].tangent_lambda   = m_tangent[i].total_lambda;
            m_manifold->contacts[i].bitangent_lambda = m_bitangent[i].total_lambda;
            m_manifold->contacts[i].normal_lambda    = m_normal[i].total_lambda;
        }
        if (m_body_a->GetMotionMode() == eMotionMode::Dynamic)
        {
            m_body_a->SetLinearVelocity(m_velocity_term.v_a);
            m_body_a->SetAngularVelocity(m_velocity_term.w_a);
        }
        if (m_body_b->GetMotionMode() == eMotionMode::Dynamic)
        {
            m_body_b->SetLinearVelocity(m_velocity_term.v_b);
            m_body_b->SetAngularVelocity(m_velocity_term.w_b);
        }
    }

    void ContactConstraint::GeneratePositionConstraints(Real dt)
    {
        if (m_b_ghost_constraints)
            return;
        E5_UNUSED_PARAM(dt);
        m_position_term.p_a = m_body_a->GetCentroid();
        m_position_term.p_b = m_body_b->GetCentroid();
    }

    void ContactConstraint::SolvePositionConstraints(Real dt)
    {
        if (m_b_ghost_constraints)
            return;
        bool motion_a = m_body_a->GetMotionMode() == eMotionMode::Dynamic;
        bool motion_b = m_body_b->GetMotionMode() == eMotionMode::Dynamic;
        for (auto& contact : m_manifold->contacts)
        {
            Vector3 local_to_global_a = m_body_a->LocalToWorldPoint(contact.collider_a->LocalToWorldPoint(contact.local_position_a));
            Vector3 local_to_global_b = m_body_b->LocalToWorldPoint(contact.collider_b->LocalToWorldPoint(contact.local_position_b));
            //contact.depth = DotProduct(local_to_global_a - local_to_global_b, contact.normal);
            Real    separation  = DotProduct(local_to_global_b - local_to_global_a, contact.normal) - Physics::Collision::POSITION_SEPARATION_SLOP;
            Real    constraints = Math::Clamp(Physics::Dynamics::BAUMGRATE * (separation + Physics::Collision::POSITION_LINEAR_SLOP), -Physics::Collision::MAX_LINEAR_CORRECTION, 0.0f);
            Vector3 ra_n        = CrossProduct(local_to_global_a - m_position_term.p_a, contact.normal);
            Vector3 rb_n        = CrossProduct(local_to_global_b - m_position_term.p_b, contact.normal);
            Real    k
                    = (motion_a ? m_mass_term.m_a + ra_n * m_mass_term.i_a * ra_n : 0.0f)
                    + (motion_b ? m_mass_term.m_b + rb_n * m_mass_term.i_b * rb_n : 0.0f);
            Real    impulse = k > 0.0f ? -constraints / k : 0.0f;
            Vector3 p       = impulse * contact.normal;
            m_position_term.p_a -= m_mass_term.m_a * p;
            m_position_term.p_b += m_mass_term.m_b * p;
        }
    }

    void ContactConstraint::ApplyPositionConstraints()
    {
        if (m_b_ghost_constraints)
            return;
        m_body_a->SetCentroid(m_position_term.p_a);
        m_body_b->SetCentroid(m_position_term.p_b);
    }

    void ContactConstraint::Render(PrimitiveRenderer* primitive_renderer, const Color& color) const
    {
        if (m_b_ghost_constraints)
            return;
        Quaternion no_rotation;
        for (auto& contact_point : m_manifold->contacts)
        {
            Vector3 pos_a = contact_point.global_position_a;
            Vector3 pos_b = contact_point.global_position_b;
            primitive_renderer->DrawPrimitive(Sphere(pos_a, no_rotation, 0.05f), eRenderingMode::Face, color);
            primitive_renderer->DrawPrimitive(Sphere(pos_b, no_rotation, 0.05f), eRenderingMode::Face, color);
            primitive_renderer->DrawSegment(pos_a, pos_a - contact_point.normal * contact_point.depth, color);
        }

        size_t contact_size = m_manifold->contacts.size();
        if (contact_size == 3)
        {
            primitive_renderer->DrawTriangle(
                                             m_manifold->contacts[0].global_position_a,
                                             m_manifold->contacts[1].global_position_a,
                                             m_manifold->contacts[2].global_position_a,
                                             eRenderingMode::Face, color);
        }
        if (contact_size == 4)
        {
            primitive_renderer->DrawRectangle(
                                              m_manifold->contacts[0].global_position_a,
                                              m_manifold->contacts[1].global_position_a,
                                              m_manifold->contacts[2].global_position_a,
                                              m_manifold->contacts[3].global_position_a,
                                              eRenderingMode::Face, color);
        }
    }

    void ContactConstraint::WarmStart()
    {
        if (m_b_ghost_constraints)
            return;
        //apply previous data
        Basis normal_basis;
        for (size_t i = 0; i < m_count; ++i)
        {
            ContactPoint& contact_point = m_manifold->contacts[i];
            normal_basis.CalculateBasisApprox(contact_point.normal);
            Vector3 p = contact_point.normal_lambda * normal_basis.i
                    + contact_point.tangent_lambda * normal_basis.j
                    + contact_point.bitangent_lambda * normal_basis.k;
            m_velocity_term.v_a -= m_mass_term.m_a * p * 0.8f;
            m_velocity_term.w_a -= m_mass_term.i_a * CrossProduct(contact_point.r_a, p) * 0.8f;
            m_velocity_term.v_b += m_mass_term.m_b * p * 0.8f;
            m_velocity_term.w_b += m_mass_term.i_b * CrossProduct(contact_point.r_b, p) * 0.8f;

            m_normal[i].total_lambda    = contact_point.normal_lambda * 0.8f;
            m_tangent[i].total_lambda   = contact_point.tangent_lambda * 0.8f;
            m_bitangent[i].total_lambda = contact_point.bitangent_lambda * 0.8f;
        }
    }

    Real ContactConstraint::GetRestitution(ColliderPrimitive* a, ColliderPrimitive* b) const
    {
        return Math::Min(a->GetMaterial().restitution, b->GetMaterial().restitution);
    }

    void ContactConstraint::InitializeJacobian(const ContactPoint& contact, const Vector3& direction, Jacobian& jacobian) const
    {
        jacobian.v_a  = -direction;
        jacobian.w_a  = -CrossProduct(contact.r_a, direction);
        jacobian.v_b  = direction;
        jacobian.w_b  = CrossProduct(contact.r_b, direction);
        jacobian.bias = 0.0f;

        bool motion_a = m_body_a->GetMotionMode() == eMotionMode::Dynamic;
        bool motion_b = m_body_b->GetMotionMode() == eMotionMode::Dynamic;
        Real k
                = (motion_a ? m_mass_term.m_a + jacobian.w_a * m_mass_term.i_a * jacobian.w_a : 0.0f)
                + (motion_b ? m_mass_term.m_b + jacobian.w_b * m_mass_term.i_b * jacobian.w_b : 0.0f);
        jacobian.effective_mass = k > 0.0f ? 1.0f / k : 0.0f;;
        jacobian.total_lambda   = 0.0f;
    }

    void ContactConstraint::SolveJacobian(const ContactPoint& contact, Jacobian& jacobian, size_t i, Real dt, bool b_normal)
    {
        Vector3 dir = jacobian.v_b;
        // jv = Jacobian * velocity vector
        Real jv =
                DotProduct(jacobian.v_a, m_velocity_term.v_a)
                + DotProduct(jacobian.w_a, m_velocity_term.w_a)
                + DotProduct(jacobian.v_b, m_velocity_term.v_b)
                + DotProduct(jacobian.w_b, m_velocity_term.w_b);

        if (b_normal)
        {
            Real    beta              = Physics::Collision::CONTACT_BETA;
            Real    restitution       = GetRestitution(contact.collider_a, contact.collider_b);
            Vector3 relative_velocity =
                    -m_velocity_term.v_a
                    - CrossProduct(m_velocity_term.w_a, contact.r_a)
                    + m_velocity_term.v_b
                    + CrossProduct(m_velocity_term.w_b, contact.r_b);
            Real closing_velocity = DotProduct(relative_velocity, dir);
            Real baumgarte_slop   = m_b_enable_baumgarte ? Math::Max(contact.depth - Physics::Collision::VELOCITY_SLOP, 0.0f) : contact.depth;
            jacobian.bias         = -(beta / (1.0f / 60.0f)) * baumgarte_slop + restitution * closing_velocity;
        }
        else
        {
            jacobian.bias = -m_tangent_speed;
        }

        // raw lambda
        Real lambda = jacobian.effective_mass * -(jv + jacobian.bias);
        // clamped lambda
        Real old_total_lambda = jacobian.total_lambda;

        if (b_normal)
        {
            //normal - contact resolution : lambda >= 0
            jacobian.total_lambda = Math::Max(0.0f, jacobian.total_lambda + lambda);
        }
        else
        {
            //tangent - friction : -max_friction <= lambda <= max_friction
            auto friction_data    = m_friction_utility->Find(contact.collider_a->GetMaterialCode(), contact.collider_b->GetMaterialCode());
            Real max_friction     = friction_data.dynamic_friction * m_normal[i].total_lambda;
            jacobian.total_lambda = Math::Clamp(jacobian.total_lambda + lambda, -max_friction, max_friction);
        }
        lambda = (jacobian.total_lambda - old_total_lambda);

        // velocity correction
        m_velocity_term.v_a += m_mass_term.m_a * jacobian.v_a * lambda;
        m_velocity_term.w_a += m_mass_term.i_a * jacobian.w_a * lambda;
        m_velocity_term.v_b += m_mass_term.m_b * jacobian.v_b * lambda;
        m_velocity_term.w_b += m_mass_term.i_b * jacobian.w_b * lambda;
    }

    void ContactConstraint::AwakeState() const
    {
        if (m_b_ghost_constraints)
            return;

        bool is_a_sleep = m_body_a->IsSleep();
        bool is_b_sleep = m_body_b->IsSleep();

        if (is_a_sleep ^ is_b_sleep)
        {
            is_a_sleep ? m_body_a->SetAwake() : m_body_b->SetAwake();
        }
    }
}
