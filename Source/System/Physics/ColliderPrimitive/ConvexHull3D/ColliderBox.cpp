#include "ColliderBox.hpp"
#include "../../../Graphics/Utility/PrimitiveRenderer.hpp"
#include "../../BroadPhase/BoundingAABB.hpp"
#include "../../../../Manager/Resource/ResourceType/JsonResource.hpp"
#include "../../../../External/JSONCPP/json/json.h"

namespace CS460
{
    ColliderBox::ColliderBox()
    {
        m_type = eColliderType::Box;
    }

    ColliderBox::~ColliderBox()
    {
    }

    void ColliderBox::Initialize()
    {
        SetBox(1.0f, 1.0f, 1.0f);
    }

    void ColliderBox::Shutdown()
    {
    }

    Vector3 ColliderBox::Support(const Vector3& direction)
    {
        Real    p = Math::REAL_NEGATIVE_MAX;
        Vector3 result;
        for (size_t i = 0; i < 8; ++i)
        {
            Real projection = Vertex(i).DotProduct(direction);
            if (projection > p)
            {
                result = Vertex(i);
                p      = projection;
            }
        }
        return result;
    }

    bool ColliderBox::TestRayIntersection(const Ray& local_ray, Real& minimum_t, Real& maximum_t) const
    {
        Real    t_min, t_max, t_y_min, t_y_max, t_z_min, t_z_max;
        Vector3 box_min = Vertex(7);
        Vector3 box_max = Vertex(0);
        Vector3 inv_dir;
        inv_dir.x = 1.0f / local_ray.direction.x;
        inv_dir.y = 1.0f / local_ray.direction.y;
        inv_dir.z = 1.0f / local_ray.direction.z;
        if (inv_dir.x >= 0.0f)
        {
            t_min = (box_min.x - local_ray.position.x) * inv_dir.x;
            t_max = (box_max.x - local_ray.position.x) * inv_dir.x;
        }
        else
        {
            t_min = (box_max.x - local_ray.position.x) * inv_dir.x;
            t_max = (box_min.x - local_ray.position.x) * inv_dir.x;
        }
        if (inv_dir.y >= 0.0f)
        {
            t_y_min = (box_min.y - local_ray.position.y) * inv_dir.y;
            t_y_max = (box_max.y - local_ray.position.y) * inv_dir.y;
        }
        else
        {
            t_y_min = (box_max.y - local_ray.position.y) * inv_dir.y;
            t_y_max = (box_min.y - local_ray.position.y) * inv_dir.y;
        }
        if ((t_min > t_y_max) || (t_y_min > t_max))
        {
            return false;
        }
        if (t_y_min > t_min)
        {
            t_min = t_y_min;
        }
        if (t_y_max < t_max)
        {
            t_max = t_y_max;
        }
        if (inv_dir.z >= 0.0f)
        {
            t_z_min = (box_min.z - local_ray.position.z) * inv_dir.z;
            t_z_max = (box_max.z - local_ray.position.z) * inv_dir.z;
        }
        else
        {
            t_z_min = (box_max.z - local_ray.position.z) * inv_dir.z;
            t_z_max = (box_min.z - local_ray.position.z) * inv_dir.z;
        }
        if ((t_min > t_z_max) || (t_z_min > t_max))
        {
            return false;
        }
        if (t_z_min > t_min)
        {
            t_min = t_z_min;
        }
        if (t_z_max < t_max)
        {
            t_max = t_z_max;
        }
        if (t_min < 0.0f && t_max < 0.0f)
        {
            return false;
        }
        minimum_t = t_min;
        maximum_t = t_max;
        if (minimum_t <= 0.0f)
        {
            minimum_t = 0.0f;
        }
        return true;
    }

    Vector3 ColliderBox::GetNormal(const Vector3& local_point_on_collider)
    {
        Vector3 normal;
        Real    w = (Vertex(0) - Vertex(4)).x;
        Real    h = (Vertex(0) - Vertex(2)).y;
        Real    d = (Vertex(0) - Vertex(1)).z;
        Vector3 size(w, h, d);
        Real    min      = Math::REAL_POSITIVE_MAX;
        Real    distance = fabsf(size.x - fabsf(local_point_on_collider.x));
        if (distance < min)
        {
            min = distance;
            normal.Set(1.0f, 0.0f, 0.0f);
            normal *= Math::Signum(local_point_on_collider.x);
        }
        distance = fabsf(size.y - fabsf(local_point_on_collider.y));
        if (distance < min)
        {
            min = distance;
            normal.Set(0.0f, 1.0f, 0.0f);
            normal *= Math::Signum(local_point_on_collider.y);
        }
        distance = fabsf(size.z - fabsf(local_point_on_collider.z));
        if (distance < min)
        {
            normal.Set(0.0f, 0.0f, 1.0f);
            normal *= Math::Signum(local_point_on_collider.z);
        }
        return normal;
    }

