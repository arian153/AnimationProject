#include "ColliderPrimitive.hpp"

#include "../../../External/imgui/imgui.h"
#include "../../../Manager/Resource/ResourceType/JsonResource.hpp"
#include "../../../External/JSONCPP/json/json.h"
#include "../../GUI/Editor/Command/EditorCommand.hpp"
#include "../NarrowPhase/Simplex.hpp"

namespace CS460
{
    ColliderPrimitive::ColliderPrimitive()
        : m_mass(1.0f), m_type(eColliderType::InvalidPrimitive)
    {
    }

    ColliderPrimitive::~ColliderPrimitive()
    {
    }

    void ColliderPrimitive::CastRay(RayCastResult& result, Real max_distance)
    {
        result.hit_data.collider = this;
        Ray body_ray;
        if (m_rigid_body != nullptr)
        {
            body_ray.position  = m_rigid_body->WorldToLocalPoint(result.ray.position);
            body_ray.direction = m_rigid_body->WorldToLocalVector(result.ray.direction);
        }
        else
        {
            body_ray = result.ray;
        }
        Ray  local_ray(WorldToLocalPoint(body_ray.position), WorldToLocalVector(body_ray.direction));
        Real minimum_t = -1.0f;
        Real maximum_t = -1.0f;
        if (this->TestRayIntersection(local_ray, minimum_t, maximum_t) == true)
        {
            result.hit_data.t   = minimum_t;
            result.hit_data.hit = true;
            //ray cast done
            Vector3 local_intersection   = local_ray.position + local_ray.direction * result.hit_data.t;
            result.hit_data.intersection = result.ray.position + result.ray.direction * result.hit_data.t;
            result.hit_data.normal       = LocalToWorldVector(GetNormal(local_intersection));
            if (m_rigid_body != nullptr)
            {
                result.hit_data.normal = m_rigid_body->LocalToWorldVector(result.hit_data.normal);
            }
            if (result.hit_data.t > max_distance && max_distance >= 0.0f)
            {
                result.hit_data.hit = false;
            }
        }
    }

    void ColliderPrimitive::IntersectRay(RayIntersectionResult& result, Real max_distance)
    {
        Ray body_ray;
        if (m_rigid_body != nullptr)
        {
            body_ray.position  = m_rigid_body->WorldToLocalPoint(result.ray.position);
            body_ray.direction = m_rigid_body->WorldToLocalVector(result.ray.direction);
        }
        else
        {
            body_ray = result.ray;
        }
        Ray     local_ray(WorldToLocalPoint(body_ray.position), WorldToLocalVector(body_ray.direction));
        HitData hit_data_a;
        HitData hit_data_b;
        Real    minimum_t = -1.0f;
        Real    maximum_t = -1.0f;
        if (TestRayIntersection(local_ray, minimum_t, maximum_t) == true)
        {
            hit_data_a.t        = minimum_t;
            hit_data_a.collider = this;
            hit_data_b.t        = maximum_t;
            hit_data_b.collider = this;
            //ray cast done
            Vector3 local_intersection_a = local_ray.position + local_ray.direction * hit_data_a.t;
            hit_data_a.intersection      = result.ray.position + result.ray.direction * hit_data_a.t;
            hit_data_a.normal            = LocalToWorldVector(GetNormal(local_intersection_a));
            if (m_rigid_body != nullptr)
            {
                hit_data_a.normal = m_rigid_body->LocalToWorldVector(hit_data_a.normal);
            }
            if (Math::IsEqual(hit_data_a.t, hit_data_b.t))
            {
                if (hit_data_a.t > max_distance || max_distance < 0.0f)
                {
                    hit_data_a.hit = true;
                    result.hit_list.push_back(hit_data_a);
                }
            }
            else
            {
                Vector3 local_intersection_b = local_ray.position + local_ray.direction * hit_data_b.t;
                hit_data_b.intersection      = result.ray.position + result.ray.direction * hit_data_b.t;
                hit_data_b.normal            = LocalToWorldVector(GetNormal(local_intersection_b));
                if (m_rigid_body != nullptr)
                {
                    hit_data_b.normal = m_rigid_body->LocalToWorldVector(hit_data_b.normal);
                }
                if (hit_data_b.t < hit_data_a.t)
                {
                    auto temp  = hit_data_a;
                    hit_data_a = hit_data_b;
                    hit_data_b = temp;
                }
                if (hit_data_a.t < max_distance || max_distance < 0.0f)
                {
                    hit_data_a.hit = true;
                    result.hit_list.push_back(hit_data_a);
                }
                if (hit_data_b.t < max_distance || max_distance < 0.0f)
                {
                    hit_data_b.hit = true;
                    result.hit_list.push_back(hit_data_b);
                }
            }
        }
    }

    Vector3 ColliderPrimitive::LocalToWorldPoint(const Vector3& local_point) const
    {
        return m_local.LocalToWorldPoint(local_point);
    }

