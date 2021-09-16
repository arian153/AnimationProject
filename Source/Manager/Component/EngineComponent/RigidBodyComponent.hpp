#pragma once
#include "..//Component.hpp"
#include "../../../System/Physics/Physics.hpp"

namespace CS460
{
    class RigidBodyComponent final : public Component
    {
    public:
        ~RigidBodyComponent();
        RigidBodyComponent()                                         = delete;
        RigidBodyComponent(const RigidBodyComponent& rhs)            = delete;
        RigidBodyComponent& operator=(const RigidBodyComponent& rhs) = delete;

        void Initialize() override;
        void Update(Real dt) override;
        void Shutdown() override;

        //setter
        void ApplyForce(const Vector3& force, const Vector3& at) const;
        void ApplyForceCentroid(const Vector3& force) const;
        void SetLinearVelocity(const Vector3& linear) const;
        void SetAngularVelocity(const Vector3& angular) const;
        void SetMassInfinite() const;
        void SetMass(Real mass = 1.0f) const;
        void SetMass(const Real& mass);
        void SetPositionalConstraints(const Vector3& linear) const;
        void SetRotationalConstraints(const Vector3& angular) const;
        void SetMotionMode(const eMotionMode& motion_mode);
        void SetTransform(Transform* transform);

        //getter
        Vector3     GetLinearVelocity() const;
        Vector3     GetAngularVelocity() const;
        Real        GetMass() const;
        Real        GetInverseMass() const;
        Matrix33    GetMassMatrix() const;
        Matrix33    GetInverseMassMatrix() const;
        Matrix33    GetInertia() const;
        Matrix33    GetInverseInertia() const;
        eMotionMode GetMotionMode() const;
        RigidBody*  GetRigidBody() const;

    protected:
        bool Load(const Json::Value& data) override;
        void Save(Json::Value& data) const override;
        void Edit(CommandRegistry* command_registry) override;
        void Subscribe() override;
        void Unsubscribe() override;

    private:
        friend class RigidBodyFactory;
        friend class ColliderComponent;
        friend class RigidBody;

    private:
        explicit RigidBodyComponent(Object* owner);
        void     Clone(RigidBodyComponent* origin);

    private:
        RigidBody*   m_rigid_body   = nullptr;
        ColliderSet* m_collider_set = nullptr;
        Transform*   m_transform    = nullptr;

        Matrix33 m_prev_inertia;
        bool     m_b_init = false;

        int m_motion_index    = 0;
        int m_detection_index = 0;
    };
}