    void ColliderBox::SetMassData(Real density)
    {
        if (Math::IsNotEqual(m_material.density, density))
        {
            m_material.density = density;
            m_material.type    = Physics::eMaterial::UserType;
        }
        Real w, h, d;
        if (m_collider_set != nullptr)
        {
            w = (m_scaled_vertices[0] - m_scaled_vertices[4]).x;
            h = (m_scaled_vertices[0] - m_scaled_vertices[2]).y;
            d = (m_scaled_vertices[0] - m_scaled_vertices[1]).z;
        }
        else
        {
            w = (m_vertices[0] - m_vertices[4]).x;
            h = (m_vertices[0] - m_vertices[2]).y;
            d = (m_vertices[0] - m_vertices[1]).z;
        }
        m_mass    = density * w * h * d;
        Real it_a = m_mass / 12.0f * (h * h + d * d);
        Real it_b = m_mass / 12.0f * (w * w + d * d);
        Real it_c = m_mass / 12.0f * (w * w + h * h);
        m_local_inertia_tensor.SetZero();
        m_local_inertia_tensor.SetDiagonal(it_a, it_b, it_c);
        m_centroid = Vector3(0.5f * w, 0.5f * h, 0.5f * d) + Vertex(7);
    }

    Real ColliderBox::GetVolume()
    {
        if (m_collider_set != nullptr)
        {
            Real w = (m_scaled_vertices[0] - m_scaled_vertices[4]).x;
            Real h = (m_scaled_vertices[0] - m_scaled_vertices[2]).y;
            Real d = (m_scaled_vertices[0] - m_scaled_vertices[1]).z;
            return w * h * d;
        }
        Real w = (m_vertices[0] - m_vertices[4]).x;
        Real h = (m_vertices[0] - m_vertices[2]).y;
        Real d = (m_vertices[0] - m_vertices[1]).z;
        return w * h * d;
    }

    void ColliderBox::SetScaleData(const Vector3& scale)
    {
        for (size_t i = 0; i < 8; ++i)
        {
            m_scaled_vertices[i] = m_vertices[i].HadamardProduct(scale.HadamardProduct(m_local.scale));
        }
    }

    void ColliderBox::SetUnit()
    {
        auto min_max      = m_vertices[0] - m_vertices[7];
        Real scale_factor = min_max.Length();
        min_max /= scale_factor;
        Real w = 0.5f * min_max.x;
        Real h = 0.5f * min_max.y;
        Real d = 0.5f * min_max.z;
        m_vertices[0].Set(+w, +h, +d);
        m_vertices[1].Set(+w, +h, -d);
        m_vertices[2].Set(+w, -h, +d);
        m_vertices[3].Set(+w, -h, -d);
        m_vertices[4].Set(-w, +h, +d);
        m_vertices[5].Set(-w, +h, -d);
        m_vertices[6].Set(-w, -h, +d);
        m_vertices[7].Set(-w, -h, -d);
        UpdatePrimitive();
    }

    void ColliderBox::UpdateBoundingVolume()
    {
        bool has_body = m_rigid_body != nullptr;

        Vector3 min = has_body
                          ? m_rigid_body->LocalToWorldPoint(m_local.LocalToWorldPoint(m_scaled_vertices[0]))
                          : m_local.LocalToWorldPoint(m_scaled_vertices[0]);
        Vector3 max = min;

        if (has_body)
        {
            for (int i = 1; i < 8; ++i)
            {
                Vector3 vertex = m_rigid_body->LocalToWorldPoint(m_local.LocalToWorldPoint(m_scaled_vertices[i]));
                min.x          = Math::Min(min.x, vertex.x);
                min.y          = Math::Min(min.y, vertex.y);
                min.z          = Math::Min(min.z, vertex.z);
                max.x          = Math::Max(max.x, vertex.x);
                max.y          = Math::Max(max.y, vertex.y);
                max.z          = Math::Max(max.z, vertex.z);
            }
        }
        else
        {
            for (int i = 1; i < 8; ++i)
            {
                Vector3 vertex = m_local.LocalToWorldPoint(m_scaled_vertices[i]);
                min.x          = Math::Min(min.x, vertex.x);
                min.y          = Math::Min(min.y, vertex.y);
                min.z          = Math::Min(min.z, vertex.z);
                max.x          = Math::Max(max.x, vertex.x);
                max.y          = Math::Max(max.y, vertex.y);
                max.z          = Math::Max(max.z, vertex.z);
            }
        }

        m_bounding_volume->Set(min, max);
    }

