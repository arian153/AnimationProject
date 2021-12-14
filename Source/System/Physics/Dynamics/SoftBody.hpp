#pragma once
#include <vector>

#include "../../Math/Structure/Transform.hpp"
#include "../Utility/PhysicsDef.hpp"

namespace CS460
{
    class MassData;
    class Transform;

    class SoftBody
    {
    public:
        SoftBody();
        ~SoftBody();

        void Shutdown() const;

        void IntegrateEuler(Real dt);
        //void IntegrateRK2(Real dt);
        //void IntegrateRK4(Real dt);
        //void IntegrateVerlet(Real dt);
        //void IntegrateMidpoint(Real dt);

        void UpdateCentroid();
        void UpdatePosition();
        void UpdateInertia();
        void UpdateOrientation();

        void SetMassData(const MassData& mass_data);

        void ApplyForce(const Vector3& force, const Vector3& at);
        void ApplyForceCentroid(const Vector3& force);
        void ApplyTorque(const Vector3& torque);

        void SetPosition(const Vector3& position);
        void SetCentroid(const Vector3& centroid);
        void SetOrientation(const Quaternion& orientation);

        Vector3    GetPosition() const;
        Vector3    GetCentroid() const;
        Vector3    GetLocalCentroid() const;
        Quaternion GetOrientation() const;

        void SetLinearVelocity(const Vector3& linear);
        void SetAngularVelocity(const Vector3& angular);
        void AddLinearVelocity(const Vector3& linear_delta);
        void AddAngularVelocity(const Vector3& angular_delta);

        Vector3 GetLinearVelocity() const;
        Vector3 GetAngularVelocity() const;
        Vector3 GetForce() const;
        Vector3 GetTorque() const;

        void SetPositionalConstraints(const Vector3& linear);
        void SetRotationalConstraints(const Vector3& angular);
        void SetAwake();
        void UpdateSleepState();

        void     SetMassInfinite();
        void     SetMass(Real mass = 1.0f);
        Real     Mass() const;
        Real     InverseMass() const;
        Matrix33 MassMatrix() const;
        Matrix33 InverseMassMatrix() const;

        void     SetInertiaInfinite();
        void     SetInertia(const Matrix33& inertia_tensor);
        Matrix33 Inertia() const;
        Matrix33 InverseInertia() const;

        Matrix33 LocalInertia() const;
        Matrix33 InverseLocalInertia() const;
        void     SetLocalInertia(const Matrix33& inertia);

        void        SetMotionMode(eMotionMode motion_mode);
        eMotionMode GetMotionMode() const;

        Vector3 LocalToWorldPoint(const Vector3& local_point) const;
        Vector3 WorldToLocalPoint(const Vector3& world_point) const;
        Vector3 LocalToWorldVector(const Vector3& local_vector) const;
        Vector3 WorldToLocalVector(const Vector3& world_vector) const;

        void SyncToTransform(Transform* transform) const;
        void SyncFromTransform(Transform* transform);
        void SetTransform(Transform* transform);
        void Clone(SoftBody* origin);
        bool IsSleep() const;

    private:

        eMotionMode m_motion_mode = eMotionMode::Dynamic;
        Transform*  m_transform   = nullptr;
        Transform   m_local_copy;

        bool m_b_sleep        = false;
        Real m_sleep_momentum = Physics::Collision::SLEEP_AWAKE;


        //std::vector<>
    };
}
