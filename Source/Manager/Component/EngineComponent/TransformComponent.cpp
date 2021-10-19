#include "TransformComponent.hpp"
#include "../../../System/Core/Utility/CoreUtility.hpp"
#include "../../Resource/ResourceType/JsonResource.hpp"
#include "../../../External/JSONCPP/json/json.h"
#include "../../../System/GUI/Editor/Command/CommandRegistry.hpp"
#include "../../../System/GUI/Editor/Command/EditorCommand.hpp"

namespace
{
    CS460::Random random;
}

namespace CS460
{
    TransformComponent::~TransformComponent()
    {
    }

    void TransformComponent::Initialize()
    {
    }

    void TransformComponent::Update(Real dt)
    {
        E5_UNUSED_PARAM(dt);
    }

    void TransformComponent::Shutdown()
    {
    }

    void TransformComponent::SetPosition(const Vector3& position)
    {
        m_transform.position = position;
        UpdateChildrenPositionRecursive(m_transform.position);
    }

    void TransformComponent::SetPosition(Real x, Real y, Real z)
    {
        m_transform.position.Set(x, y, z);
        UpdateChildrenPositionRecursive(m_transform.position);
    }

    void TransformComponent::SetOrientation(const Matrix33& rotation_matrix)
    {
        m_transform.orientation = rotation_matrix.ToQuaternion();
        m_axis_holder           = m_transform.orientation.ToAxisRadian();
        UpdateChildrenOrientationRecursive(m_transform.orientation);
    }

    void TransformComponent::SetOrientation(const Quaternion& quaternion)
    {
        m_transform.orientation = quaternion;
        m_transform.orientation.SetNormalize();
        m_axis_holder = m_transform.orientation.ToAxisRadian();
        UpdateChildrenOrientationRecursive(m_transform.orientation);
    }

    void TransformComponent::SetOrientation(const AxisRadian& axis_radian)
    {
        m_axis_holder = axis_radian;
        m_transform.orientation.Set(axis_radian);
        UpdateChildrenOrientationRecursive(m_transform.orientation);
    }

    void TransformComponent::SetOrientation(const EulerAngle& rotation)
    {
        m_transform.orientation.Set(rotation);
        m_axis_holder = m_transform.orientation.ToAxisRadian();
        UpdateChildrenOrientationRecursive(m_transform.orientation);
    }

    void TransformComponent::SetOrientationAxis(const Vector3& axis)
    {
        m_axis_holder.axis = axis;
        m_transform.orientation.Set(m_axis_holder);
        UpdateChildrenOrientationRecursive(m_transform.orientation);
    }

    void TransformComponent::SetScale(const Vector3& scale)
    {
        m_transform.scale = scale;
    }

    void TransformComponent::SetScale(const Vector3& scale, eAnchor3D anchor)
    {
        if (anchor == eAnchor3D::CCC)
        {
            m_transform.scale = scale;
        }
        else
        {
            auto curr_size = m_transform.scale.Half();
            auto new_size  = scale.Half();
            m_transform.position += (new_size - curr_size).HadamardProduct(Anchor::AnchorVector3(anchor));
            m_transform.scale = scale;
        }
    }

    void TransformComponent::SetScale(Real x, Real y, Real z, eAnchor3D anchor)
    {
        Vector3 scale(x, y, z);
        if (anchor == eAnchor3D::CCC)
        {
            m_transform.scale = scale;
        }
        else
        {
            auto curr_size = m_transform.scale.Half();
            auto new_size  = scale.Half();
            m_transform.position += (new_size - curr_size).HadamardProduct(Anchor::AnchorVector3(anchor));
            m_transform.scale = scale;
        }
    }

    void TransformComponent::AddPosition(const Vector3& delta_position)
    {
        m_transform.position += delta_position;
        UpdateChildrenPositionRecursive(m_transform.position);
    }

    void TransformComponent::AddRotation(const Quaternion& delta_rotation)
    {
        m_transform.orientation.AddRotation(delta_rotation);
        m_axis_holder = m_transform.orientation.ToAxisRadian();
        UpdateChildrenOrientationRecursive(m_transform.orientation);
    }

