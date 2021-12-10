#include "SpringConstraint.hpp"

#include "../../../../Manager/Component/EngineComponent/SpringConstraintsComponent.hpp"
#include "../../../Graphics/Utility/PrimitiveRenderer.hpp"
#include "../../Dynamics/RigidBody.hpp"

namespace CS460
{
    SpringConstraint::SpringConstraint()
    {
    }

    SpringConstraint::~SpringConstraint()
    {
    }

    void SpringConstraint::Shutdown()
    {
        if (m_component != nullptr)
        {
            m_component->m_spring_constraint = nullptr;
        }
    }

    void SpringConstraint::GenerateVelocityConstraints(Real dt)
    {
    }

    void SpringConstraint::GeneratePositionConstraints(Real dt)
    {
    }

    void SpringConstraint::SolveVelocityConstraints(Real dt)
    {
        //anchor-body mode -> only apply body a
        //body-body mode -> apply force both body.

        if (m_b_anchored)
        {
            Vector3 v_a  = m_body_a->GetLinearVelocity();
            Vector3 r_a  = m_local_q_a - m_body_a->GetLocalCentroid();
            Vector3 q_a2 = m_body_a->LocalToWorldVector(r_a) + m_body_a->GetCentroid();

            //spring's end point which is connected to body.
            Vector3 q_a = m_body_a->LocalToWorldPoint(m_local_q_a);
            Vector3 q_b = m_anchor->position;

            Vector3 f_a = m_spring_constant * (q_b - q_a) + m_damper_constant * (-v_a);
            m_body_a->ApplyForce(f_a, q_a);
        }
        else
        {
            Vector3 v_a = m_body_a->GetLinearVelocity();
            Vector3 v_b = m_body_b->GetLinearVelocity();

            //spring's end point which is connected to body.
            Vector3 q_a = m_body_a->LocalToWorldPoint(m_local_q_a);
            Vector3 q_b = m_body_b->LocalToWorldPoint(m_local_q_b);

            Vector3 f_a = m_spring_constant * (q_b - q_a) + m_damper_constant * (v_b - v_a);
            Vector3 f_b = -f_a;
            m_body_a->ApplyForce(f_a, q_a);
            m_body_b->ApplyForce(f_b, q_b);
        }
    }

    void SpringConstraint::SolvePositionConstraints(Real dt)
    {
    }

    void SpringConstraint::ApplyVelocityConstraints()
    {
    }

    void SpringConstraint::ApplyPositionConstraints()
    {
    }

    void SpringConstraint::Render(PrimitiveRenderer* primitive_renderer, const Color& color) const
    {
        if (m_b_anchored)
        {
            primitive_renderer->DrawSegment(m_anchor->position, m_body_a->LocalToWorldPoint(m_local_q_a), color);
        }
        else
        {
            primitive_renderer->DrawSegment(m_body_a->LocalToWorldPoint(m_local_q_a), m_body_b->LocalToWorldPoint(m_local_q_b), color);
        }
    }

    void SpringConstraint::SetUp(RigidBody* body_a, RigidBody* body_b)
    {
        m_b_anchored = false;
        m_body_a     = body_a;
        m_body_b     = body_b;
    }

    void SpringConstraint::SetUp(RigidBody* body_a, RigidBody* body_b, const Vector3& local_qa, const Vector3& local_qb)
    {
        m_b_anchored = false;
        m_body_a     = body_a;
        m_body_b     = body_b;
        m_local_q_a  = local_qa;
        m_local_q_b  = local_qb;
    }

    void SpringConstraint::SetUp(Transform* anchor, RigidBody* body)
    {
        m_b_anchored = true;
        m_anchor     = anchor;
        m_body_a     = body;
    }

    void SpringConstraint::SetUp(Transform* anchor, RigidBody* body, const Vector3& local_q)
    {
        m_b_anchored = true;
        m_anchor     = anchor;
        m_body_a     = body;
        m_local_q_a  = local_q;
    }
}
