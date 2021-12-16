#include "SoftBody.hpp"

#include "../../Math/Algebra/Matrix33.hpp"

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
    }

    void SoftBody::UpdateCentroid()
    {
    }

    void SoftBody::UpdatePosition()
    {
    }

    void SoftBody::UpdateInertia()
    {
    }

    void SoftBody::UpdateOrientation()
    {
    }

    void SoftBody::SetMassData(const MassData& mass_data)
    {
    }

    void SoftBody::ApplyForce(const Vector3& force, const Vector3& at)
    {
    }

    void SoftBody::ApplyForceCentroid(const Vector3& force)
    {
    }

    void SoftBody::ApplyTorque(const Vector3& torque)
    {
    }

    void SoftBody::SetPosition(const Vector3& position)
    {
    }

    void SoftBody::SetCentroid(const Vector3& centroid)
    {
    }

    void SoftBody::SetOrientation(const Quaternion& orientation)
    {
    }

    Vector3 SoftBody::GetPosition() const
    {
        return Vector3();
    }

    Vector3 SoftBody::GetCentroid() const
    {
        return Vector3();
    }

    Vector3 SoftBody::GetLocalCentroid() const
    {
        return Vector3();
    }

    Quaternion SoftBody::GetOrientation() const
    {
        return Quaternion();
    }

    void SoftBody::SetLinearVelocity(const Vector3& linear)
    {
    }

    void SoftBody::SetAngularVelocity(const Vector3& angular)
    {
    }

    void SoftBody::AddLinearVelocity(const Vector3& linear_delta)
    {
    }

    void SoftBody::AddAngularVelocity(const Vector3& angular_delta)
    {
    }

    Vector3 SoftBody::GetLinearVelocity() const
    {
        return Vector3();
    }

    Vector3 SoftBody::GetAngularVelocity() const
    {
        return Vector3();
    }

    Vector3 SoftBody::GetForce() const
    {
        return Vector3();
    }

    Vector3 SoftBody::GetTorque() const
    {
        return Vector3();
    }

    void SoftBody::SetPositionalConstraints(const Vector3& linear)
    {
    }

    void SoftBody::SetRotationalConstraints(const Vector3& angular)
    {
    }

    void SoftBody::SetAwake()
    {
    }

    void SoftBody::UpdateSleepState()
    {
    }

    void SoftBody::SetMassInfinite()
    {
    }

    void SoftBody::SetMass(Real mass)
    {
    }

    Real SoftBody::Mass() const
    {
        return 1.0f;
    }

    Real SoftBody::InverseMass() const
    {
        return 1.0f;
    }

    Matrix33 SoftBody::MassMatrix() const
    {
        return Matrix33();
    }

    Matrix33 SoftBody::InverseMassMatrix() const
    {
        return Matrix33();
    }

    void SoftBody::SetInertiaInfinite()
    {
    }

    void SoftBody::SetInertia(const Matrix33& inertia_tensor)
    {
    }

    Matrix33 SoftBody::Inertia() const
    {
        return Matrix33();
    }

    Matrix33 SoftBody::InverseInertia() const
    {
        return Matrix33();
    }

    Matrix33 SoftBody::LocalInertia() const
    {
        return Matrix33();
    }

    Matrix33 SoftBody::InverseLocalInertia() const
    {
        return Matrix33();
    }

    void SoftBody::SetLocalInertia(const Matrix33& inertia)
    {
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
        return Vector3();
    }

    Vector3 SoftBody::WorldToLocalPoint(const Vector3& world_point) const
    {
        return Vector3();
    }

    Vector3 SoftBody::LocalToWorldVector(const Vector3& local_vector) const
    {
        return Vector3();
    }

    Vector3 SoftBody::WorldToLocalVector(const Vector3& world_vector) const
    {
        return Vector3();
    }

    void SoftBody::SyncToTransform(Transform* transform) const
    {
    }

    void SoftBody::SyncFromTransform(Transform* transform)
    {
    }

    void SoftBody::SetTransform(Transform* transform)
    {
    }

    void SoftBody::Clone(SoftBody* origin)
    {
    }

    bool SoftBody::IsSleep() const
    {
        return m_b_sleep;
    }
}