    void TransformComponent::AddRotation(const AxisRadian& axis_radian)
    {
        m_transform.orientation.AddRotation(axis_radian.axis, axis_radian.radian);
        m_axis_holder = m_transform.orientation.ToAxisRadian();
        UpdateChildrenOrientationRecursive(m_transform.orientation);
    }

    void TransformComponent::AddRotationX(Real radian)
    {
        m_transform.orientation.AddRotation(Math::Vector3::X_AXIS, radian);
        m_axis_holder = m_transform.orientation.ToAxisRadian();
        UpdateChildrenOrientationRecursive(m_transform.orientation);
    }

    void TransformComponent::AddRotationY(Real radian)
    {
        m_transform.orientation.AddRotation(Math::Vector3::Y_AXIS, radian);
        m_axis_holder = m_transform.orientation.ToAxisRadian();
        UpdateChildrenOrientationRecursive(m_transform.orientation);
    }

    void TransformComponent::AddRotationZ(Real radian)
    {
        m_transform.orientation.AddRotation(Math::Vector3::Z_AXIS, radian);
        m_axis_holder = m_transform.orientation.ToAxisRadian();
        UpdateChildrenOrientationRecursive(m_transform.orientation);
    }

    void TransformComponent::AddRotationA(Real radian)
    {
        m_axis_holder.radian += radian;
        m_transform.orientation.Set(m_axis_holder);
        UpdateChildrenOrientationRecursive(m_transform.orientation);
    }

    void TransformComponent::AddScale(const Vector3& delta_scale, eAnchor3D anchor)
    {
        if (anchor == eAnchor3D::CCC)
        {
            m_transform.scale += delta_scale;
        }
        else
        {
            auto curr_size = m_transform.scale.Half();
            auto new_size  = (m_transform.scale + delta_scale).Half();
            m_transform.position += (new_size - curr_size).HadamardProduct(Anchor::AnchorVector3(anchor));
            m_transform.scale += delta_scale;
        }
    }

    Vector3 TransformComponent::GetPosition() const
    {
        return m_transform.position;
    }

    Vector3 TransformComponent::GetScale() const
    {
        return m_transform.scale;
    }

    Quaternion TransformComponent::GetOrientation() const
    {
        return m_transform.orientation;
    }

    Matrix33 TransformComponent::GetRotationMatrix() const
    {
        return m_transform.orientation.ToMatrix();
    }

    Matrix44 TransformComponent::GetTransformMatrix() const
    {
        return m_transform.LocalToWorldMatrix();
    }

    Transform* TransformComponent::GetTransform()
    {
        return &m_transform;
    }

    Vector3 TransformComponent::LocalToWorldPoint(const Vector3& local_point) const
    {
        return m_transform.LocalToWorldPoint(local_point);
    }

    Vector3 TransformComponent::WorldToLocalPoint(const Vector3& world_point) const
    {
        return m_transform.WorldToLocalPoint(world_point);
    }

    Vector3 TransformComponent::LocalToWorldVector(const Vector3& local_vector) const
    {
        return m_transform.LocalToWorldVector(local_vector);
    }

    Vector3 TransformComponent::WorldToLocalVector(const Vector3& world_vector) const
    {
        return m_transform.WorldToLocalVector(world_vector);
    }

