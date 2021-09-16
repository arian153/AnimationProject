#include "CameraComponent.hpp"
#include "../../Space/Space.hpp"
#include "TransformComponent.hpp"
#include "../../Object/Object.hpp"
#include "../../../System/Core/Utility/CoreUtility.hpp"
#include "../../Resource/ResourceType/JsonResource.hpp"
#include "../../../External/JSONCPP/json/json.h"
#include "../../../System/Graphics/Element/Camera.hpp"
#include "../../../System/Graphics/Element/Scene.hpp"

namespace CS460
{
    CameraComponent::~CameraComponent()
    {
    }

    void CameraComponent::Initialize()
    {
        if (m_camera == nullptr)
        {
            m_camera = new Camera();
            m_camera->Initialize();
            m_camera->m_component = this;
            m_camera->SetScene(m_space->GetScene());
            Subscribe();
        }
        if (m_transform == nullptr)
        {
            if (m_owner->HasComponent<TransformComponent>())
            {
                m_transform = m_owner->GetComponent<TransformComponent>()->GetTransform();
                m_camera->SetTransform(m_transform);
                m_camera->SyncFromTransform();
                m_camera->UpdateViewMatrix();
            }
        }
    }

    void CameraComponent::Update(Real dt)
    {
        E5_UNUSED_PARAM(dt);
    }

    void CameraComponent::Shutdown()
    {
        Unsubscribe();
        if (m_camera != nullptr)
        {
            m_camera->Shutdown();
            delete m_camera;
            m_camera = nullptr;
        }
    }

    void CameraComponent::SetPosition(const Vector3& position) const
    {
        m_camera->SetPosition(position);
    }

    void CameraComponent::SetOrientation(const Quaternion& orientation) const
    {
        m_camera->SetOrientation(orientation);
    }

    void CameraComponent::LookAt(const Vector3& position, const Vector3& target, const Vector3& up) const
    {
        m_camera->LookAt(position, target, up);
    }

    void CameraComponent::AddPosition(const Vector3& delta_position) const
    {
        m_camera->SetPosition(delta_position + m_camera->m_position);
    }

    void CameraComponent::AddDistanceInLookingDirection(Real distance) const
    {
        auto    basis  = m_camera->GetBasis();
        Vector3 result = basis.k * distance;
        m_camera->SetPosition(result + m_camera->m_position);
    }

    void CameraComponent::AddDistanceInUpDirection(Real distance) const
    {
        auto    basis = m_camera->GetBasis();
        Vector3 result = basis.j * distance;
        m_camera->SetPosition(result + m_camera->m_position);
    }

    void CameraComponent::AddDistanceInRightDirection(Real distance) const
    {
        auto    basis = m_camera->GetBasis();
        Vector3 result = basis.i * distance;
        m_camera->SetPosition(result + m_camera->m_position);
    }

    void CameraComponent::AddRotation(const Quaternion& delta_rotation)
    {
        m_camera->m_orientation.AddRotation(delta_rotation);
        m_camera->SetOrientation(m_camera->m_orientation);
        m_axis_holder = m_camera->m_orientation.ToAxisRadian();
    }

    void CameraComponent::AddRotation(const AxisRadian& axis_radian)
    {
        m_camera->m_orientation.AddRotation(axis_radian.axis, axis_radian.radian);
        m_camera->SetOrientation(m_camera->m_orientation);
        m_axis_holder = m_camera->m_orientation.ToAxisRadian();
    }

    void CameraComponent::AddRotationX(Real radian)
    {
        m_camera->m_orientation.AddRotation(Math::Vector3::X_AXIS, radian);
        m_camera->SetOrientation(m_camera->m_orientation);
        m_axis_holder = m_camera->m_orientation.ToAxisRadian();
    }

    void CameraComponent::AddRotationY(Real radian)
    {
        m_camera->m_orientation.AddRotation(Math::Vector3::Y_AXIS, radian);
        m_camera->SetOrientation(m_camera->m_orientation);
        m_axis_holder = m_camera->m_orientation.ToAxisRadian();
    }

    void CameraComponent::AddRotationZ(Real radian)
    {
        m_camera->m_orientation.AddRotation(Math::Vector3::Z_AXIS, radian);
        m_camera->SetOrientation(m_camera->m_orientation);
        m_axis_holder = m_camera->m_orientation.ToAxisRadian();
    }

    void CameraComponent::AddRotationA(Real radian)
    {
        m_axis_holder.radian += radian;
        m_camera->m_orientation.Set(m_axis_holder);
        m_camera->SetOrientation(m_camera->m_orientation);
    }

    void CameraComponent::AddZoom(Real zoom) const
    {
        m_camera->SetZoom(m_camera->m_zoom + zoom);
    }

    void CameraComponent::ResetZoom() const
    {
        m_camera->SetZoom(1.0f);
    }

    Basis CameraComponent::GetBasis() const
    {
        return m_camera->GetBasis();
    }

    Vector3 CameraComponent::GetPosition() const
    {
        return m_camera->GetPosition();
    }

    bool CameraComponent::Load(const Json::Value& data)
    {
        if (JsonResource::HasMember(data, "Orientation") && JsonResource::IsQuaternion(data["Orientation"]))
        {
            m_camera->SetOrientation(JsonResource::AsQuaternionRIJK(data["Orientation"]));
        }
        if (JsonResource::HasMember(data, "Position") && JsonResource::IsVector3(data["Position"]))
        {
            m_camera->SetPosition(JsonResource::AsVector3(data["Position"]));
        }
        if (JsonResource::HasMember(data, "Zoom") && data["Zoom"].isDouble())
        {
            m_camera->m_zoom = data["Zoom"].asFloat();
        }
        return true;
    }

    void CameraComponent::Save(Json::Value& data) const
    {
    }

    void CameraComponent::Edit(CommandRegistry* command_registry)
    {
        ImGui::CollapsingHeader(m_type.c_str(), &m_b_open);
    }

    void CameraComponent::Subscribe()
    {
        if (m_space != nullptr && m_camera != nullptr)
        {
            m_space->GetScene()->AddCamera(m_camera);
        }
    }

    void CameraComponent::Unsubscribe()
    {
        if (m_space != nullptr && m_camera != nullptr)
        {
            m_space->GetScene()->RemoveCamera(m_camera);
        }
    }

    CameraComponent::CameraComponent(Object* owner)
        : Component(owner)
    {
    }

    void CameraComponent::Clone(CameraComponent* origin)
    {
        if (origin != nullptr && origin != this)
        {
            //copy data
        }
    }
}
