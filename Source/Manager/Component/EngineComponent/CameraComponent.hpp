#pragma once
#include "..//Component.hpp"
#include "../../../System/Math/Utility/VectorDef.hpp"
#include "../../../System/Math/Structure/AxisRadian.hpp"
#include "../../../System/Math/Structure/Basis.hpp"

namespace CS460
{
    class AxisRadian;
    class Quaternion;
    class Vector3;
    class Transform;
    class Camera;

    class CameraComponent final : public Component
    {
    public:
        ~CameraComponent();
        CameraComponent()                                      = delete;
        CameraComponent(const CameraComponent& rhs)            = delete;
        CameraComponent& operator=(const CameraComponent& rhs) = delete;

        void Initialize() override;
        void Update(Real dt) override;
        void Shutdown() override;

        void SetPosition(const Vector3& position) const;
        void SetOrientation(const Quaternion& orientation) const;
        void LookAt(const Vector3& position, const Vector3& target, const Vector3& up = Math::Vector3::Y_AXIS) const;

        //modify addition
        void AddPosition(const Vector3& delta_position) const;
        void AddDistanceInLookingDirection(Real distance) const;
        void AddDistanceInUpDirection(Real distance) const;
        void AddDistanceInRightDirection(Real distance) const;
        void AddRotation(const Quaternion& delta_rotation);
        void AddRotation(const AxisRadian& axis_radian);
        void AddRotationX(Real radian);
        void AddRotationY(Real radian);
        void AddRotationZ(Real radian);
        void AddRotationA(Real radian);
        void AddZoom(Real zoom) const;

        void ResetZoom() const;

        Basis GetBasis() const;
        Vector3 GetPosition() const;
    protected:
        bool Load(const Json::Value& data) override;
        void Save(Json::Value& data) const override;
        void Edit(CommandRegistry* command_registry) override;
        void Subscribe() override;
        void Unsubscribe() override;

    private:
        friend class CameraFactory;
        friend class Camera;

    private:
        explicit CameraComponent(Object* owner);
        void     Clone(CameraComponent* origin);

    private:
        Camera*    m_camera    = nullptr;
        Transform* m_transform = nullptr;
        AxisRadian m_axis_holder;
    };
}
