#include "SpringConstraint.hpp"

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
    }

    void SpringConstraint::GenerateVelocityConstraints(Real dt)
    {
 
    }

    void SpringConstraint::GeneratePositionConstraints(Real dt)
    {
    }

    void SpringConstraint::SolveVelocityConstraints(Real dt)
    {
        Vector3 v_a = m_body_a->GetLinearVelocity();
        Vector3 v_b = m_body_b->GetLinearVelocity();

        //spring's end point which is connected to body.
        Vector3 q_a;
        Vector3 q_b;

        Vector3 f_a = m_spring_constant * (q_b - q_a) + m_damper_constant * (v_b - v_a);
        Vector3 f_b = -f_a;

        m_body_a->ApplyForce(f_a, q_a);
        m_body_b->ApplyForce(f_b, q_b);
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
    }

    void SpringConstraint::SetUp(RigidBody* body_a, RigidBody* body_b)
    {
        m_b_anchored = false;
    }

    void SpringConstraint::SetUp(Transform* anchor, RigidBody* body)
    {
        m_b_anchored = true;
    }
}