    Vector3 ColliderPrimitive::WorldToLocalPoint(const Vector3& world_point) const
    {
        return m_local.WorldToLocalPoint(world_point);
    }

    Vector3 ColliderPrimitive::LocalToWorldVector(const Vector3& local_vector) const
    {
        return m_local.LocalToWorldVector(local_vector);
    }

    Vector3 ColliderPrimitive::WorldToLocalVector(const Vector3& world_vector) const
    {
        return m_local.WorldToLocalVector(world_vector);
    }

    Vector3 ColliderPrimitive::CastPointWorld(const Vector3& point)
    {
        Ray           ray(point, (GetBodyPosition() - point).Unit());
        RayCastResult result(ray);
        CastRay(result);

        return result.GetWorldIntersection();
    }

    Vector3 ColliderPrimitive::CastPointLocal(const Vector3& point)
    {
        Ray           ray(point, (GetBodyPosition() - point).Unit());
        RayCastResult result(ray);
        CastRay(result);

        return result.GetLocalIntersection();
    }

    Vector3 ColliderPrimitive::ClosestPointSimplex(const Vector3& point, Simplex& simplex)
    {
        m_collider_set->GJKDistanceTest(this, point, simplex);
        return simplex.ClosestPointGlobal(Vector3());
    }

    Matrix33 ColliderPrimitive::WorldInertia() const
    {
        return MassData::TranslateInertia(MassData::RotateInertia(m_local_inertia_tensor, m_local.orientation), m_mass, m_local.position);
    }

    Vector3 ColliderPrimitive::WorldCentroid() const
    {
        return m_local.LocalToWorldPoint(m_centroid);
    }

    Vector3 ColliderPrimitive::GetBodyPosition() const
    {
        if (m_rigid_body != nullptr)
        {
            return m_rigid_body->m_local.position;
        }
        return Vector3();
    }

    Quaternion ColliderPrimitive::GetBodyOrientation() const
    {
        if (m_rigid_body != nullptr)
        {
            return m_rigid_body->m_local.orientation;
        }
        return Quaternion();
    }

    Transform* ColliderPrimitive::GetBodyTransform() const
    {
        if (m_rigid_body != nullptr)
        {
            return m_rigid_body->m_transform;
        }
        return nullptr;
    }

    RigidBody* ColliderPrimitive::GetRigidBody() const
    {
        return m_rigid_body;
    }

    BoundingAABB* ColliderPrimitive::GetBoundingVolume() const
    {
        return m_bounding_volume;
    }

    ColliderSet* ColliderPrimitive::GetColliderSet() const
    {
        return m_collider_set;
    }

    Real ColliderPrimitive::GetDensity() const
    {
        return m_material.density;
    }

    Vector3 ColliderPrimitive::ConvertBodyWorldPoint(const Vector3& local_point) const
    {
        Vector3 result = m_local.orientation.Rotate(local_point);
        result += m_local.position;
        result = GetBodyOrientation().Rotate(result);
        result += GetBodyPosition();
        return result;
    }

    void ColliderPrimitive::SetScale(const Vector3& scale)
    {
        SetScaleData(scale);
        UpdateScaleData();
    }

    void ColliderPrimitive::SetMass(Real density)
    {
        SetMassData(density);
        UpdateMassData();
    }

    void ColliderPrimitive::SetMaterial(Physics::eMaterial material, Real d, Real r)
    {
        m_material = Physics::MaterialData(material, d, r);
    }

    Physics::eMaterial ColliderPrimitive::GetMaterialCode() const
    {
        return m_material.type;
    }

    Physics::MaterialData ColliderPrimitive::GetMaterial() const
    {
        return m_material;
    }

    eColliderType ColliderPrimitive::Type() const
    {
        return m_type;
    }

    bool ColliderPrimitive::Is2DPrimitive() const
    {
        return m_is_2D;
    }

    Vector3 ColliderPrimitive::PlaneNormal2D() const
    {
        return m_local.orientation.Rotate(Vector3(0.0f, 0.0f, 1.0f));
    }

    void ColliderPrimitive::UpdatePrimitive()
    {
        if (m_collider_set != nullptr)
        {
            SetScale(m_collider_set->GetScale());
            if (m_bounding_volume != nullptr)
            {
                UpdateBoundingVolume();
            }
            SetMassData(m_material.density);
            m_collider_set->UpdateMassData();
            m_collider_set->UpdateBoundingVolume();
        }
    }

    void ColliderPrimitive::UpdateMassData() const
    {
        if (m_collider_set != nullptr)
        {
            m_collider_set->UpdateMassData();
        }
    }

    void ColliderPrimitive::UpdateScaleData()
    {
        if (m_collider_set != nullptr)
        {
            if (m_bounding_volume != nullptr)
            {
                UpdateBoundingVolume();
            }
            SetMassData(m_material.density);
            m_collider_set->UpdateMassData();
            m_collider_set->UpdateBoundingVolume();
        }
    }