    bool TransformComponent::Load(const Json::Value& data)
    {
        if (JsonResource::HasMember(data, "Position"))
        {
            if (JsonResource::IsVector3(data["Position"]))
            {
                SetPosition(JsonResource::AsVector3(data["Position"]));
            }
            if (data["Position"].isString())
            {
                m_b_random_pos = true;
                SetPosition(
                            Vector3(
                                    random.GetRangedRandomReal(-50.0f, 50.0f),
                                    random.GetRangedRandomReal(-20.0f, 20.0f),
                                    random.GetRangedRandomReal(-50.0f, 50.0f)));
            }
        }
        if (JsonResource::HasMember(data, "Scale") && JsonResource::IsVector3(data["Scale"]))
        {
            SetScale(JsonResource::AsVector3(data["Scale"]));
        }
        if (JsonResource::HasMember(data, "Orientation"))
        {
            if (JsonResource::IsVector3(data["Orientation"]))
            {
                auto rotation = JsonResource::AsVector3(data["Orientation"]);
                SetOrientation(EulerAngle(rotation.x, rotation.y, rotation.z));
            }
            if (JsonResource::IsMatrix33(data["Orientation"]))
            {
                SetOrientation(JsonResource::AsMatrix33(data["Orientation"]));
            }
            if (JsonResource::IsQuaternion(data["Orientation"]))
            {
                SetOrientation(JsonResource::AsQuaternionRIJK(data["Orientation"]));
            }

            if (data["Orientation"].isString())
            {
                m_b_random_rot        = true;
                Quaternion random_rot = Quaternion(
                                                   random.GetRangedRandomReal(-1.0f, 1.0f),
                                                   random.GetRangedRandomReal(-1.0f, 1.0f),
                                                   random.GetRangedRandomReal(-1.0f, 1.0f),
                                                   random.GetRangedRandomReal(-1.0f, 1.0f));

                random_rot.SetNormalize();
                SetOrientation(random_rot);
            }
        }
        return true;
    }

    void TransformComponent::Save(Json::Value& data) const
    {
    }

    void TransformComponent::Edit(CommandRegistry* command_registry)
    {
        if (ImGui::CollapsingHeader(m_type.c_str(), &m_b_open))
        {
            float position[3]   = {m_transform.position.x, m_transform.position.y, m_transform.position.z};
            float scale[3]      = {m_transform.scale.x, m_transform.scale.y, m_transform.scale.z};
            float axis[3]       = {m_axis_holder.axis.x, m_axis_holder.axis.y, m_axis_holder.axis.z};
            float radian        = m_axis_holder.radian;
            float quaternion[4] = {m_transform.orientation.r, m_transform.orientation.i, m_transform.orientation.j, m_transform.orientation.k};
            ImGui::Separator();
            ImGui::Text("Position");
            ImGui::InputFloat3("##TransformEdit0", position, 3);
            if (ImGui::IsItemEdited())
            {
                Vector3 prev = m_transform.position;
                Vector3 next(position);
                command_registry->PushCommand(
                                              new EditFunction<
                                                  Vector3,
                                                  TransformComponent,
                                                  &TransformComponent::SetPosition>(this, prev, next));
            }
            ImGui::Separator();
            ImGui::Text("Scale");
            ImGui::InputFloat3("##TransformEdit1", scale, 3);
            if (ImGui::IsItemEdited() == true)
            {
                Vector3 prev = m_transform.scale;
                Vector3 next(scale);
                command_registry->PushCommand(
                                              new EditFunction<
                                                  Vector3,
                                                  TransformComponent,
                                                  &TransformComponent::SetScale>(this, prev, next));
            }
            ImGui::Separator();
            ImGui::Text("Orientation");

            ImGui::Text("Add Rotation");
            if (ImGui::Button("X + 90"))
            {
                AddRotationX(Math::HALF_PI);
            }
            ImGui::SameLine();
            if (ImGui::Button("Y + 90"))
            {
                AddRotationY(Math::HALF_PI);
            }
            ImGui::SameLine();
            if (ImGui::Button("Z + 90"))
            {
                AddRotationZ(Math::HALF_PI);
            }
            if (ImGui::Button("X - 90"))
            {
                AddRotationX(-Math::HALF_PI);
            }
            ImGui::SameLine();
            if (ImGui::Button("Y - 90"))
            {
                AddRotationY(-Math::HALF_PI);
            }
            ImGui::SameLine();
            if (ImGui::Button("Z - 90"))
            {
                AddRotationZ(-Math::HALF_PI);
            }

            ImGui::Text("Axis");
            ImGui::InputFloat3("##TransformEdit2", axis, 3);
            if (ImGui::IsItemEdited() == true)
            {
                AxisRadian prev = m_axis_holder;
                AxisRadian next = prev;
                next.axis.Set(axis[0], axis[1], axis[2]);
                next.axis.SetNormalize();
                command_registry->PushCommand(
                                              new EditFunction<
                                                  AxisRadian,
                                                  TransformComponent,
                                                  &TransformComponent::SetOrientation>(this, prev, next));
            }
            ImGui::Text("Radian");
            if (ImGui::SliderAngle("##TransformEdit3", &radian))
            {
                AxisRadian prev = m_axis_holder;
                prev.radian     = radian;
                m_transform.orientation.Set(prev);
            }
            if (ImGui::IsItemEdited())
            {
                m_edit_radian.axis   = m_axis_holder.axis;
                m_edit_radian.radian = radian;
            }
            if (ImGui::IsItemDeactivatedAfterEdit())
            {
                command_registry->PushCommand(
                                              new EditFunction<
                                                  AxisRadian,
                                                  TransformComponent,
                                                  &TransformComponent::SetOrientation>(this, m_axis_holder, m_edit_radian));
            }
            ImGui::Text("Quaternion");
            if (ImGui::SliderFloat4("##TransformEdit4", quaternion, -1.0f, 1.0f))
            {
                Quaternion edited(quaternion[0], quaternion[1], quaternion[2], quaternion[3]);
                edited.SetNormalize();
                m_transform.orientation.Set(edited);
            }
            if (ImGui::IsItemEdited())
            {
                m_edit_quaternion = Quaternion(quaternion[0], quaternion[1], quaternion[2], quaternion[3]);
            }
            if (ImGui::IsItemDeactivatedAfterEdit())
            {
                command_registry->PushCommand(
                                              new EditFunction<
                                                  Quaternion,
                                                  TransformComponent,
                                                  &TransformComponent::SetOrientation>(this, m_transform.orientation, m_edit_quaternion));
            }
            Real degree = Math::RadiansToDegrees(radian);
            ImGui::Text("R : %.3f [cos(%.f)]", quaternion[0], degree);
            ImGui::Text("I : %.3f [sin(%.f) * %.3fi]", quaternion[1], degree, axis[0]);
            ImGui::Text("J : %.3f [sin(%.f) * %.3fj]", quaternion[2], degree, axis[1]);
            ImGui::Text("K : %.3f [sin(%.f) * %.3fk]", quaternion[3], degree, axis[2]);
            ImGui::Separator();
        }
    }

