#pragma once
#include "../../Math/Algebra/Vector3.hpp"
#include "../../Math/Algebra/Quaternion.hpp"
#include "../../Math/Structure/Basis.hpp"
#include "../../Math/Algebra/Matrix44.hpp"

namespace CS460
{
    class Transform;
    class CameraComponent;
    class Scene;

    class Camera
    {
    public:
        Camera();
        ~Camera();

        void Initialize();
        void Update();
        void Shutdown() const;
        void SetPosition(const Vector3& position);
        void SetOrientation(const Quaternion& orientation);
        void SetZoom(Real zoom);
        void SetScene(Scene* scene);
        void LookAt(const Vector3& position, const Vector3& target, const Vector3& up = Math::Vector3::Y_AXIS);

        Vector3    GetPosition() const;
        Quaternion GetOrientation() const;
        Matrix44   GetViewMatrix() const;
        Basis      GetBasis() const;

        void SetTransform(Transform* transform);
        void SyncToTransform() const;
        bool SyncFromTransform();
        void UpdateViewMatrix();

        Transform* GetTransform() const;
    private:
        friend class CameraComponent;

    private:
        Matrix44         m_view_matrix;
        Vector3          m_position;
        Quaternion       m_orientation;
        Real             m_zoom = 1.0f;
        Basis            m_basis;
        Scene*           m_scene     = nullptr;
        Transform*       m_transform = nullptr;
        CameraComponent* m_component = nullptr;
    };
}
