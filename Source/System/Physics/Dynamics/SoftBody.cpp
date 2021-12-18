#include "SoftBody.hpp"

#include "../../Math/Algebra/Matrix33.hpp"
#include "../../Math/Utility/Utility.inl"

namespace CS460
{
    SoftBody::SoftBody()
    {
    }

    SoftBody::~SoftBody()
    {
    }

    void SoftBody::Shutdown() const
    {
    }

    void SoftBody::IntegrateEuler(Real dt)
    {
        if (m_motion_mode == eMotionMode::Static)
            return;

        if (m_b_sleep)
            return;

        // integrate velocity

        for (auto& mass_point : m_mass_points)
        {
            mass_point.linear_velocity += mass_point.mass * mass_point.force_accumulator * dt;
            mass_point.force_accumulator.SetZero();
            Vector3 d_pos = (m_linear_velocity * dt).HadamardProduct(m_linear_constraints);
            mass_point.local_position += d_pos;
        }

        SyncFromTransform(m_shared_data);
        Vector3 delta_linear_velocity = m_linear_velocity * dt;
        delta_linear_velocity         = delta_linear_velocity.HadamardProduct(m_linear_constraints);
        m_global_centroid += delta_linear_velocity;
        // integrate orientation
        Vector3 delta_angular_velocity = m_angular_velocity;
        delta_angular_velocity         = delta_angular_velocity.HadamardProduct(m_angular_constraints);
        Vector3 axis                   = delta_angular_velocity.Unit();
        Real    radian                 = delta_angular_velocity.Length() * dt;
        m_transform.orientation.AddRotation(axis, radian);
        // update remain properties
        UpdateOrientation();
        UpdateInertia();
        UpdatePosition();
        SyncToTransform(m_shared_data);
    }

    void SoftBody::IntegrateVerlet(Real dt)
    {
    }

    void SoftBody::UpdateMassData()
    {
        m_mass_data.mass = 0.0f;
        m_mass_data.local_centroid.SetZero();
        m_mass_data.local_inertia.SetZero();

        for (auto& mass_point : m_mass_points)
        {
            m_mass_data.mass += mass_point.mass;
            m_mass_data.local_centroid += mass_point.mass * mass_point.local_position;

            Real xx = mass_point.local_position.x * mass_point.local_position.x;
            Real xy = mass_point.local_position.x * mass_point.local_position.y;
            Real xz = mass_point.local_position.x * mass_point.local_position.z;
            Real yy = mass_point.local_position.y * mass_point.local_position.y;
            Real yz = mass_point.local_position.y * mass_point.local_position.z;
            Real zz = mass_point.local_position.z * mass_point.local_position.z;

            m_mass_data.local_inertia.data[0] += yy + zz;
            m_mass_data.local_inertia.data[1] -= xy;
            m_mass_data.local_inertia.data[2] -= xz;
            m_mass_data.local_inertia.data[3] -= xy;
            m_mass_data.local_inertia.data[4] += xx + zz;
            m_mass_data.local_inertia.data[5] -= yz;
            m_mass_data.local_inertia.data[6] -= xz;
            m_mass_data.local_inertia.data[7] -= yz;
            m_mass_data.local_inertia.data[8] += xx + yy;
        }

        if (Math::IsZero(m_mass_data.mass))
        {
            m_mass_data.inverse_mass = 0.0f;
        }
        else
        {
            m_mass_data.inverse_mass = 1.0f / m_mass_data.mass;
        }

        m_mass_data.local_centroid *= m_mass_data.inverse_mass;
        m_mass_data.local_inverse_inertia = m_mass_data.local_inertia.Inverse();
    }

    void SoftBody::ResolveInternalSpringForce()
    {
        for (auto& spring : m_springs)
        {
            MassPoint& mass_a = m_mass_points[spring.id_a];
            MassPoint& mass_b = m_mass_points[spring.id_b];

            Vector3 spring_force = spring.K * (mass_b.world_position - mass_a.world_position)
                    + spring.D * (mass_b.linear_velocity - mass_a.linear_velocity);

            mass_a.force_accumulator += spring_force;
            mass_b.force_accumulator -= spring_force;
        }
    }

