#include "Camera.hpp"
#include "../../Math/Math.hpp"
#include "../../Math/Utility/MatrixUtility.hpp"
#include "Scene.hpp"
#include "../../../Manager/Component/EngineComponent/CameraComponent.hpp"

namespace CS460
{
    Camera::Camera()
    {
    }

    Camera::~Camera()
    {
    }

    void Camera::Initialize()
    {
    }

    void Camera::Update()
    {
        if (SyncFromTransform())
        {
            UpdateViewMatrix();
            SyncToTransform();
        }
    }

    void Camera::Shutdown() const
    {
        if (m_component != nullptr)
        {
            m_component->m_camera = nullptr;
        }
    }

    void Camera::SetPosition(const Vector3& position)
    {
        m_position = position;
        UpdateViewMatrix();
        SyncToTransform();
    }

    void Camera::SetOrientation(const Quaternion& orientation)
    {
        m_orientation = orientation;
        m_orientation.SetNormalize();
        m_basis = Basis();
        m_basis.Rotate(m_orientation);
        UpdateViewMatrix();
        SyncToTransform();
    }

    void Camera::SetZoom(Real zoom)
    {
        m_zoom = zoom;
        UpdateViewMatrix();
        SyncToTransform();
    }

    void Camera::SetScene(Scene* scene)
    {
        if (scene != nullptr)
        {
            m_scene = scene;
        }
    }

    Vector3 Camera::GetPosition() const
    {
        return m_position;
    }

    Quaternion Camera::GetOrientation() const
    {
        return m_orientation;
    }

    Matrix44 Camera::GetViewMatrix() const
    {
        return m_view_matrix;
    }

    void Camera::LookAt(const Vector3& position, const Vector3& target, const Vector3& up)
    {
        m_view_matrix = Math::Matrix44::LookAt(position, target, up);
        m_position    = position;
        m_basis.k     = (target - position).Normalize();
        m_basis.i     = CrossProduct(up, m_basis.k).Normalize();
        m_basis.j     = CrossProduct(m_basis.k, m_basis.i);
        m_orientation.Set(Vector3(1.0f), m_basis.i);
        m_view_matrix *= m_zoom;
        if (m_scene != nullptr)
        {
            m_scene->UpdateView();
        }
        SyncToTransform();
    }

    Basis Camera::GetBasis() const
    {
        return m_basis;
    }

    void Camera::SetTransform(Transform* transform)
    {
        m_transform = transform;
    }

    void Camera::SyncToTransform() const
    {
        if (m_transform != nullptr)
        {
            m_transform->position    = m_position;
            m_transform->orientation = m_orientation;
            m_transform->scale       = Vector3(m_zoom, m_zoom, m_zoom);
        }
    }

    bool Camera::SyncFromTransform()
    {
        bool result = false;
        if (m_transform != nullptr)
        {
            if (m_position.IsNotEqual(m_transform->position))
            {
                m_position = m_transform->position;
                result     = true;
            }
            if (m_orientation.IsNotEqual(m_transform->orientation))
            {
                m_orientation = m_transform->orientation;
                m_basis       = Basis();
                m_orientation.SetNormalize();
                m_basis.Rotate(m_orientation);
                m_basis.Normalize();
                result = true;
            }
            if (Math::IsNotEqual(m_zoom, m_transform->scale.x))
            {
                m_zoom = m_transform->scale.x;
                result = true;
            }
        }
        return result;
    }

    void Camera::UpdateViewMatrix()
    {
        Vector3 up    = m_orientation.Rotate(Math::Vector3::Y_AXIS);
        Vector3 look  = m_orientation.Rotate(Math::Vector3::Z_AXIS);
        m_view_matrix = Math::Matrix44::LookAt(m_position, look + m_position, up);
        m_view_matrix *= m_zoom;
        if (m_scene != nullptr)
        {
            m_scene->UpdateView();
        }
    }

    Transform* Camera::GetTransform() const
    {
        return m_transform;
    }
}
