// ReSharper disable CppMemberFunctionMayBeConst
#include "RigidBodyComponent.hpp"

#include <iostream>

#include "../../Space/Space.hpp"
#include "../../Object/Object.hpp"
#include "ColliderComponent.hpp"
#include "TransformComponent.hpp"
#include "../../Resource/ResourceType/JsonResource.hpp"
#include "../../../External/JSONCPP/json/json.h"
#include "../../../System/Core/Utility/CoreUtility.hpp"
#include "../../../System/GUI/Editor/Command/EditorCommand.hpp"
#include "../../../System/GUI/Editor/Command/CommandRegistry.hpp"

namespace CS460
{
    RigidBodyComponent::~RigidBodyComponent()
    {
    }

    void RigidBodyComponent::Initialize()
    {
        if (m_rigid_body == nullptr)
        {
            m_rigid_body              = new RigidBody();
            m_rigid_body->m_component = this;
            Subscribe();
        }
        if (m_collider_set != nullptr)
        {
            m_b_init = true;
        }
        else
        {
            if (m_owner->HasComponent<ColliderComponent>())
            {
                auto collider          = m_owner->GetComponent<ColliderComponent>();
                m_collider_set         = collider->m_collider_set;
                collider->m_rigid_body = m_rigid_body;
                collider->Initialize();
                m_b_init = true;
            }
        }
        if (m_transform == nullptr)
        {
            if (m_owner->HasComponent<TransformComponent>())
            {
                m_transform = m_owner->GetComponent<TransformComponent>()->GetTransform();
                m_rigid_body->SetTransform(m_transform);
                m_rigid_body->SyncFromTransform(m_transform);
            }
        }
    }

    void RigidBodyComponent::Update(Real dt)
    {
        E5_UNUSED_PARAM(dt);
    }

    void RigidBodyComponent::Shutdown()
    {
        Unsubscribe();
        if (m_rigid_body != nullptr)
        {
            m_rigid_body->Shutdown();
            delete m_rigid_body;
            m_rigid_body = nullptr;
        }
    }

    void RigidBodyComponent::ApplyForce(const Vector3& force, const Vector3& at) const
    {
        m_rigid_body->ApplyForce(force, at);
    }

    void RigidBodyComponent::ApplyForceCentroid(const Vector3& force) const
    {
        m_rigid_body->ApplyForceCentroid(force);
    }

    void RigidBodyComponent::SetLinearVelocity(const Vector3& linear) const
    {
        m_rigid_body->SetLinearVelocity(linear);
    }

    void RigidBodyComponent::SetAngularVelocity(const Vector3& angular) const
    {
        m_rigid_body->SetAngularVelocity(angular);
    }

    void RigidBodyComponent::SetMassInfinite() const
    {
        m_rigid_body->SetMassInfinite();
        m_rigid_body->SetInertiaInfinite();
    }

    void RigidBodyComponent::SetMass(Real mass) const
    {
        auto prev_inertia  = m_rigid_body->LocalInertia();
        auto prev_inv_mass = m_rigid_body->InverseMass();
        auto new_inertia   = (mass * prev_inv_mass) * prev_inertia;
        m_rigid_body->SetMass(mass);
        m_rigid_body->SetLocalInertia(new_inertia);
    }

    void RigidBodyComponent::SetMass(const Real& mass)
    {
        if (mass > 0.0f)
        {
            Real     prev_inv_mass = m_rigid_body->InverseMass();
            Matrix33 new_inertia   = Math::IsZero(prev_inv_mass) ? mass * m_prev_inertia : (mass * prev_inv_mass) * m_rigid_body->LocalInertia();
            m_rigid_body->SetMass(mass);
            m_rigid_body->SetLocalInertia(new_inertia);
        }
        else
        {
            m_prev_inertia = m_rigid_body->InverseMass() * m_rigid_body->LocalInertia();
            m_rigid_body->SetMassInfinite();
            m_rigid_body->SetInertiaInfinite();
        }
    }

    void RigidBodyComponent::SetPositionalConstraints(const Vector3& linear) const
    {
        m_rigid_body->SetPositionalConstraints(linear);
    }

