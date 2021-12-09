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
        void SetUp(Transform* anchor, RigidBody* body);
    private:
        friend class SpringConstraintComponent;

    protected:
        bool       m_b_anchored = false;
        Transform* m_anchor     = nullptr;
        RigidBody* m_body_a     = nullptr;
        RigidBody* m_body_b     = nullptr;

        Real m_spring_constant = 1.0f;
        Real m_damper_constant = 1.0f;
    };
}
