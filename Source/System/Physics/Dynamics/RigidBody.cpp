#include "RigidBody.hpp"
#include "World.hpp"
#include "../../../Manager/Component/EngineComponent/RigidBodyComponent.hpp"
#include "../Utility/StepData.hpp"

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

    void RigidBody::IntegrateEuler(Real dt)
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

    void RigidBody::IntegrateRK2(Real dt)
    {
        if (m_motion_mode == eMotionMode::Static)
            return;

        if (m_b_sleep)
            return;

        // Runge-Kutta 2 method
        StepData step_data;

        //step K1 = f(y, dt);
        Vector3 mid_linear_velocity  = m_linear_velocity;
        Vector3 mid_angular_velocity = m_angular_velocity;
        //apply internal constraints
        mid_linear_velocity  = mid_linear_velocity.HadamardProduct(m_linear_constraints);
        mid_angular_velocity = mid_angular_velocity.HadamardProduct(m_angular_constraints);
        //calculate K1 values
        Vector3    lxk1 = mid_linear_velocity * dt;
        Vector3    lvk1 = m_mass_data.inverse_mass * m_force_accumulator * dt;
        Quaternion rxk1 = Quaternion(mid_angular_velocity.Unit(), mid_angular_velocity.Length() * dt).Unit();
        Vector3    rvk1 = m_global_inverse_inertia * m_torque_accumulator * dt;
        //caluculate force & torque from step data
        step_data.linear_position_step  = lxk1;
        step_data.linear_velocity_step  = lvk1;
        step_data.angular_rotation_step = rxk1;
        step_data.angular_velocity_step = rvk1;
        CalculateStepData(step_data);

        //step K2 = f(y+(h * K3), dt+h);
        mid_linear_velocity  = m_linear_velocity + lvk1;
        mid_angular_velocity = m_angular_velocity + rvk1;
        //apply internal constraints
        mid_linear_velocity  = mid_linear_velocity.HadamardProduct(m_linear_constraints);
        mid_angular_velocity = mid_angular_velocity.HadamardProduct(m_angular_constraints);
        //calculate K2 values
        Vector3    lxk2 = mid_linear_velocity * dt;
        Vector3    lvk2 = m_mass_data.inverse_mass * step_data.force_result * dt;
        Quaternion rxk2 = Quaternion(mid_angular_velocity.Unit(), mid_angular_velocity.Length() * dt).Unit();
        Vector3    rvk2 = m_global_inverse_inertia * step_data.torque_result * dt;

        //final value;
        Vector3    delta_pos = (lxk1 + lxk2) * 0.5f;
        Vector3    delta_lv  = (lvk1 + lvk2) * 0.5f;
        Quaternion delta_rot = (rxk1 + rxk2) * 0.5f;
        Vector3    delta_rv  = (rvk1 + rvk2) * 0.5f;

        m_global_centroid += delta_pos;
        m_linear_velocity += delta_lv;
        m_transform.orientation.AddRotation(delta_rot);
        m_angular_velocity += delta_rv;

        // update remain properties
        UpdateOrientation();
        UpdateInertia();
        UpdatePosition();
        SyncToTransform(m_shared_data);
    }

    void RigidBody::IntegrateRK4(Real dt)
    {
        if (m_motion_mode == eMotionMode::Static)
            return;

        if (m_b_sleep)
            return;

        SyncFromTransform(m_shared_data);

        // Runge-Kutta 4 method
        StepData step_data;

        //step K1 = f(y, dt);
        Vector3 mid_linear_velocity  = m_linear_velocity;
        Vector3 mid_angular_velocity = m_angular_velocity;
        //apply internal constraints
        mid_linear_velocity  = mid_linear_velocity.HadamardProduct(m_linear_constraints);
        mid_angular_velocity = mid_angular_velocity.HadamardProduct(m_angular_constraints);
        //calculate K1 values
        Vector3    lxk1 = mid_linear_velocity * dt;
        Vector3    lvk1 = m_mass_data.inverse_mass * m_force_accumulator * dt;
        Quaternion rxk1 = Quaternion(mid_angular_velocity.Unit(), mid_angular_velocity.Length() * dt).Unit();
        Vector3    rvk1 = m_global_inverse_inertia * m_torque_accumulator * dt;
        //caluculate force & torque from step data
        step_data.linear_position_step  = lxk1;
        step_data.linear_velocity_step  = lvk1;
        step_data.angular_rotation_step = rxk1;
        step_data.angular_velocity_step = rvk1;
        CalculateStepData(step_data);

        //step K2 = f(y+(h/2 * K1), dt+h/2);
        mid_linear_velocity  = m_linear_velocity + 0.5f * lvk1;
        mid_angular_velocity = m_angular_velocity + 0.5f * rvk1;
        //apply internal constraints
        mid_linear_velocity  = mid_linear_velocity.HadamardProduct(m_linear_constraints);
        mid_angular_velocity = mid_angular_velocity.HadamardProduct(m_angular_constraints);
        //calculate K2 values
        Vector3    lxk2 = mid_linear_velocity * dt;
        Vector3    lvk2 = m_mass_data.inverse_mass * step_data.force_result * dt;
        Quaternion rxk2 = Quaternion(mid_angular_velocity.Unit(), mid_angular_velocity.Length() * dt).Unit();
        Vector3    rvk2 = m_global_inverse_inertia * step_data.torque_result * dt;
        //caluculate force & torque from step data
        step_data.linear_position_step  = lxk2;
        step_data.linear_velocity_step  = lvk2;
        step_data.angular_rotation_step = rxk2;
        step_data.angular_velocity_step = rvk2;
        CalculateStepData(step_data);

        //step K3 = f(y+(h/2 * K2), dt+h/2);
        mid_linear_velocity  = m_linear_velocity + 0.5f * lvk2;
        mid_angular_velocity = m_angular_velocity + 0.5f * rvk2;
        //apply internal constraints
        mid_linear_velocity  = mid_linear_velocity.HadamardProduct(m_linear_constraints);
        mid_angular_velocity = mid_angular_velocity.HadamardProduct(m_angular_constraints);
        //calculate K3 values
        Vector3    lxk3 = mid_linear_velocity * dt;
        Vector3    lvk3 = m_mass_data.inverse_mass * step_data.force_result * dt;
        Quaternion rxk3 = Quaternion(mid_angular_velocity.Unit(), mid_angular_velocity.Length() * dt).Unit();
        Vector3    rvk3 = m_global_inverse_inertia * step_data.torque_result * dt;
        //caluculate force & torque from step data
        step_data.linear_position_step  = lxk3;
        step_data.linear_velocity_step  = lvk3;
        step_data.angular_rotation_step = rxk3;
        step_data.angular_velocity_step = rvk3;
        CalculateStepData(step_data);

        //step K4 = f(y+(h * K3), dt+h);
        mid_linear_velocity  = m_linear_velocity + lvk3;
        mid_angular_velocity = m_angular_velocity + rvk3;
        //apply internal constraints
        mid_linear_velocity  = mid_linear_velocity.HadamardProduct(m_linear_constraints);
        mid_angular_velocity = mid_angular_velocity.HadamardProduct(m_angular_constraints);
        //calculate K4 values
        Vector3    lxk4 = mid_linear_velocity * dt;
        Vector3    lvk4 = m_mass_data.inverse_mass * step_data.force_result * dt;
        Quaternion rxk4 = Quaternion(mid_angular_velocity.Unit(), mid_angular_velocity.Length() * dt).Unit();
        Vector3    rvk4 = m_global_inverse_inertia * step_data.torque_result * dt;

        //final value;
        Vector3    delta_pos = (lxk1 + 2.0f * lxk2 + 2.0f * lxk3 + lxk4) / 6.0f;
        Vector3    delta_lv  = (lvk1 + 2.0f * lvk2 + 2.0f * lvk3 + lvk4) / 6.0f;
        Quaternion delta_rot = (rxk1 + 2.0f * rxk2 + 2.0f * rxk3 + rxk4) * (1.0f / 6.0f);
        Vector3    delta_rv  = (rvk1 + 2.0f * rvk2 + 2.0f * rvk3 + rvk4) / 6.0f;

        m_global_centroid += delta_pos;
        m_linear_velocity += delta_lv;
        m_transform.orientation.AddRotation(delta_rot);
        m_angular_velocity += delta_rv;

        // update remain properties
        UpdateOrientation();
        UpdateInertia();
        UpdatePosition();
        SyncToTransform(m_shared_data);
    }

    void RigidBody::IntegrateVerlet(Real dt)
    {
        if (m_motion_mode == eMotionMode::Static)
            return;

        if (m_b_sleep)
            return;

        SyncFromTransform(m_shared_data);

        // Verlet method
        StepData step_data;

        //calculate half step
        Vector3 linear_acceleration        = m_mass_data.inverse_mass * m_force_accumulator;
        Vector3 angular_acceleration       = m_global_inverse_inertia * m_torque_accumulator;
        Vector3 half_step_linear_velocity  = m_linear_velocity + 0.5f * dt * linear_acceleration;
        Vector3 half_step_angular_velocity = m_angular_velocity + 0.5f * dt * angular_acceleration;
        //apply internal constraints
        half_step_linear_velocity  = half_step_linear_velocity.HadamardProduct(m_linear_constraints);
        half_step_angular_velocity = half_step_angular_velocity.HadamardProduct(m_angular_constraints);
        //caluculate force & torque from step data
        step_data.linear_position_step  = half_step_linear_velocity * dt;
        step_data.linear_velocity_step  = linear_acceleration * dt;
        step_data.angular_rotation_step = Quaternion(half_step_angular_velocity.Unit(), half_step_angular_velocity.Length() * dt).Unit();
        step_data.angular_velocity_step = angular_acceleration * dt;
        CalculateStepData(step_data);

        //calculate verlet step
        linear_acceleration        = m_mass_data.inverse_mass * step_data.force_result;
        angular_acceleration       = m_global_inverse_inertia * step_data.torque_result;
        half_step_linear_velocity  = half_step_linear_velocity + 0.5f * dt * linear_acceleration;
        half_step_angular_velocity = half_step_angular_velocity + 0.5f * dt * angular_acceleration;

        m_global_centroid += step_data.linear_position_step;
        m_linear_velocity += half_step_linear_velocity;
        m_transform.orientation.AddRotation(step_data.angular_rotation_step);
        m_angular_velocity += half_step_angular_velocity;

        // update remain properties
        UpdateOrientation();
        UpdateInertia();
        UpdatePosition();
        SyncToTransform(m_shared_data);
    }

    void RigidBody::IntegrateMidpoint(Real dt)
    {
        if (m_motion_mode == eMotionMode::Static)
            return;

        if (m_b_sleep)
            return;

        SyncFromTransform(m_shared_data);

        // Midpoint method
        StepData step_data;
        Real     midpoint_dt = 0.5f * dt;

        //step K1 = f(y, dt);
        Vector3 delta_linear_velocity  = m_linear_velocity;
        Vector3 delta_angular_velocity = m_angular_velocity;
        //apply internal constraints
        delta_linear_velocity  = delta_linear_velocity.HadamardProduct(m_linear_constraints);
        delta_angular_velocity = delta_angular_velocity.HadamardProduct(m_angular_constraints);
        //caluculate force & torque from step data
        step_data.linear_position_step  = delta_linear_velocity * midpoint_dt;
        step_data.linear_velocity_step  = m_mass_data.inverse_mass * m_force_accumulator * midpoint_dt;
        step_data.angular_rotation_step = Quaternion(delta_angular_velocity.Unit(), delta_angular_velocity.Length() * midpoint_dt).Unit();
        step_data.angular_velocity_step = m_global_inverse_inertia * m_torque_accumulator * midpoint_dt;
        CalculateStepData(step_data);

        //apply midpoint method
        m_global_centroid += delta_linear_velocity * dt;
        m_linear_velocity += dt * m_mass_data.inverse_mass * step_data.force_result;
        m_transform.orientation.AddRotation(Quaternion(delta_angular_velocity.Unit(), delta_angular_velocity.Length() * dt));
        m_angular_velocity += m_global_inverse_inertia * step_data.torque_result * dt;

        // update remain properties
        UpdateOrientation();
        UpdateInertia();
        UpdatePosition();
        SyncToTransform(m_shared_data);
    }

    void RigidBody::UpdateCentroid()
    {
        m_global_centroid = m_transform.orientation.Rotate(m_mass_data.local_centroid) + m_transform.position;
    }

    void RigidBody::UpdatePosition()
    {
        m_transform.position = m_transform.orientation.Rotate(-m_mass_data.local_centroid) + m_global_centroid;
    }

    void RigidBody::UpdateInertia()
    {
        m_global_inverse_inertia = m_transform.orientation * m_mass_data.local_inverse_inertia * m_inverse_orientation;
        m_global_inertia         = m_transform.orientation * m_mass_data.local_inertia * m_inverse_orientation;
    }

    void RigidBody::UpdateOrientation()
    {
        m_transform.orientation.SetNormalize();
        m_inverse_orientation = m_transform.orientation.Inverse();
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
        m_transform.position = position;
        UpdateCentroid();
    }

    void RigidBody::SetCentroid(const Vector3& centroid)
    {
        m_global_centroid = centroid;
        UpdatePosition();
    }

    void RigidBody::SetOrientation(const Quaternion& orientation)
    {
        m_transform.orientation = orientation;
        UpdateOrientation();
        UpdateCentroid();
        UpdateInertia();
    }

    Vector3 RigidBody::GetPosition() const
    {
        return m_transform.position;
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
        return m_transform.orientation;
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
        m_mass_data.local_inverse_inertia = m_inverse_orientation * m_global_inverse_inertia * m_transform.orientation;
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
        m_global_inverse_inertia          = m_transform.orientation * m_mass_data.local_inverse_inertia * m_inverse_orientation;
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
        return m_transform.LocalToWorldPoint(local_point);
    }

    Vector3 RigidBody::WorldToLocalPoint(const Vector3& world_point) const
    {
        return m_transform.WorldToLocalPoint(world_point);;
    }

    Vector3 RigidBody::LocalToWorldVector(const Vector3& local_vector) const
    {
        return m_transform.LocalToWorldVector(local_vector);
    }

    Vector3 RigidBody::WorldToLocalVector(const Vector3& world_vector) const
    {
        return m_transform.WorldToLocalVector(world_vector);
    }

    void RigidBody::SyncToTransform(Transform* transform) const
    {
        if (transform != nullptr)
        {
            *transform = m_transform;
        }
    }

    void RigidBody::SyncFromTransform(Transform* transform)
    {
        if (transform != nullptr)
        {
            m_transform = *transform;
            UpdateOrientation();
            UpdateCentroid();
            UpdateInertia();
        }
    }

    void RigidBody::SetTransform(Transform* transform)
    {
        m_shared_data = transform;
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
            m_transform   = origin->m_transform;
            m_motion_mode = origin->m_motion_mode;
        }
    }

    bool RigidBody::IsSleep() const
    {
        return m_b_sleep;
    }

    Vector3 RigidBody::GetLinearAcceleration(const Vector3& v, const Vector3& p, Real dt) const
    {
        //Dummy
        Vector3 avg_d_vel = (p - m_global_centroid) / dt;
        return (v - avg_d_vel) / dt;
    }

    Vector3 RigidBody::GetAngularAcceleration(const Vector3& w, const Quaternion& r, Real dt) const
    {
        //Dummy
        Quaternion rotation = m_transform.orientation.Inverse() * r;
        rotation.SetNormalize();
        AxisRadian axis_radian                = rotation.ToAxisRadian();
        Vector3    avg_delta_angular_velocity = axis_radian.axis * (axis_radian.radian / dt);
        return (w - avg_delta_angular_velocity) / dt;
    }

    void RigidBody::CalculateStepData([[maybe_unused]] StepData& step_data)
    {
        //Get next step data from resolution phase
    }
}
