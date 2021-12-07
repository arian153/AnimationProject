#pragma once
#include "../../../Math/Math.hpp"
#include "Constraint.hpp"
#include "../../Utility/FrictionUtility.hpp"
#include "../../Utility/PhysicsDef.hpp"

namespace CS460
{
    class ColorFlag;
    class ColliderPrimitive;
    class ContactPoint;
    class ContactManifold;
    class RigidBody;

    class ContactConstraint final : public Constraint
    {
    private:
        class Jacobian
        {
        public:
            Vector3 v_a;
            Vector3 w_a;
            Vector3 v_b;
            Vector3 w_b;
            Real    bias           = 0.0f;
            Real    effective_mass = 0.0f;
            Real    total_lambda   = 0.0f;
        };

        class VelocityTerm
        {
        public:
            Vector3 v_a;
            Vector3 w_a;
            Vector3 v_b;
            Vector3 w_b;
        };

        class PositionTerm
        {
        public:
            Vector3 p_a;
            Vector3 p_b;
        };

        class MassTerm
        {
        public:
            Real     m_a = 0.0f;
            Real     m_b = 0.0f;
            Matrix33 i_a;
            Matrix33 i_b;
        };

    public:
        explicit ContactConstraint(ContactManifold* input, FrictionUtility* friction_utility, bool enable_baum, Real tangent_speed = 0.0f);
        ~ContactConstraint();

        void Shutdown() override;

        void GenerateVelocityConstraints(Real dt) override;
        void SolveVelocityConstraints(Real dt) override;
        void ApplyVelocityConstraints() override;

        void GeneratePositionConstraints(Real dt) override;
        void SolvePositionConstraints(Real dt) override;
        void ApplyPositionConstraints() override;

        void Render(PrimitiveRenderer* primitive_renderer, const Color& color) const override;
        void WarmStart();
        Real GetRestitution(ColliderPrimitive* a, ColliderPrimitive* b) const;
        void InitializeJacobian(const ContactPoint& contact, const Vector3& direction, Jacobian& jacobian) const;
        void SolveJacobian(const ContactPoint& contact, Jacobian& jacobian, size_t i, Real dt, bool b_normal = false);
        void AwakeState() const;

    private:
        FrictionUtility* m_friction_utility = nullptr;
        ContactManifold* m_manifold         = nullptr;
        RigidBody*       m_body_a           = nullptr;
        RigidBody*       m_body_b           = nullptr;

        eMotionMode m_motion_a;
        eMotionMode m_motion_b;
        bool        m_b_enable_baumgarte = true;

        PositionTerm m_position_term;
        VelocityTerm m_velocity_term;
        MassTerm     m_mass_term;
        Jacobian     m_normal[Physics::Collision::MAX_MANIFOLD_POINT_COUNT];
        Jacobian     m_tangent[Physics::Collision::MAX_MANIFOLD_POINT_COUNT];
        Jacobian     m_bitangent[Physics::Collision::MAX_MANIFOLD_POINT_COUNT];
        Real         m_tangent_speed       = 0.0f;
        size_t       m_count               = 0;
        bool         m_b_init              = false;
        bool         m_b_ghost_constraints = false;
    };
}
