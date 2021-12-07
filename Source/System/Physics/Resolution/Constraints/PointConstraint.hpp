#pragma once
#include "Constraint.hpp"
#include "../../Utility/ConstraintUtility.hpp"

namespace CS460
{
    class RigidBody;

    class PointConstraint final : public Constraint
    {
    public:
        explicit PointConstraint(RigidBody* body, ConstraintUtility* utility);
        ~PointConstraint();

        void Shutdown() override;

        void GenerateVelocityConstraints(Real dt) override;
        void GeneratePositionConstraints(Real dt) override;
        void SolveVelocityConstraints(Real dt) override;
        void SolvePositionConstraints(Real dt) override;
        void ApplyVelocityConstraints() override;
        void ApplyPositionConstraints() override;

        void Render(PrimitiveRenderer* primitive_renderer, const Color& color) const override;

        void SetConstraintMode(eConstraintMode mode);
        void SetFrequency(Real frequency);
        void SetDampingRatio(Real damping_ratio);
        void EnableRotation(bool b_rotation);
        void SetTargetPoint(const Vector3& target);
        void SetAnchorPoint(const Vector3& anchor);

    private:
        friend class PointConstraintComponent;

    protected:
        PointConstraintComponent* m_component          = nullptr;
        ConstraintUtility*        m_constraint_utility = nullptr;
        RigidBody*                m_body               = nullptr;
        Vector3*                  m_target             = nullptr;

        eConstraintMode m_mode = eConstraintMode::Hard;
        ConstraintBias  m_bias;

        Real m_frequency     = 5.0f;
        Real m_damping_ratio = 0.5f;
        bool m_b_rotation    = true;

        Vector3  m_total_lambda;
        Vector3  m_r;
        Matrix33 m_effective_mass;
        Matrix33 m_cross;
        Vector3  m_position_error_bias;
        Vector3  m_local_anchor;
        Vector3  m_local_target;

    };
}
