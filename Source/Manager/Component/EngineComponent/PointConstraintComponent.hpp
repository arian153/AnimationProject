#pragma once
#include "..//Component.hpp"
#include "../../../System/Physics/Physics.hpp"

namespace CS460
{
    class PointConstraint;

    class PointConstraintComponent final : public Component
    {
    public:
        ~PointConstraintComponent();
        PointConstraintComponent()                                               = delete;
        PointConstraintComponent(const PointConstraintComponent& rhs)            = delete;
        PointConstraintComponent& operator=(const PointConstraintComponent& rhs) = delete;

        void Initialize() override;
        void Update(Real dt) override;
        void Shutdown() override;

        void SetConstraintMode(eConstraintMode mode) const;
        void SetFrequency(Real frequency) const;
        void SetDampingRatio(Real damping_ratio) const;
        void EnableRotation(bool b_rotation) const;
        void SetTargetPoint(const Vector3& target) const;
        void SetAnchorPoint(const Vector3& local_anchor) const;
        void SetTargetObject(Object* target) const;

    protected:
        bool Load(const Json::Value& data) override;
        void Save(Json::Value& data) const override;
        void Edit(CommandRegistry* command_registry) override;
        void Subscribe() override;
        void Unsubscribe() override;

    private:
        friend class PointConstraintFactory;
        friend class PointConstraint;

    private:
        explicit PointConstraintComponent(Object* owner);
        void     Clone(PointConstraintComponent* origin);

    private:
        RigidBody*       m_rigid_body       = nullptr;
        PointConstraint* m_point_constraint = nullptr;

        Object*    m_target_object    = nullptr;

        bool m_b_init = false;
    };
}