    void ColliderBox::Draw(PrimitiveRenderer* renderer, eRenderingMode mode, const Color& color) const
    {
        I32 index = static_cast<I32>(renderer->VerticesSize(mode));
        renderer->ReserveVertices(8, mode);
        Vector3 vertices[8];
        if (m_collider_set != nullptr)
        {
            std::memcpy(vertices, m_scaled_vertices, sizeof(m_scaled_vertices));
        }
        else
        {
            std::memcpy(vertices, m_vertices, sizeof(m_vertices));
        }
        Transform* body_transform = GetBodyTransform();
        for (auto& vertex : vertices)
        {
            //collider local space to object space(body local)
            vertex = LocalToWorldPoint(vertex);
            vertex = body_transform->LocalToWorldPoint(vertex);
            //push to renderer
            renderer->PushVertex(vertex, mode, color);
        }
        //add indices
        if (mode == eRenderingMode::Dot)
        {
            for (I32 i = 0; i < 8; ++i)
            {
                renderer->PushIndex(index + i, mode);
            }
        }
        else if (mode == eRenderingMode::Line)
        {
            //front
            renderer->PushLineIndices(index + 0, index + 2);
            renderer->PushLineIndices(index + 2, index + 6);
            renderer->PushLineIndices(index + 6, index + 4);
            renderer->PushLineIndices(index + 4, index + 0);
            //back
            renderer->PushLineIndices(index + 1, index + 3);
            renderer->PushLineIndices(index + 3, index + 7);
            renderer->PushLineIndices(index + 7, index + 5);
            renderer->PushLineIndices(index + 5, index + 1);
            //side
            renderer->PushLineIndices(index + 0, index + 1);
            renderer->PushLineIndices(index + 2, index + 3);
            renderer->PushLineIndices(index + 6, index + 7);
            renderer->PushLineIndices(index + 4, index + 5);
        }
        else if (mode == eRenderingMode::Face)
        {
            //front
            renderer->PushFaceIndices(index + 0, index + 2, index + 4);
            renderer->PushFaceIndices(index + 2, index + 6, index + 4);
            //back
            renderer->PushFaceIndices(index + 1, index + 3, index + 5);
            renderer->PushFaceIndices(index + 3, index + 7, index + 5);
            //right
            renderer->PushFaceIndices(index + 0, index + 1, index + 3);
            renderer->PushFaceIndices(index + 2, index + 3, index + 0);
            //left
            renderer->PushFaceIndices(index + 6, index + 7, index + 5);
            renderer->PushFaceIndices(index + 4, index + 5, index + 6);
            //top
            renderer->PushFaceIndices(index + 1, index + 0, index + 4);
            renderer->PushFaceIndices(index + 1, index + 4, index + 5);
            //bottom
            renderer->PushFaceIndices(index + 3, index + 2, index + 6);
            renderer->PushFaceIndices(index + 3, index + 6, index + 7);
        }
    }

    Vector3 ColliderBox::Vertex(size_t i) const
    {
        return m_collider_set != nullptr ? m_scaled_vertices[i] : m_vertices[i];
    }

    void ColliderBox::SetBox(Real width, Real height, Real depth)
    {
        Real w = 0.5f * width;
        Real h = 0.5f * height;
        Real d = 0.5f * depth;
        m_vertices[0].Set(+w, +h, +d);
        m_vertices[1].Set(+w, +h, -d);
        m_vertices[2].Set(+w, -h, +d);
        m_vertices[3].Set(+w, -h, -d);
        m_vertices[4].Set(-w, +h, +d);
        m_vertices[5].Set(-w, +h, -d);
        m_vertices[6].Set(-w, -h, +d);
        m_vertices[7].Set(-w, -h, -d);
        UpdatePrimitive();
    }

    void ColliderBox::Clone(ColliderPrimitive* origin)
    {
        if (origin != this && origin != nullptr && origin->Type() == m_type)
        {
            ColliderBox* box = static_cast<ColliderBox*>(origin);
            //collider local space data
            m_local = box->m_local;
            //collider mass data
            m_centroid             = box->m_centroid;
            m_mass                 = box->m_mass;
            m_local_inertia_tensor = box->m_local_inertia_tensor;
            m_material             = box->m_material;
            //box data
            std::memcpy(m_vertices, box->m_vertices, sizeof m_vertices);
            std::memcpy(m_scaled_vertices, box->m_scaled_vertices, sizeof m_scaled_vertices);
        }
    }

    void ColliderBox::Load(const Json::Value& data)
    {
        LoadTransform(data);
        //box data
        if (JsonResource::HasMember(data, "Vertices") && data["Vertices"].isArray() && data["Vertices"].size() == 8)
        {
            for (int i = 0; i < 8; ++i)
            {
                if (JsonResource::IsVector3(data["Vertices"][i]))
                {
                    m_vertices[i] = JsonResource::AsVector3(data["Vertices"][i]);
                }
            }
        }
        if (JsonResource::HasMember(data, "Cube") && JsonResource::IsVector3(data["Cube"]))
        {
            auto cube = JsonResource::AsVector3(data["Cube"]);
            SetBox(cube.x, cube.y, cube.z);
        }
        LoadMaterial(data);
        LoadMass(data);
    }

    void ColliderBox::Save(const Json::Value& data)
    {
    }

    void ColliderBox::EditPrimitive(CommandRegistry* registry)
    {
    }
}
