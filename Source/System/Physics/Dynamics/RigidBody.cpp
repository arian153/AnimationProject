#include "RigidBody.hpp"
#include "World.hpp"
#include "../../../Manager/Component/EngineComponent/RigidBodyComponent.hpp"

namespace CS460
{
    RigidBody::RigidBody()
    {
    }

    RigidBody::~RigidBody()
    {
    }

    void RigidBody::Shutdown() const
    {
        //disconnect rigid body
        if (m_component != nullptr)
        {
            m_component->m_rigid_body = nullptr;
        }
    }

    void RigidBody::Integrate(Real dt)
    {
        if (m_motion_mode == eMotionMode::Static)
            return;

        if (m_b_sleep)
            return;

        // integrate linear velocity
        m_linear_velocity += m_mass_data.inverse_mass * m_force_accumulator * dt;
        // integrate angular velocity
        m_angular_velocity += m_global_inverse_inertia * m_torque_accumulator * dt;
        // zero out accumulated force and torque
        m_force_accumulator.SetZero();
        m_torque_accumulator.SetZero();

        SyncFromTransform(m_transform);
        Vector3 delta_linear_velocity = m_linear_velocity * dt;
        delta_linear_velocity         = delta_linear_velocity.HadamardProduct(m_linear_constraints);
        m_global_centroid += delta_linear_velocity;
        // integrate orientation
        Vector3 delta_angular_velocity = m_angular_velocity;
        delta_angular_velocity         = delta_angular_velocity.HadamardProduct(m_angular_constraints);
        Vector3 axis                   = delta_angular_velocity.Unit();
        Real    radian                 = delta_angular_velocity.Length() * dt;
        m_local.orientation.AddRotation(axis, radian);
        // update remain properties
        UpdateOrientation();
        UpdateInertia();
        UpdatePosition();
        SyncToTransform(m_transform);
    }

    void RigidBody::UpdateCentroid()
    {
        m_global_centroid = m_local.orientation.Rotate(m_mass_data.local_centroid) + m_local.position;
    }

    void RigidBody::UpdatePosition()
    {
        m_local.position = m_local.orientation.Rotate(-m_mass_data.local_centroid) + m_global_centroid;
    }

    void RigidBody::UpdateInertia()
    {
        m_global_inverse_inertia = m_local.orientation * m_mass_data.local_inverse_inertia * m_inverse_orientation;
        m_global_inertia         = m_local.orientation * m_mass_data.local_inertia * m_inverse_orientation;
    }

    void RigidBody::UpdateOrientation()
    {
        m_local.orientation.SetNormalize();
        m_inverse_orientation = m_local.orientation.Inverse();
        m_inverse_orientation.SetNormalize();
    }

    void RigidBody::SetMassData(const MassData& mass_data)
    {
        m_mass_data = mass_data;
        if (m_motion_mode != eMotionMode::Dynamic)
        {
            SetMassInfinite();
            SetInertiaInfinite();
        }
    }

    void RigidBody::ApplyForce(const Vector3& force, const Vector3& at)
    {
        SetAwake();
        m_force_accumulator += force;
        m_torque_accumulator += (at - m_global_centroid).CrossProduct(force);
    }

    void RigidBody::ApplyForceCentroid(const Vector3& force)
    {
        SetAwake();
        m_force_accumulator += force;
    }

    void RigidBody::ApplyTorque(const Vector3& torque)
    {
        SetAwake();
        m_torque_accumulator += torque;
    }

    void RigidBody::SetPosition(const Vector3& position)
    {
        m_local.position = position;
        UpdateCentroid();
    }

    void RigidBody::SetCentroid(const Vector3& centroid)
    {
        m_global_centroid = centroid;
        UpdatePosition();
    }

    void RigidBody::SetOrientation(const Quaternion& orientation)
    {
        m_local.orientation = orientation;
        UpdateOrientation();
        UpdateCentroid();
        UpdateInertia();
    }

    Vector3 RigidBody::GetPosition() const
    {
        return m_local.position;
    }

    Vector3 RigidBody::GetCentroid() const
    {
        return m_global_centroid;
    }

    Vector3 RigidBody::GetLocalCentroid() const
    {
        return m_mass_data.local_centroid;
    }

    Quaternion RigidBody::GetOrientation() const
    {
        return m_local.orientation;
    }

    void RigidBody::SetLinearVelocity(const Vector3& linear)
    {
        m_linear_velocity = linear;
    }

    void RigidBody::SetAngularVelocity(const Vector3& angular)
    {
        m_angular_velocity = angular;
    }

    void RigidBody::AddLinearVelocity(const Vector3& linear_delta)
    {
        m_linear_velocity += linear_delta;
    }

    void RigidBody::AddAngularVelocity(const Vector3& angular_delta)
    {
        m_angular_velocity += angular_delta;
    }

    Vector3 RigidBody::GetLinearVelocity() const
    {
        return m_linear_velocity;
    }

    Vector3 RigidBody::GetAngularVelocity() const
    {
        return m_angular_velocity;
    }

    Vector3 RigidBody::GetForce() const
    {
        return m_force_accumulator;
    }

    Vector3 RigidBody::GetTorque() const
    {
        return m_torque_accumulator;
    }

    void RigidBody::SetPositionalConstraints(const Vector3& linear)
    {
        m_linear_constraints = linear;
    }

    void RigidBody::SetRotationalConstraints(const Vector3& angular)
    {
        m_angular_constraints = angular;
    }

    void RigidBody::SetAwake()
    {
        if (m_motion_mode != eMotionMode::Static)
        {
            m_b_sleep        = false;
            m_sleep_momentum = Physics::Collision::SLEEP_AWAKE;
        }
    }