    void RigidBodyComponent::SetRotationalConstraints(const Vector3& angular) const
    {
        m_rigid_body->SetRotationalConstraints(angular);
    }

    void RigidBodyComponent::SetMotionMode(const eMotionMode& motion_mode)
    {
        m_rigid_body->SetMotionMode(motion_mode);
    }

    void RigidBodyComponent::SetTransform(Transform* transform)
    {
        m_transform = transform;
    }

    Vector3 RigidBodyComponent::GetLinearVelocity() const
    {
        return m_rigid_body->GetLinearVelocity();
    }

    Vector3 RigidBodyComponent::GetAngularVelocity() const
    {
        return m_rigid_body->GetAngularVelocity();
    }

    Real RigidBodyComponent::GetMass() const
    {
        return m_rigid_body->Mass();
    }

    Real RigidBodyComponent::GetInverseMass() const
    {
        return m_rigid_body->InverseMass();
    }

    Matrix33 RigidBodyComponent::GetMassMatrix() const
    {
        return m_rigid_body->MassMatrix();
    }

    Matrix33 RigidBodyComponent::GetInverseMassMatrix() const
    {
        return m_rigid_body->InverseMassMatrix();
    }

    Matrix33 RigidBodyComponent::GetInertia() const
    {
        return m_rigid_body->Inertia();
    }

    Matrix33 RigidBodyComponent::GetInverseInertia() const
    {
        return m_rigid_body->InverseInertia();
    }

    eMotionMode RigidBodyComponent::GetMotionMode() const
    {
        return m_rigid_body->GetMotionMode();
    }

    RigidBody* RigidBodyComponent::GetRigidBody() const
    {
        return m_rigid_body;
    }

    bool RigidBodyComponent::Load(const Json::Value& data)
    {
        if (JsonResource::HasMember(data, "Motion") && (data["Motion"].isString()))
        {
            std::string motion = data["Motion"].asString();
            if (motion == "Dynamic")
            {
                m_rigid_body->m_motion_mode = eMotionMode::Dynamic;
            }
            else if (motion == "Kinematic")
            {
                m_rigid_body->m_motion_mode = eMotionMode::Kinematic;
            }
            else if (motion == "Static")
            {
                m_rigid_body->m_motion_mode = eMotionMode::Static;
            }
            else
            {
                m_rigid_body->m_motion_mode = eMotionMode::Dynamic;
            }
        }
        //linear data
        if (JsonResource::HasMember(data, "Linear"))
        {
            auto linear = data["Linear"];
            if (JsonResource::HasMember(linear, "Position") && JsonResource::IsVector3(linear["Position"]))
            {
                m_rigid_body->m_shared_data->position = JsonResource::AsVector3(linear["Position"]);
            }
            if (JsonResource::HasMember(linear, "Velocity") && JsonResource::IsVector3(linear["Velocity"]))
            {
                m_rigid_body->m_linear_velocity = JsonResource::AsVector3(linear["Velocity"]);
            }
            if (JsonResource::HasMember(linear, "Force") && JsonResource::IsVector3(linear["Force"]))
            {
                m_rigid_body->m_force_accumulator = JsonResource::AsVector3(linear["Force"]);
            }
            if (JsonResource::HasMember(linear, "Constraints") && JsonResource::IsVector3(linear["Constraints"]))
            {
                m_rigid_body->m_linear_constraints = JsonResource::AsVector3(linear["Constraints"]);
            }
        }
        //angular data
        if (JsonResource::HasMember(data, "Angular"))
        {
            auto angular = data["Angular"];
            if (JsonResource::HasMember(angular, "Orientation") && JsonResource::IsQuaternion(angular["Orientation"]))
            {
                m_rigid_body->SetOrientation(JsonResource::AsQuaternionRIJK(angular["Orientation"]));
            }
            if (JsonResource::HasMember(angular, "Velocity") && JsonResource::IsVector3(angular["Velocity"]))
            {
                m_rigid_body->m_angular_velocity = JsonResource::AsVector3(angular["Velocity"]);
            }
            if (JsonResource::HasMember(angular, "Torque") && JsonResource::IsVector3(angular["Torque"]))
            {
                m_rigid_body->m_torque_accumulator = JsonResource::AsVector3(angular["Torque"]);
            }
            if (JsonResource::HasMember(angular, "Constraints") && JsonResource::IsVector3(angular["Constraints"]))
            {
                m_rigid_body->m_angular_constraints = JsonResource::AsVector3(angular["Constraints"]);
            }
        }
        //mass data
        if (JsonResource::HasMember(data, "Mass"))
        {
            auto mass_data = data["Mass"];
            if (JsonResource::HasMember(mass_data, "Mass") && mass_data["Mass"].isDouble())
            {
                Real mass = mass_data["Mass"].asFloat();
                Math::IsZero(mass)
                    ? m_rigid_body->SetMassInfinite()
                    : m_rigid_body->SetMass(mass);
            }
            if (JsonResource::HasMember(mass_data, "Inertia") && JsonResource::IsMatrix33(mass_data["Inertia"]))
            {
                Matrix33 inertia_tensor = JsonResource::AsMatrix33(mass_data["Inertia"]);
                inertia_tensor.IsZero()
                    ? m_rigid_body->SetInertiaInfinite()
                    : m_rigid_body->SetLocalInertia(inertia_tensor);
            }
            if (JsonResource::HasMember(mass_data, "Centroid") && JsonResource::IsVector3(mass_data["Centroid"]))
            {
                m_rigid_body->m_mass_data.local_centroid = JsonResource::AsVector3(mass_data["Centroid"]);
                m_rigid_body->UpdateCentroid();
            }
        }
        m_rigid_body->UpdateInertia();
        return true;
    }