    void SoftBody::ResolveInternalCollision()
    {
        if (m_b_internal_collision)
        {
            size_t size = m_mass_points.size();
            size_t last = size - 1;
            for (size_t i = 0; i < last; ++i)
            {
                for (size_t j = i + 1; j < size; ++j)
                {
                    //resolve collision
                    Vector3 distance = m_mass_points[i].local_position - m_mass_points[j].local_position;
                    Real    radius   = m_mass_points[i].effective_radius + m_mass_points[j].effective_radius;
                    if (distance.LengthSquared() < radius * radius)
                    {
                        Real    dist      = distance.Length();
                        Real    depth     = radius - dist;
                        Vector3 normal    = distance / dist;
                        Vector3 contact_i = normal * m_mass_points[i].effective_radius + m_mass_points[i].local_position;
                        Vector3 contact_j = -normal * m_mass_points[j].effective_radius + m_mass_points[j].local_position;
                    }
                }
            }
        }
    }

    void SoftBody::UpdateCentroid()
    {
        m_global_centroid = m_transform.orientation.Rotate(m_mass_data.local_centroid) + m_transform.position;
    }

    void SoftBody::UpdatePosition()
    {
        m_transform.position = m_transform.orientation.Rotate(-m_mass_data.local_centroid) + m_global_centroid;
    }

    void SoftBody::UpdateInertia()
    {
        m_global_inverse_inertia = m_transform.orientation * m_mass_data.local_inverse_inertia * m_inverse_orientation;
        m_global_inertia         = m_transform.orientation * m_mass_data.local_inertia * m_inverse_orientation;
    }

    void SoftBody::UpdateOrientation()
    {
        m_transform.orientation.SetNormalize();
        m_inverse_orientation = m_transform.orientation.Inverse();
        m_inverse_orientation.SetNormalize();
    }

    void SoftBody::ApplyForce(const Vector3& force, const Vector3& at_world)
    {
        SetAwake();
        Vector3 torque = (at_world - m_global_centroid).CrossProduct(force);
        m_force_accumulator += force;
        m_torque_accumulator += torque;

        for (auto& mass_point : m_mass_points)
        {
            mass_point.force_accumulator += force;
            Vector3 r = mass_point.world_position - m_global_centroid;
            mass_point.force_accumulator += r.SkewSymmetricMatrix().Inverse() * torque;
        }
    }

    void SoftBody::ApplyForceCentroid(const Vector3& force)
    {
        SetAwake();
        m_force_accumulator += force;
        for (auto& mass_point : m_mass_points)
        {
            mass_point.force_accumulator += force;
        }
    }

    void SoftBody::ApplyTorque(const Vector3& torque)
    {
        SetAwake();
        m_torque_accumulator += torque;
        for (auto& mass_point : m_mass_points)
        {
            Vector3 r = mass_point.world_position - m_global_centroid;
            mass_point.force_accumulator += r.SkewSymmetricMatrix().Inverse() * torque;
        }
    }

    void SoftBody::ClearForceStatus()
    {
        m_force_accumulator.SetZero();
        m_torque_accumulator.SetZero();

        for (auto& mass_point : m_mass_points)
        {
            mass_point.force_accumulator.SetZero();
        }
    }

    void SoftBody::SetPosition(const Vector3& position)
    {
        m_transform.position = position;
        UpdateCentroid();
    }

    void SoftBody::SetCentroid(const Vector3& centroid)
    {
        m_global_centroid = centroid;
        UpdatePosition();
    }

    void SoftBody::SetOrientation(const Quaternion& orientation)
    {
        m_transform.orientation = orientation;
        UpdateOrientation();
        UpdateCentroid();
        UpdateInertia();
    }

    Vector3 SoftBody::GetPosition() const
    {
        return m_transform.position;
    }

    Vector3 SoftBody::GetCentroid() const
    {
        return m_global_centroid;
    }

    Vector3 SoftBody::GetLocalCentroid() const
    {
        return m_mass_data.local_centroid;
    }

    Quaternion SoftBody::GetOrientation() const
    {
        return m_transform.orientation;
    }

    void SoftBody::SetLinearVelocity(const Vector3& linear)
    {
        m_linear_velocity = linear;
        for (auto& mass_point : m_mass_points)
        {
            mass_point.linear_velocity = linear;
        }
    }

    void SoftBody::SetAngularVelocity(const Vector3& angular)
    {
        m_angular_velocity = angular;

        for (auto& mass_point : m_mass_points)
        {
            Vector3 r                  = mass_point.local_position - m_mass_data.local_centroid;
            mass_point.linear_velocity = r.SkewSymmetricMatrix().Inverse() * angular;
        }
    }