    void RigidBody::UpdateSleepState()
    {
        if (m_motion_mode == eMotionMode::Kinematic)
        {
            return;
        }

        Real current_momentum = (m_linear_velocity.LengthSquared() + m_angular_velocity.LengthSquared()) * m_mass_data.inverse_mass;
        m_sleep_momentum      = Physics::Collision::SLEEP_BIAS * m_sleep_momentum + (1.0f - Physics::Collision::SLEEP_BIAS) * current_momentum;

        if (m_sleep_momentum < Physics::Collision::SLEEP_THRESHOLD)
        {
            //set sleep
            m_linear_velocity.SetZero();
            m_angular_velocity.SetZero();
            m_b_sleep = true;
        }
        else
        {
            if (m_sleep_momentum > Physics::Collision::SLEEP_THRESHOLD_EXTREME)
                m_sleep_momentum = Physics::Collision::SLEEP_THRESHOLD_EXTREME;
        }
    }

    void RigidBody::SetMassInfinite()
    {
        m_mass_data.mass         = 0.0f;
        m_mass_data.inverse_mass = 0.0f;
    }

    void RigidBody::SetMass(Real mass)
    {
        m_mass_data.mass         = mass;
        m_mass_data.inverse_mass = 1.0f / mass;
    }

    Real RigidBody::Mass() const
    {
        return m_mass_data.mass;
    }

    Real RigidBody::InverseMass() const
    {
        return m_mass_data.inverse_mass;
    }

    Matrix33 RigidBody::MassMatrix() const
    {
        Real     mass = m_mass_data.mass;
        Matrix33 mass_matrix;
        mass_matrix.SetDiagonal(mass, mass, mass);
        return mass_matrix;
    }

    Matrix33 RigidBody::InverseMassMatrix() const
    {
        Real     inv_mass = m_mass_data.inverse_mass;
        Matrix33 inverse_mass_matrix;
        inverse_mass_matrix.SetDiagonal(inv_mass, inv_mass, inv_mass);
        return inverse_mass_matrix;
    }

    void RigidBody::SetInertiaInfinite()
    {
        m_global_inverse_inertia.SetZero();
        m_mass_data.local_inertia.SetZero();
        m_mass_data.local_inverse_inertia.SetZero();
    }

    void RigidBody::SetInertia(const Matrix33& inertia_tensor)
    {
        m_global_inverse_inertia          = inertia_tensor.Inverse();
        m_mass_data.local_inverse_inertia = m_inverse_orientation * m_global_inverse_inertia * m_local.orientation;
        m_mass_data.local_inertia         = m_mass_data.local_inverse_inertia.Inverse();
    }

    Matrix33 RigidBody::Inertia() const
    {
        return m_global_inertia;
    }

    Matrix33 RigidBody::InverseInertia() const
    {
        return m_global_inverse_inertia;
    }

    Matrix33 RigidBody::LocalInertia() const
    {
        return m_mass_data.local_inertia;
    }

    Matrix33 RigidBody::InverseLocalInertia() const
    {
        return m_mass_data.local_inverse_inertia;
    }

    void RigidBody::SetLocalInertia(const Matrix33& inertia)
    {
        m_mass_data.local_inertia         = inertia;
        m_mass_data.local_inverse_inertia = inertia.Inverse();
        m_global_inverse_inertia          = m_local.orientation * m_mass_data.local_inverse_inertia * m_inverse_orientation;
    }

    void RigidBody::SetMotionMode(eMotionMode motion_mode)
    {
        m_motion_mode = motion_mode;
    }

    eMotionMode RigidBody::GetMotionMode() const
    {
        return m_motion_mode;
    }

    Vector3 RigidBody::LocalToWorldPoint(const Vector3& local_point) const
    {
        return m_local.LocalToWorldPoint(local_point);
    }

    Vector3 RigidBody::WorldToLocalPoint(const Vector3& world_point) const
    {
        return m_local.WorldToLocalPoint(world_point);;
    }

    Vector3 RigidBody::LocalToWorldVector(const Vector3& local_vector) const
    {
        return m_local.LocalToWorldVector(local_vector);
    }

    Vector3 RigidBody::WorldToLocalVector(const Vector3& world_vector) const
    {
        return m_local.WorldToLocalVector(world_vector);
    }

    void RigidBody::SyncToTransform(Transform* transform) const
    {
        if (transform != nullptr)
        {
            *transform = m_local;
        }
    }

    void RigidBody::SyncFromTransform(Transform* transform)
    {
        if (transform != nullptr)
        {
            if (m_local.position != transform->position
                || m_local.orientation != transform->orientation
            )
            {
                m_local = *transform;
                UpdateOrientation();
                UpdateCentroid();
                UpdateInertia();
            }
        }
    }

    void RigidBody::SetTransform(Transform* transform)
    {
        m_transform = transform;
    }

    void RigidBody::Clone(RigidBody* origin)
    {
        if (origin != this)
        {
            //linear data
            m_linear_velocity    = origin->m_linear_velocity;
            m_force_accumulator  = origin->m_force_accumulator;
            m_linear_constraints = origin->m_linear_constraints;
            //angular data
            m_inverse_orientation = origin->m_inverse_orientation;
            m_angular_velocity    = origin->m_angular_velocity;
            m_torque_accumulator  = origin->m_torque_accumulator;
            m_angular_constraints = origin->m_angular_constraints;
            //mass data
            m_mass_data              = origin->m_mass_data;
            m_global_centroid        = origin->m_global_centroid;
            m_global_inverse_inertia = origin->m_global_inverse_inertia;
            //others
            m_local       = origin->m_local;
            m_motion_mode = origin->m_motion_mode;
        }
    }

    bool RigidBody::IsSleep() const
    {
        return m_b_sleep;
    }
}
