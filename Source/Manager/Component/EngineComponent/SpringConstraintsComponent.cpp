#include "SpringConstraintsComponent.hpp"

#include "ColliderComponent.hpp"
#include "RigidBodyComponent.hpp"
#include "TransformComponent.hpp"
#include "../../../External/JSONCPP/json/json.h"
#include "../../../System/Core/Utility/CoreUtility.hpp"
#include "../../../System/Physics/Dynamics/World.hpp"
#include "../../../System/Physics/Resolution/Constraints/SpringConstraint.hpp"
#include "../../Object/Object.hpp"
#include "../../Object/ObjectManager.hpp"
#include "../../Resource/ResourceType/JsonResource.hpp"
#include "../../Space/Space.hpp"

namespace CS460
{
    SpringConstraintsComponent::~SpringConstraintsComponent()
    {
    }

    void SpringConstraintsComponent::Initialize()
    {
        if (m_spring_constraint == nullptr)
        {
            m_spring_constraint              = new SpringConstraint();
            m_spring_constraint->m_component = this;
            Subscribe();
        }
    }

    void SpringConstraintsComponent::Update(Real dt)
    {
        E5_UNUSED_PARAM(dt);
    }

    void SpringConstraintsComponent::Shutdown()
    {
        Unsubscribe();
        if (m_spring_constraint != nullptr)
        {
            m_spring_constraint->Shutdown();
            delete m_spring_constraint;
            m_spring_constraint = nullptr;
        }
    }

    bool SpringConstraintsComponent::Load(const Json::Value& data)
    {
        if (JsonResource::HasMember(data, "Target Object") && data["Target Object"].isString())
        {
            std::string name  = data["Target Object"].asString();
            auto        found = m_space->GetObjectManager()->FindObjectBegin(name);

            if (found != nullptr && found->HasComponent<RigidBodyComponent>())
            {
                m_target_a = found;
            }
        }

        if (JsonResource::HasMember(data, "Anchor Object") && data["Anchor Object"].isString())
        {
            std::string name  = data["Anchor Object"].asString();
            auto        found = m_space->GetObjectManager()->FindObjectBegin(name);
            if (found != nullptr && found->HasComponent<TransformComponent>())
            {
                m_target_b   = found;
                m_has_anchor = true;
                //m_spring_constraint->m_anchor = found->GetComponent<TransformComponent>()->GetTransform();
            }
        }

        if (JsonResource::HasMember(data, "Target Object A") && data["Target Object A"].isString())
        {
            std::string name = data["Target Object A"].asString();
            auto        found = m_space->GetObjectManager()->FindObjectBegin(name);

            if (found != nullptr && found->HasComponent<RigidBodyComponent>())
            {
                m_target_a = found;
            }
        }

        if (JsonResource::HasMember(data, "Target Object B") && data["Target Object B"].isString())
        {
            std::string name = data["Target Object B"].asString();
            auto        found = m_space->GetObjectManager()->FindObjectBegin(name);

            if (found != nullptr && found->HasComponent<RigidBodyComponent>())
            {
                m_target_b = found;
            }
        }


        if (JsonResource::HasMember(data, "A Box Vertex ID") && data["A Box Vertex ID"].isInt())
        {
            target_a_idx = data["A Box Vertex ID"].asLargestUInt();
        }

        if (JsonResource::HasMember(data, "B Box Vertex ID") && data["B Box Vertex ID"].isInt())
        {
            target_b_idx = data["B Box Vertex ID"].asLargestUInt();
        }

        SetUpConstraints();
        return true;
    }

    void SpringConstraintsComponent::Save(Json::Value& data) const
    {
    }

    void SpringConstraintsComponent::Edit(CommandRegistry* command_registry)
    {
        ImGui::CollapsingHeader(m_type.c_str(), &m_b_open);
    }

    void SpringConstraintsComponent::Subscribe()
    {
        if (m_space != nullptr && m_spring_constraint != nullptr)
        {
            m_space->GetWorld()->AddConstraint(m_spring_constraint);
        }
    }

    void SpringConstraintsComponent::Unsubscribe()
    {
        if (m_space != nullptr && m_spring_constraint != nullptr)
        {
            m_space->GetWorld()->RemoveConstraint(m_spring_constraint);
        }
    }

    void SpringConstraintsComponent::SetUpConstraints() const
    {
        if (m_has_anchor)
        {
            Vector3 local_q;

            if (m_target_a->HasComponent<ColliderComponent>())
            {
                auto collider = m_target_a->GetComponent<ColliderComponent>()->GetCollider(0);
                if (collider != nullptr && collider->Type() == eColliderType::Box)
                {
                    ColliderBox* box = (ColliderBox*)collider;
                    local_q          = box->Vertex(target_a_idx);
                }
            }

            m_spring_constraint->SetUp(
                                       m_target_b->GetComponent<TransformComponent>()->GetTransform(),
                                       m_target_a->GetComponent<RigidBodyComponent>()->GetRigidBody(),
                                       local_q
                                      );
        }
        else
        {
            Vector3 local_qa;
            Vector3 local_qb;
            if (m_target_a->HasComponent<ColliderComponent>())
            {
                auto collider = m_target_a->GetComponent<ColliderComponent>()->GetCollider(0);
                if (collider != nullptr && collider->Type() == eColliderType::Box)
                {
                    ColliderBox* box = (ColliderBox*)collider;
                    local_qa         = box->Vertex(target_a_idx);
                }
            }

            if (m_target_b->HasComponent<ColliderComponent>())
            {
                auto collider = m_target_b->GetComponent<ColliderComponent>()->GetCollider(0);
                if (collider != nullptr && collider->Type() == eColliderType::Box)
                {
                    ColliderBox* box = (ColliderBox*)collider;
                    local_qb         = box->Vertex(target_b_idx);
                }
            }

            m_spring_constraint->SetUp(
                                       m_target_a->GetComponent<RigidBodyComponent>()->GetRigidBody(),
                                       m_target_b->GetComponent<RigidBodyComponent>()->GetRigidBody(),
                                       local_qa,
                                       local_qb
                                      );
        }
    }

    SpringConstraintsComponent::SpringConstraintsComponent(Object* owner)
        : Component(owner)
    {
    }

    void SpringConstraintsComponent::Clone(SpringConstraintsComponent* origin)
    {
    }
}