    void RigidBodyComponent::Save(Json::Value& data) const
    {
    }

    void RigidBodyComponent::Edit(CommandRegistry* command_registry)
    {
        if (ImGui::CollapsingHeader(m_type.c_str(), &m_b_open))
        {
            ImGui::Text("Movement");
            ImGui::Separator();
            ImGui::Text("Linear Velocity");
            Vector3 linear_velocity = m_rigid_body->m_linear_velocity;
            ImGui::InputFloat3("##RigidBodyEdit0", linear_velocity.GetData(), 3);
            if (ImGui::IsItemEdited())
            {
                m_rigid_body->SetAwake();
                command_registry->PushCommand(
                                              new EditFunction<
                                                  Vector3,
                                                  RigidBody,
                                                  &RigidBody::SetLinearVelocity>(
                                                                                 m_rigid_body,
                                                                                 m_rigid_body->m_linear_velocity,
                                                                                 linear_velocity
                                                                                )
                                             );
            }
            ImGui::Text("Angular Velocity");
            Vector3 angular_velocity = m_rigid_body->m_angular_velocity;
            ImGui::InputFloat3("##RigidBodyEdit1", angular_velocity.GetData(), 3);
            if (ImGui::IsItemEdited())
            {
                m_rigid_body->SetAwake();
                command_registry->PushCommand(
                                              new EditFunction<
                                                  Vector3,
                                                  RigidBody,
                                                  &RigidBody::SetAngularVelocity>(
                                                                                  m_rigid_body,
                                                                                  m_rigid_body->m_angular_velocity,
                                                                                  angular_velocity
                                                                                 )
                                             );
            }
            ImGui::Separator();
            ImGui::Text("Force");
            Vector3 force_accumulator = m_rigid_body->m_force_accumulator;
            ImGui::InputFloat3("##RigidBodyEdit2", force_accumulator.GetData(), 3);
            if (ImGui::IsItemEdited())
            {
                command_registry->PushCommand(
                                              new EditFunction<
                                                  Vector3,
                                                  RigidBody,
                                                  &RigidBody::ApplyForceCentroid>(
                                                                                  m_rigid_body,
                                                                                  m_rigid_body->m_force_accumulator,
                                                                                  force_accumulator
                                                                                 )
                                             );
            }
            ImGui::Text("Torque");
            Vector3 torque_accumulator = m_rigid_body->m_torque_accumulator;
            ImGui::InputFloat3("##RigidBodyEdit3", torque_accumulator.GetData(), 3);
            if (ImGui::IsItemEdited())
            {
                command_registry->PushCommand(
                                              new EditFunction<
                                                  Vector3,
                                                  RigidBody,
                                                  &RigidBody::ApplyTorque>(
                                                                           m_rigid_body,
                                                                           m_rigid_body->m_torque_accumulator,
                                                                           torque_accumulator
                                                                          )
                                             );
            }
            ImGui::Separator();
            ImGui::Text("Physics Coefficient");
            ImGui::Separator();
            ImGui::Text("Drag");
            ImGui::Text("Linear Drag");
            ImGui::Text("Angular Drag");
            ImGui::Separator();
            ImGui::Text("Constraints");
            ImGui::Text("Linear Constraints");
            Vector3 linear_constraints = m_rigid_body->m_linear_constraints;
            ImGui::InputFloat3("##RigidBodyEdit6", linear_constraints.GetData(), 3);
            if (ImGui::IsItemEdited())
            {
                command_registry->PushCommand(
                                              new EditFunction<
                                                  Vector3,
                                                  RigidBody,
                                                  &RigidBody::SetPositionalConstraints>(
                                                                                        m_rigid_body,
                                                                                        m_rigid_body->m_linear_constraints,
                                                                                        linear_constraints
                                                                                       )
                                             );
            }
            ImGui::Text("Angular Constraints");
            Vector3 angular_constraints = m_rigid_body->m_angular_constraints;
            ImGui::InputFloat3("##RigidBodyEdit7", angular_constraints.GetData(), 3);
            if (ImGui::IsItemEdited())
            {
                command_registry->PushCommand(
                                              new EditFunction<
                                                  Vector3,
                                                  RigidBody,
                                                  &RigidBody::SetRotationalConstraints>(
                                                                                        m_rigid_body,
                                                                                        m_rigid_body->m_angular_constraints,
                                                                                        angular_constraints
                                                                                       )
                                             );
            }
            ImGui::Separator();
            ImGui::Text("Mass");
            Real mass = m_rigid_body->m_mass_data.mass;
            ImGui::InputFloat("##RigidBodyEdit8", &mass, 0.1f);
            if (ImGui::IsItemEdited())
            {
                command_registry->PushCommand(
                                              new EditFunction<
                                                  Real,
                                                  RigidBodyComponent,
                                                  &RigidBodyComponent::SetMass>(
                                                                                this,
                                                                                m_rigid_body->m_mass_data.mass,
                                                                                mass
                                                                               )
                                             );
            }

            ImGui::Separator();
            if (ImGui::TreeNode("Momentum & Kinetic Energy"))
            {
                Vector3 p = m_rigid_body->m_mass_data.mass * m_rigid_body->m_linear_velocity;
                ImGui::Text("Linear Momentum");
                ImGui::Text("[%.3f, %.3f, %.3f]", p[0], p[1], p[2]);

                Vector3 l = m_rigid_body->m_mass_data.local_inertia * m_rigid_body->m_angular_velocity;
                ImGui::Text("Angular Momentum");
                ImGui::Text("[%.3f, %.3f, %.3f]", l[0], l[1], l[2]);

                ImGui::Text("Linear Kinetic Energy");
                ImGui::Text("%.3f", 0.5f * m_rigid_body->m_mass_data.mass * m_rigid_body->m_linear_velocity * m_rigid_body->m_linear_velocity);

                ImGui::Text("Angular Kinetic Energy");
                ImGui::Text("%.3f", 0.5f * m_rigid_body->m_mass_data.local_inertia * m_rigid_body->m_angular_velocity * m_rigid_body->m_angular_velocity);
                ImGui::TreePop();
            }
            ImGui::Separator();
            if (ImGui::TreeNode("Mass Data"))
            {
                ImGui::Text("Mass");
                ImGui::Text("%.1f", m_rigid_body->m_mass_data.mass);
                ImGui::Text("Inverse Mass");
                ImGui::Text("%f", m_rigid_body->m_mass_data.inverse_mass);
                Vector3 c = m_rigid_body->m_mass_data.local_centroid;
                ImGui::Text("Centroid - Local");
                ImGui::Text("[%.1f, %.1f, %.1f]", c[0], c[1], c[2]);
                c = m_rigid_body->m_global_centroid;
                ImGui::Text("Centroid - Global");
                ImGui::Text("[%.1f, %.1f, %.1f]", c[0], c[1], c[2]);
                Matrix33 i = m_rigid_body->m_mass_data.local_inertia;
                ImGui::Text("Inertia - Local");
                ImGui::Text("|%.1f, %.1f, %.1f|", i[0], i[1], i[2]);
                ImGui::Text("|%.1f, %.1f, %.1f|", i[3], i[4], i[5]);
                ImGui::Text("|%.1f, %.1f, %.1f|", i[6], i[7], i[8]);
                i = m_rigid_body->m_global_inertia;
                ImGui::Text("Inertia - Global");
                ImGui::Text("|%.1f, %.1f, %.1f|", i[0], i[1], i[2]);
                ImGui::Text("|%.1f, %.1f, %.1f|", i[3], i[4], i[5]);
                ImGui::Text("|%.1f, %.1f, %.1f|", i[6], i[7], i[8]);
                i = m_rigid_body->m_mass_data.local_inverse_inertia;
                ImGui::Text("Inverse Inertia - Local");
                ImGui::Text("|%f, %f, %f|", i[0], i[1], i[2]);
                ImGui::Text("|%f, %f, %f|", i[3], i[4], i[5]);
                ImGui::Text("|%f, %f, %f|", i[6], i[7], i[8]);
                i = m_rigid_body->m_global_inverse_inertia;
                ImGui::Text("Inverse Inertia - Global");
                ImGui::Text("|%f, %f, %f|", i[0], i[1], i[2]);
                ImGui::Text("|%f, %f, %f|", i[3], i[4], i[5]);
                ImGui::Text("|%f, %f, %f|", i[6], i[7], i[8]);
                ImGui::Separator();
                ImGui::TreePop();
            }
            ImGui::Separator();
            ImGui::Text("Motion Mode");
            const char* motion_modes[] = {"Dynamic", "Static", "Kinematic"};
            m_motion_index             = (int)m_rigid_body->m_motion_mode;
            if (ImGui::Combo("##RigidBodyEdit9", &m_motion_index, motion_modes, 3))
            {
                command_registry->PushCommand(
                                              new EditFunction<
                                                  eMotionMode,
                                                  RigidBodyComponent,
                                                  &RigidBodyComponent::SetMotionMode>(
                                                                                      this,
                                                                                      m_rigid_body->m_motion_mode,
                                                                                      (eMotionMode)m_motion_index
                                                                                     )
                                             );
            }
            ImGui::Separator();
            ImGui::Text("Detection Mode");
            const char* detection_modes[] = {"Discrete", "Continuous"};
            if (ImGui::Combo("##RigidBodyEdit10", &m_detection_index, detection_modes, 2))
            {
                //do nothing yet.
            }
            //10
            ImGui::Separator();
            ImGui::Separator();
            ImGui::Text("Local Transform");
            ImGui::Separator();
            ImGui::Text("Position");
            Vector3 p = m_rigid_body->m_transform.position;
            ImGui::Text("[%.3f, %.3f, %.3f]", p[0], p[1], p[2]);
            ImGui::Text("Orientation");
            Quaternion o = m_rigid_body->m_transform.orientation;
            ImGui::Text("[%.3f, %.3f, %.3f, %.3f]", o.r, o.i, o.j, o.k);
            ImGui::Separator();
        }
    }

    void RigidBodyComponent::Subscribe()
    {
        if (m_space != nullptr && m_rigid_body != nullptr)
        {
            m_space->GetWorld()->AddRigidBody(m_rigid_body);
        }
    }

    void RigidBodyComponent::Unsubscribe()
    {
        if (m_space != nullptr && m_rigid_body != nullptr)
        {
            m_space->GetWorld()->RemoveRigidBody(m_rigid_body);
        }
    }

    RigidBodyComponent::RigidBodyComponent(Object* owner)
        : Component(owner)
    {
    }

    void RigidBodyComponent::Clone(RigidBodyComponent* origin)
    {
        if (origin != nullptr && origin != this)
        {
            //copy data
            Initialize();
            m_rigid_body->Clone(origin->m_rigid_body);
        }
    }
}