    void ColliderPrimitive::LoadMaterial(const Json::Value& data)
    {
        if (JsonResource::HasMember(data, "Material"))
        {
            Vector2            material_data;
            Physics::eMaterial type = Physics::eMaterial::Rock;
            if (data["Material"].isString())
            {
                std::string material = data["Material"].asString();
                if (material == "Rock")
                {
                    type = Physics::eMaterial::Rock;
                }
                else if (material == "Wood")
                {
                    type = Physics::eMaterial::Wood;
                }
                else if (material == "Metal")
                {
                    type = Physics::eMaterial::Metal;
                }
                else if (material == "BouncyBall")
                {
                    type = Physics::eMaterial::BouncyBall;
                }
                else if (material == "SuperBall")
                {
                    type = Physics::eMaterial::SuperBall;
                }
                else if (material == "Pillow")
                {
                    type = Physics::eMaterial::Pillow;
                }
                else if (material == "Static")
                {
                    type = Physics::eMaterial::Static;
                }
                else if (material == "Concrete")
                {
                    type = Physics::eMaterial::Concrete;
                }
                else if (material == "Ice")
                {
                    type = Physics::eMaterial::Ice;
                }
                else if (material == "Glass")
                {
                    type = Physics::eMaterial::Glass;
                }
                else if (material == "Lubricant")
                {
                    type = Physics::eMaterial::Lubricant;
                }
                else if (material == "Rubber")
                {
                    type = Physics::eMaterial::Rubber;
                }
                else if (material == "Velcro")
                {
                    type = Physics::eMaterial::Velcro;
                }
            }
            else if (JsonResource::IsVector2(data["Material"]))
            {
                type          = Physics::eMaterial::UserType;
                material_data = JsonResource::AsVector2(data["Material"]);
            }
            SetMaterial(type, material_data.x, material_data.y);
        }
        if (JsonResource::HasMember(data, "Density") && data["Density"].isDouble())
        {
            m_material.type    = Physics::eMaterial::UserType;
            m_material.density = data["Density"].asFloat();
        }
        if (JsonResource::HasMember(data, "Restitution") && data["Restitution"].isDouble())
        {
            m_material.type        = Physics::eMaterial::UserType;
            m_material.restitution = data["Restitution"].asFloat();
        }

        SetMassData(m_material.density);
    }

    void ColliderPrimitive::LoadTransform(const Json::Value& data)
    {
        if (JsonResource::HasMember(data, "Orientation") && JsonResource::IsQuaternion(data["Orientation"]))
        {
            m_local.orientation = JsonResource::AsQuaternionRIJK(data["Orientation"]);
        }
        if (JsonResource::HasMember(data, "Position") && JsonResource::IsVector3(data["Position"]))
        {
            m_local.position = JsonResource::AsVector3(data["Position"]);
        }
        if (JsonResource::HasMember(data, "Scale") && JsonResource::IsVector3(data["Scale"]))
        {
            m_local.scale = JsonResource::AsVector3(data["Scale"]);;
        }
    }

    void ColliderPrimitive::LoadMass(const Json::Value& data)
    {
        if (JsonResource::HasMember(data, "Centroid") && JsonResource::IsVector3(data["Centroid"]))
        {
            m_centroid = JsonResource::AsVector3(data["Centroid"]);
        }
        if (JsonResource::HasMember(data, "Mass") && data["Mass"].isDouble())
        {
            m_mass = data["Mass"].asFloat();
        }
        if (JsonResource::HasMember(data, "Inertia") && JsonResource::IsMatrix33(data["Inertia"]))
        {
            m_local_inertia_tensor = JsonResource::AsMatrix33(data["Inertia"]);
        }

        m_rigid_body->UpdateInertia();
    }

    void ColliderPrimitive::EditCollider(CommandRegistry* registry)
    {
        AxisRadian axis_rad = m_local.orientation.ToAxisRadian();
        ImGui::Text("Position");
        ImGui::InputFloat3("##LocalTransformEdit0", &m_local.position.x, 3);
        ImGui::Text("Scale");
        ImGui::InputFloat3("##LocalTransformEdit1", &m_local.scale.x, 3);
        ImGui::Text("Orientation");
        ImGui::Text("Axis");
        if (ImGui::InputFloat3("##LocalTransformEdit2", &axis_rad.axis.x, 3))
        {
            m_local.orientation.Set(axis_rad);
        }
        ImGui::Text("Radian");
        if (ImGui::SliderAngle("##LocalTransformEdit3", &axis_rad.radian))
        {
            m_local.orientation.Set(axis_rad);
        }
        ImGui::Text("Quaternion");
        ImGui::SliderFloat4("##LocalTransformEdit4", &m_local.orientation.r, -1.0f, 1.0f);
    }

    void ColliderPrimitive::UpdateRigidBody()
    {
        if (m_bounding_volume != nullptr)
        {
            UpdateBoundingVolume();
        }
    }
}
