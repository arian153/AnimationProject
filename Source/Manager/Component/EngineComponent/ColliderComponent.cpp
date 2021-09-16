#include "ColliderComponent.hpp"
#include "../../../System/Core/Utility/CoreUtility.hpp"
#include "../../Space/Space.hpp"
#include "../../Object/Object.hpp"
#include "RigidBodyComponent.hpp"
#include "TransformComponent.hpp"
#include "../../Resource/ResourceType/JsonResource.hpp"
#include "../../../External/JSONCPP/json/json.h"
#include "../../../System/Graphics/Utility/PrimitiveRenderer.hpp"

namespace CS460
{
    ColliderComponent::~ColliderComponent()
    {
    }

    void ColliderComponent::Initialize()
    {
        if (m_collider_set == nullptr)
        {
            World* world   = m_space != nullptr ? m_space->GetWorld() : nullptr;
            m_collider_set = new ColliderSet(world);
            m_collider_set->Initialize();
            m_collider_set->m_component = this;
            Subscribe();
        }
        if (m_rigid_body != nullptr)
        {
            m_collider_set->SetRigidBody(m_rigid_body);
            m_b_init = true;
        }
        else
        {
            if (m_owner->HasComponent<RigidBodyComponent>())
            {
                auto body            = m_owner->GetComponent<RigidBodyComponent>();
                m_rigid_body         = body->m_rigid_body;
                body->m_collider_set = m_collider_set;
                body->Initialize();
                m_collider_set->SetRigidBody(m_rigid_body);
                m_b_init = true;
            }
        }
        if (m_transform == nullptr)
        {
            if (m_owner->HasComponent<TransformComponent>())
            {
                m_transform = m_owner->GetComponent<TransformComponent>()->GetTransform();
            }
        }
    }

    void ColliderComponent::Update(Real dt)
    {
        E5_UNUSED_PARAM(dt);
    }

    void ColliderComponent::Shutdown()
    {
        Unsubscribe();
        if (m_collider_set != nullptr)
        {
            m_collider_set->Shutdown();
            delete m_collider_set;
            m_collider_set = nullptr;
        }
    }

    ColliderPrimitive* ColliderComponent::AddCollider(eColliderType type) const
    {
        return m_collider_set->AddCollider(type);
    }

    ColliderPrimitive* ColliderComponent::GetCollider(size_t index) const
    {
        return m_collider_set->GetCollider(index);
    }

    void ColliderComponent::EraseCollider(ColliderPrimitive* collider) const
    {
        m_collider_set->EraseCollider(collider);
    }

    void ColliderComponent::SetMass(Real density) const
    {
        m_collider_set->SetMass(density);
    }

    void ColliderComponent::SetScale(const Vector3& scale) const
    {
        m_collider_set->SetScale(scale);
        if (m_transform != nullptr)
        {
            m_transform->scale = scale;
        }
    }

    MassData ColliderComponent::GetMass() const
    {
        return m_collider_set->GetMassData();
    }

    Vector3 ColliderComponent::GetScale() const
    {
        return m_collider_set->GetScale();
    }

    void ColliderComponent::SetTransform(Transform* transform)
    {
        m_transform = transform;
    }

    void ColliderComponent::SetRigidBody(RigidBody* rigid_body)
    {
        m_rigid_body = rigid_body;
        m_collider_set->SetRigidBody(m_rigid_body);
    }

    void ColliderComponent::RenderBoundingVolume(PrimitiveRenderer* renderer, const Color& color) const
    {
        m_collider_set->UpdateBoundingVolume();
        renderer->DrawBox(
                          m_collider_set->m_bounding_volume.Center(), Quaternion(),
                          m_collider_set->m_bounding_volume.Size(), eRenderingMode::Line, color);
    }

    bool ColliderComponent::Load(const Json::Value& data)
    {
        if (JsonResource::HasMember(data, "Colliders") && data["Colliders"].isArray())
        {
            for (auto it = data["Colliders"].begin(); it != data["Colliders"].end(); ++it)
            {
                auto collider = *it;
                if (JsonResource::HasMember(collider, "Type") && collider["Type"].isString())
                {
                    std::string   type          = collider["Type"].asString();
                    eColliderType collider_type = eColliderType::InvalidPrimitive;
                    if (type == "Circle")
                    {
                        collider_type = eColliderType::Circle;
                    }
                    else if (type == "Ellipse")
                    {
                        collider_type = eColliderType::Ellipse;
                    }
                    else if (type == "Polygon")
                    {
                        collider_type = eColliderType::Polygon;
                    }
                    else if (type == "Rectangle")
                    {
                        collider_type = eColliderType::Rectangle;
                    }
                    else if (type == "Triangle")
                    {
                        collider_type = eColliderType::Triangle;
                    }
                    else if (type == "Box")
                    {
                        collider_type = eColliderType::Box;
                    }
                    else if (type == "Capsule")
                    {
                        collider_type = eColliderType::Capsule;
                    }
                    else if (type == "Cone")
                    {
                        collider_type = eColliderType::Cone;
                    }
                    else if (type == "Cylinder")
                    {
                        collider_type = eColliderType::Cylinder;
                    }
                    else if (type == "Dome")
                    {
                        collider_type = eColliderType::Dome;
                    }
                    else if (type == "Ellipsoid")
                    {
                        collider_type = eColliderType::Ellipsoid;
                    }
                    else if (type == "Polyhedron")
                    {
                        collider_type = eColliderType::Polyhedron;
                    }
                    else if (type == "Sphere")
                    {
                        collider_type = eColliderType::Sphere;
                    }
                    else if (type == "Tetrahedron")
                    {
                        collider_type = eColliderType::Tetrahedron;
                    }
                    else if (type == "Truncated")
                    {
                        collider_type = eColliderType::Truncated;
                    }
                    if (collider_type != eColliderType::InvalidPrimitive)
                    {
                        auto created = AddCollider(collider_type);
                        created->Load(collider);
                    }
                }
            }
            m_collider_set->UpdateMassData();
            m_collider_set->UpdateBoundingVolume();
        }
        return true;
    }

    void ColliderComponent::Save(Json::Value& data) const
    {
    }

    void ColliderComponent::Edit(CommandRegistry* command_registry)
    {
        ImGui::CollapsingHeader(m_type.c_str(), &m_b_open);
    }

    void ColliderComponent::Subscribe()
    {
        if (m_space != nullptr && m_collider_set != nullptr)
        {
            m_space->GetWorld()->AddColliderSet(m_collider_set);
        }
    }

    void ColliderComponent::Unsubscribe()
    {
        if (m_space != nullptr && m_collider_set != nullptr)
        {
            m_space->GetWorld()->RemoveColliderSet(m_collider_set);
        }
    }

    ColliderComponent::ColliderComponent(Object* owner)
        : Component(owner)
    {
    }

    void ColliderComponent::Clone(ColliderComponent* origin)
    {
        if (origin != nullptr && origin != this)
        {
            Initialize();
            World* world = m_space != nullptr ? m_space->GetWorld() : nullptr;
            m_collider_set->Clone(origin->m_collider_set, m_rigid_body, world);
        }
    }
}
