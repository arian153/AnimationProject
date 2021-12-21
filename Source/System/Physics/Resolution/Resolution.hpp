#pragma once
#include "../../Math/Math.hpp"
#include "../NarrowPhase/ManifoldTable.hpp"
#include "../Dynamics/RigidBody.hpp"
#include "../Utility/FrictionUtility.hpp"
#include <vector>
#include "Constraints/ContactConstraint.hpp"
#include "../Utility/ConstraintUtility.hpp"

namespace CS460
{
    class CommandRegistry;
    class Force;
    class ColorFlag;
    class Constraint;
    class RigidContactPoint;
    class RigidContactManifold;

    class Resolution
    {
    public:
        Resolution();
        ~Resolution();
        void Initialize();
        void Shutdown();

        void ApplyForces(std::vector<RigidBody*>* rigid_bodies, Real dt);
        void ProcessContactConstraints(ManifoldTable* manifold_table);
        void SolveVelocityConstraints(Real dt);
        void IntegrateRigidBodies(std::vector<RigidBody*>* rigid_bodies, Real dt) const;
        void SolvePositionConstraints(Real dt);

        void SetPrimitiveRenderer(PrimitiveRenderer* primitive_renderer);
        void Render(const ColorFlag& draw_contact_flag);
        void Edit(CommandRegistry* registry);

    private:
        friend class World;
    private:
        bool m_b_warm_starting    = true;
        bool m_b_enable_sleep     = true;
        bool m_b_enable_baumgarte = true;

        int m_velocity_iteration = 8;
        int m_position_iteration = 3;

        FrictionUtility                m_friction_utility;
        ConstraintUtility              m_constraint_utility;
        std::vector<ContactConstraint> m_contact_constraints;
        std::vector<Constraint*>       m_constraints;
        std::vector<Force*>            m_forces;
        PrimitiveRenderer*             m_primitive_renderer = nullptr;
    };
}
