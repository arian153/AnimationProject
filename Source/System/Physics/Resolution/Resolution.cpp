#include "Resolution.hpp"
#include "../ColliderPrimitive/ColliderPrimitive.hpp"
#include "Constraints/ContactConstraint.hpp"
#include <vector>
// ReSharper disable once CppUnusedIncludeDirective
#include "Contact/ContactManifold.hpp"
#include "../../Graphics/DataType/Color.hpp"
#include "../../Graphics/Utility/PrimitiveRenderer.hpp"
#include "Force/Force.hpp"

namespace CS460
{
    Resolution::Resolution()
    {
    }

    Resolution::~Resolution()
    {
    }

    void Resolution::Initialize()
    {
        m_friction_utility.Initialize();
    }

    void Resolution::Shutdown()
    {
        m_friction_utility.Shutdown();
        for (auto& constraint : m_constraints)
        {
            constraint->Shutdown();
            delete constraint;
            constraint = nullptr;
        }
        m_constraints.clear();
        for (auto& force : m_forces)
        {
            force->Shutdown();
            delete force;
            force = nullptr;
        }
        m_forces.clear();
    }

    void Resolution::ApplyForces(std::vector<RigidBody*>* rigid_bodies, Real dt)
    {
        for (auto& force : m_forces)
        {
            force->Clear();
            for (auto& body : *rigid_bodies)
            {
                force->Update(body, dt);
            }
        }
    }

    void Resolution::ProcessContactConstraints(ManifoldTable* manifold_table)
    {
        m_contact_constraints.clear();
        for (auto& [key, manifold] : manifold_table->m_manifold_table)
        {
            if (manifold.is_collide)
            {
                m_contact_constraints.emplace_back(&manifold, &m_friction_utility, m_b_enable_baumgarte);
            }
        }
    }

    void Resolution::SolveVelocityConstraints(Real dt)
    {
        if (m_velocity_iteration > 0)
        {
            for (auto& constraint : m_constraints)
            {
                constraint->GenerateVelocityConstraints(dt);
            }

            if (m_b_warm_starting == true)
            {
                for (auto& contact_constraint : m_contact_constraints)
                {
                    contact_constraint.GenerateVelocityConstraints(dt);
                    contact_constraint.WarmStart();
                    contact_constraint.AwakeState();
                }
            }
            else
            {
                for (auto& contact_constraint : m_contact_constraints)
                {
                    contact_constraint.GenerateVelocityConstraints(dt);
                    contact_constraint.AwakeState();
                }
            }
            for (int i = 0; i < m_velocity_iteration; ++i)
            {
                for (auto& constraint : m_constraints)
                {
                    constraint->SolveVelocityConstraints(dt);
                }
                for (auto& contact : m_contact_constraints)
                {
                    contact.SolveVelocityConstraints(dt);
                }
            }

            for (auto& constraint : m_constraints)
            {
                constraint->ApplyVelocityConstraints();
            }
            for (auto& contact : m_contact_constraints)
            {
                contact.ApplyVelocityConstraints();
            }
        }
    }

    void Resolution::IntegrateRigidBodies(std::vector<RigidBody*>* rigid_bodies, Real dt) const
    {
        if (m_b_enable_sleep)
        {
            for (auto& body : *rigid_bodies)
            {
                body->IntegrateEuler(dt);
                body->UpdateSleepState();
            }
        }
        else
        {
            for (auto& body : *rigid_bodies)
            {
                body->IntegrateEuler(dt);
            }
        }
    }

    void Resolution::SolvePositionConstraints(Real dt)
    {
        if (m_position_iteration > 0 && m_velocity_iteration > 0)
        {
            for (auto& constraint : m_constraints)
            {
                constraint->GeneratePositionConstraints(dt);
            }
            for (auto& contact : m_contact_constraints)
            {
                contact.GeneratePositionConstraints(dt);
            }
            for (int i = 0; i < m_position_iteration; ++i)
            {
                for (auto& constraint : m_constraints)
                {
                    constraint->SolvePositionConstraints(dt);
                }
                for (auto& contact : m_contact_constraints)
                {
                    contact.SolvePositionConstraints(dt);
                }
            }
            for (auto& constraint : m_constraints)
            {
                constraint->ApplyPositionConstraints();
            }
            for (auto& contact : m_contact_constraints)
            {
                contact.ApplyPositionConstraints();
            }
        }
    }

    void Resolution::SetPrimitiveRenderer(PrimitiveRenderer* primitive_renderer)
    {
        m_primitive_renderer = primitive_renderer;
    }

    void Resolution::Render(const ColorFlag& draw_contact_flag)
    {
        if (draw_contact_flag.b_flag)
        {
            for (auto& contact : m_contact_constraints)
            {
                contact.Render(m_primitive_renderer, draw_contact_flag.color);
            }
        }

        for (auto& constraint : m_constraints)
        {
            constraint->Render(m_primitive_renderer, Color());
        }

        for (auto& force : m_forces)
        {
            if (force->draw_force.b_flag)
            {
                for (auto& draw : force->applied_forces)
                {
                    m_primitive_renderer->DrawArrow(draw.a, draw.a + draw.b, force->draw_force.color);
                }
            }
        }
    }

    void Resolution::Edit(CommandRegistry* registry)
    {
        for (auto& force : m_forces)
        {
            force->Edit(registry);
        }
    }
}
