#pragma once
#include "Constraint.hpp"
#include "../../Utility/ConstraintUtility.hpp"

namespace CS460
{
    class RigidBody;

    class SpringConstraint final : public Constraint
    {
    public:
        explicit SpringConstraint();
        ~SpringConstraint();

        void Shutdown() override;

        void GenerateVelocityConstraints(Real dt) override;
        void GeneratePositionConstraints(Real dt) override;
        void SolveVelocityConstraints(Real dt) override;
        void SolvePositionConstraints(Real dt) override;
        void ApplyVelocityConstraints() override;
        void ApplyPositionConstraints() override;

        void Render(PrimitiveRenderer* primitive_renderer, const Color& color) const override;

        void SetUp(RigidBody* body_a, RigidBody* body_b);
        void SetUp(RigidBody* body_a, RigidBody* body_b, const Vector3& local_qa, const Vector3& local_qb);
        void SetUp(Transform* anchor, RigidBody* body);
        void SetUp(Transform* anchor, RigidBody* body, const Vector3& local_q);


    private:
        friend class SpringConstraintsComponent;

    protected:
        SpringConstraintsComponent* m_component = nullptr;

        bool       m_b_anchored = false;
        Transform* m_anchor     = nullptr;
        RigidBody* m_body_a     = nullptr;
        RigidBody* m_body_b     = nullptr;

        Real m_spring_constant = 1.0f;
        Real m_damper_constant = 1.0f;

        Vector3 m_local_q_a;
        Vector3 m_local_q_b;
    };
}