    void SoftBody::AddLinearVelocity(const Vector3& linear_delta)
    {
        m_linear_velocity += linear_delta;
        for (auto& mass_point : m_mass_points)
        {
            mass_point.linear_velocity += linear_delta;
        }
    }

    void SoftBody::AddAngularVelocity(const Vector3& angular_delta)
    {
        m_angular_velocity += angular_delta;
        for (auto& mass_point : m_mass_points)
        {
            Vector3 r = mass_point.local_position - m_mass_data.local_centroid;
            mass_point.linear_velocity += r.SkewSymmetricMatrix().Inverse() * angular_delta;
        }
    }

    Vector3 SoftBody::GetLinearVelocity() const
    {
        return m_linear_velocity;
    }

    Vector3 SoftBody::GetAngularVelocity() const
    {
        return m_angular_velocity;
    }

    Vector3 SoftBody::GetForce() const
    {
        return m_force_accumulator;
    }

    Vector3 SoftBody::GetTorque() const
    {
        return m_torque_accumulator;
    }

    void SoftBody::SetPositionalConstraints(const Vector3& linear)
    {
        m_linear_constraints = linear;
    }

    void SoftBody::SetRotationalConstraints(const Vector3& angular)
    {
        m_angular_constraints = angular;
    }

    void SoftBody::SetAwake()
    {
        if (m_motion_mode != eMotionMode::Static)
        {
            m_b_sleep        = false;
            m_sleep_momentum = Physics::Collision::SLEEP_AWAKE;
        }
    }

    void SoftBody::UpdateSleepState()
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

    Real SoftBody::Mass() const
    {
        return m_mass_data.mass;
    }

    Real SoftBody::InverseMass() const
    {
        return m_mass_data.inverse_mass;
    }

    Matrix33 SoftBody::MassMatrix() const
    {
        Real     mass = m_mass_data.mass;
        Matrix33 mass_matrix;
        mass_matrix.SetDiagonal(mass, mass, mass);
        return mass_matrix;
    }

    Matrix33 SoftBody::InverseMassMatrix() const
    {
        Real     inv_mass = m_mass_data.inverse_mass;
        Matrix33 inverse_mass_matrix;
        inverse_mass_matrix.SetDiagonal(inv_mass, inv_mass, inv_mass);
        return inverse_mass_matrix;
    }

    Matrix33 SoftBody::Inertia() const
    {
        return m_global_inertia;
    }

    Matrix33 SoftBody::InverseInertia() const
    {
        return m_global_inverse_inertia;
    }

    Matrix33 SoftBody::LocalInertia() const
    {
        return m_mass_data.local_inertia;
    }

    Matrix33 SoftBody::InverseLocalInertia() const
    {
        return m_mass_data.local_inverse_inertia;
    }

    void SoftBody::SetMotionMode(eMotionMode motion_mode)
    {
        m_motion_mode = motion_mode;
    }

    eMotionMode SoftBody::GetMotionMode() const
    {
        return m_motion_mode;
    }

    Vector3 SoftBody::LocalToWorldPoint(const Vector3& local_point) const
    {
        return m_transform.LocalToWorldPoint(local_point);
    }

    Vector3 SoftBody::WorldToLocalPoint(const Vector3& world_point) const
    {
        return m_transform.WorldToLocalPoint(world_point);;
    }

    Vector3 SoftBody::LocalToWorldVector(const Vector3& local_vector) const
    {
        return m_transform.LocalToWorldVector(local_vector);
    }

    Vector3 SoftBody::WorldToLocalVector(const Vector3& world_vector) const
    {
        return m_transform.WorldToLocalVector(world_vector);
    }

    void SoftBody::SyncToTransform(Transform* transform) const
    {
        if (transform != nullptr)
        {
            *transform = m_transform;
        }
    }

    void SoftBody::SyncFromTransform(Transform* transform)
    {
        if (transform != nullptr)
        {
            m_transform = *transform;
            UpdateOrientation();
            UpdateCentroid();
            UpdateInertia();
        }
    }

    void SoftBody::SetTransform(Transform* transform)
    {
        m_shared_data = transform;
    }

    void SoftBody::Clone(SoftBody* origin)
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
            m_transform   = origin->m_transform;
            m_motion_mode = origin->m_motion_mode;
        }
    }

    bool SoftBody::IsSleep() const
    {
        return m_b_sleep;
    }
}