    void TransformComponent::Subscribe()
    {
    }

    void TransformComponent::Unsubscribe()
    {
    }

    TransformComponent::TransformComponent(Object* owner)
        : Component(owner)
    {
    }

    void TransformComponent::Clone(TransformComponent* origin)
    {
        if (origin != nullptr && origin != this)
        {
            //copy data
            m_transform   = origin->m_transform;
            m_axis_holder = origin->m_axis_holder;

            if (origin->m_b_random_pos)
            {
                SetPosition(
                            Vector3(
                                    random.GetRangedRandomReal(-50.0f, 50.0f),
                                    random.GetRangedRandomReal(-20.0f, 20.0f),
                                    random.GetRangedRandomReal(-50.0f, 50.0f)));
            }

            if (origin->m_b_random_rot)
            {
                Quaternion random_rot = Quaternion(
                                                   random.GetRangedRandomReal(-1.0f, 1.0f),
                                                   random.GetRangedRandomReal(-1.0f, 1.0f),
                                                   random.GetRangedRandomReal(-1.0f, 1.0f),
                                                   random.GetRangedRandomReal(-1.0f, 1.0f));

                random_rot.SetNormalize();
                SetOrientation(random_rot);
            }
        }
    }

    void TransformComponent::UpdateChildrenPositionRecursive(const Vector3& position)
    {
    }

    void TransformComponent::UpdateChildrenOrientationRecursive(const Quaternion& orientation)
    {
    }
}
