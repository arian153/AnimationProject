#include "ColliderRectangle.hpp"
#include "../../../Graphics/Utility/PrimitiveRenderer.hpp"
#include "../../BroadPhase/BoundingAABB.hpp"
#include "../../../../Manager/Resource/ResourceType/JsonResource.hpp"
#include "../../../../External/JSONCPP/json/json.h"

namespace CS460
{
    ColliderRectangle::ColliderRectangle()
    {
        m_is_2D = true;
    }

    ColliderRectangle::~ColliderRectangle()
    {
    }

    void ColliderRectangle::Initialize()
    {
    }

    void ColliderRectangle::Shutdown()
    {
    }

    Vector3 ColliderRectangle::Support(const Vector3& direction)
    {
        Real    p = Math::REAL_NEGATIVE_MAX;
        Vector3 result;
        for (size_t i = 0; i < 4; ++i)
        {
            Real projection = Vector3(Vertex(i)).DotProduct(direction);
            if (projection > p)
            {
                result = Vertex(i);
                p      = projection;
            }
        }
        return result;
    }

    bool ColliderRectangle::TestRayIntersection(const Ray& local_ray, Real& minimum_t, Real& maximum_t) const
    {
        minimum_t = -1.0f;
        maximum_t = -1.0f;
        //plane elements
        Vector3 normal(0.0f, 0.0f, 1.0f);
        Vector3 pc          = -local_ray.position;
        Real    denominator = normal.DotProduct(local_ray.direction);
        if (Math::IsZero(denominator) == true)
        {
            //ray is parallel to plane.
            if (Math::IsZero(pc.DotProduct(normal)) == true)
            {
                //ray is on the plane.
                Vector2 dir(local_ray.direction.x, local_ray.direction.y);
                Vector2 pos(local_ray.position.x, local_ray.position.y);
                Real    t_min, t_max, t_y_min, t_y_max;
                Vector2 box_min = Vertex(3);
                Vector2 box_max = Vertex(0);
                Vector2 inv_dir;
                inv_dir.x = 1.0f / local_ray.direction.x;
                inv_dir.y = 1.0f / local_ray.direction.y;
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
                if (t_min < 0.0f && t_max < 0.0f)
                {
                    return false;
                }
                minimum_t = t_min;
                maximum_t = t_max;
            }
            else
            {
                return false;
            }
        }
        else
        {
            //ray-plane intersect one point.
            Real    plane_t            = pc.DotProduct(normal) / denominator;
            Vector3 plane_intersection = local_ray.position + local_ray.direction * plane_t;
            //define ellipse.
            bool is_rectangle_contain_point = false;
            if (is_rectangle_contain_point)
            {
                minimum_t = maximum_t = plane_t;
            }
            else
            {
                return false;
            }
        }
        if (minimum_t < 0.0f && maximum_t < 0.0f)
        {
            return false;
        }
        if (minimum_t <= 0.0f)
        {
            minimum_t = 0.0f;
        }
        return true;
    }

    Vector3 ColliderRectangle::GetNormal(const Vector3& local_point_on_collider)
    {
        Vector3 normal;
        Real    w = (Vertex(0) - Vertex(3)).x;
        Real    h = (Vertex(0) - Vertex(3)).y;
        Vector3 size(w, h);
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

    void ColliderRectangle::SetMassData(Real density)
    {
        if (Math::IsNotEqual(m_material.density, density))
        {
            m_material.density = density;
            m_material.type    = Physics::eMaterial::UserType;
        }
        Real w, h;
        if (m_collider_set != nullptr)
        {
            w = (m_scaled_vertices[0] - m_scaled_vertices[2]).x;
            h = (m_scaled_vertices[0] - m_scaled_vertices[3]).y;
        }
        else
        {
            w = (m_vertices[0] - m_vertices[2]).x;
            h = (m_vertices[0] - m_vertices[3]).y;
        }
        m_mass    = density * w * h;
        Real it_a = m_mass / 12.0f * (h * h);
        Real it_b = m_mass / 12.0f * (w * w);
        Real it_c = m_mass / 12.0f * (w * w + h * h);
        m_local_inertia_tensor.SetZero();
        m_local_inertia_tensor.SetDiagonal(it_a, it_b, it_c);
        auto vertex = Vertex(7);
        m_centroid  = Vector3(0.5f * w, 0.5f * h, 0.0f) + Vector3(vertex.x, vertex.y, 0.0f);
    }

    Real ColliderRectangle::GetVolume()
    {
        if (m_collider_set != nullptr)
        {
            Real w = (m_scaled_vertices[0] - m_scaled_vertices[3]).x;
            Real h = (m_scaled_vertices[0] - m_scaled_vertices[3]).y;
            return w * h;
        }
        Real w = (m_vertices[0] - m_vertices[3]).x;
        Real h = (m_vertices[0] - m_vertices[3]).y;
        return w * h;
    }

    void ColliderRectangle::SetScaleData(const Vector3& scale)
    {
        for (size_t i = 0; i < 4; ++i)
        {
            m_scaled_vertices[i] = m_vertices[i].HadamardProduct(Vector2(scale.HadamardProduct(m_local.scale)));
        }
    }

    void ColliderRectangle::SetUnit()
    {
        auto min_max      = m_vertices[0] - m_vertices[3];
        Real scale_factor = min_max.Length();
        min_max /= scale_factor;
        Real w = 0.5f * min_max.x;
        Real h = 0.5f * min_max.y;
        m_vertices[0].Set(+w, +h);
        m_vertices[1].Set(+w, -h);
        m_vertices[2].Set(-w, +h);
        m_vertices[3].Set(-w, -h);
        UpdatePrimitive();
    }

    void ColliderRectangle::UpdateBoundingVolume()
    {
        Vector3 obb_vertices[8];
        obb_vertices[0].Set(m_scaled_vertices[0].x, m_scaled_vertices[0].y, Physics::Primitive::BOUNDING_VOLUME_MARGIN);
        obb_vertices[1].Set(m_scaled_vertices[0].x, m_scaled_vertices[0].y, -Physics::Primitive::BOUNDING_VOLUME_MARGIN);
        obb_vertices[2].Set(m_scaled_vertices[1].x, m_scaled_vertices[1].y, Physics::Primitive::BOUNDING_VOLUME_MARGIN);
        obb_vertices[3].Set(m_scaled_vertices[1].x, m_scaled_vertices[1].y, -Physics::Primitive::BOUNDING_VOLUME_MARGIN);
        obb_vertices[4].Set(m_scaled_vertices[2].x, m_scaled_vertices[2].y, Physics::Primitive::BOUNDING_VOLUME_MARGIN);
        obb_vertices[5].Set(m_scaled_vertices[2].x, m_scaled_vertices[2].y, -Physics::Primitive::BOUNDING_VOLUME_MARGIN);
        obb_vertices[6].Set(m_scaled_vertices[3].x, m_scaled_vertices[3].y, Physics::Primitive::BOUNDING_VOLUME_MARGIN);
        obb_vertices[7].Set(m_scaled_vertices[3].x, m_scaled_vertices[3].y, -Physics::Primitive::BOUNDING_VOLUME_MARGIN);

        bool has_body = m_rigid_body != nullptr;

        Vector3 min = has_body
            ? m_rigid_body->LocalToWorldPoint(m_local.LocalToWorldPoint(obb_vertices[0]))
            : m_local.LocalToWorldPoint(obb_vertices[0]);
        Vector3 max = min;

        if (has_body)
        {
            for (int i = 1; i < 8; ++i)
            {
                Vector3 vertex = m_rigid_body->LocalToWorldPoint(m_local.LocalToWorldPoint(obb_vertices[i]));
                min.x = Math::Min(min.x, vertex.x);
                min.y = Math::Min(min.y, vertex.y);
                min.z = Math::Min(min.z, vertex.z);
                max.x = Math::Max(max.x, vertex.x);
                max.y = Math::Max(max.y, vertex.y);
                max.z = Math::Max(max.z, vertex.z);
            }
        }
        else
        {
            for (int i = 1; i < 8; ++i)
            {
                Vector3 vertex = m_local.LocalToWorldPoint(obb_vertices[i]);
                min.x = Math::Min(min.x, vertex.x);
                min.y = Math::Min(min.y, vertex.y);
                min.z = Math::Min(min.z, vertex.z);
                max.x = Math::Max(max.x, vertex.x);
                max.y = Math::Max(max.y, vertex.y);
                max.z = Math::Max(max.z, vertex.z);
            }
        }

        m_bounding_volume->Set(min, max);
    }

    void ColliderRectangle::Draw(PrimitiveRenderer* renderer, eRenderingMode mode, const Color& color) const
    {
        I32 index = static_cast<I32>(renderer->VerticesSize(mode));
        renderer->ReserveVertices(8, mode);
        Vector2 vertices[ 4 ];
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
            Vector3 vertex3(vertex);
            vertex3 = LocalToWorldPoint(vertex3);
            vertex3 = body_transform->LocalToWorldPoint(vertex3);
            //push to renderer
            renderer->PushVertex(vertex3, mode, color);
        }
        //add indices
        if (mode == eRenderingMode::Dot)
        {
            for (I32 i = 0; i < 4; ++i)
            {
                renderer->PushIndex(index + i, mode);
            }
        }
        else if (mode == eRenderingMode::Line)
        {
            //front
            renderer->PushLineIndices(index + 0, index + 1);
            renderer->PushLineIndices(index + 1, index + 2);
            renderer->PushLineIndices(index + 2, index + 3);
            renderer->PushLineIndices(index + 3, index + 0);
        }
        else if (mode == eRenderingMode::Face)
        {
            //front
            renderer->PushFaceIndices(index + 0, index + 1, index + 2);
            renderer->PushFaceIndices(index + 1, index + 3, index + 2);
        }
    }

    Vector2 ColliderRectangle::Vertex(size_t i) const
    {
        return m_collider_set != nullptr ? m_scaled_vertices[i] : m_vertices[i];
    }

    void ColliderRectangle::SetRectangle(Real width, Real height)
    {
        Real w = 0.5f * width;
        Real h = 0.5f * height;
        m_vertices[0].Set(+w, +h);
        m_vertices[1].Set(+w, -h);
        m_vertices[2].Set(-w, +h);
        m_vertices[3].Set(-w, -h);
        UpdatePrimitive();
    }

    void ColliderRectangle::Clone(ColliderPrimitive* origin)
    {
        if (origin != this && origin != nullptr && origin->Type() == m_type)
        {
            ColliderRectangle* rectangle = static_cast<ColliderRectangle*>(origin);
            //collider local space data
            m_local = rectangle->m_local;
            //collider mass data
            m_centroid             = rectangle->m_centroid;
            m_mass                 = rectangle->m_mass;
            m_local_inertia_tensor = rectangle->m_local_inertia_tensor;
            m_material             = rectangle->m_material;
            //rectangle
            std::memcpy(m_vertices, rectangle->m_vertices, sizeof m_vertices);
            std::memcpy(m_scaled_vertices, rectangle->m_scaled_vertices, sizeof m_scaled_vertices);
        }
    }

    void ColliderRectangle::Load(const Json::Value& data)
    {
        LoadTransform(data);
        //rectangle data
        if (JsonResource::HasMember(data, "Vertices") && data["Vertices"].isArray() && data["Vertices"].size() == 4)
        {
            for (int i = 0; i < 4; ++i)
            {
                if (JsonResource::IsVector2(data["Vertices"][i]))
                {
                    m_vertices[i] = JsonResource::AsVector2(data["Vertices"][i]);
                }
            }
        }
        if (JsonResource::HasMember(data, "Box") && JsonResource::IsVector3(data["Box"]))
        {
            auto cube = JsonResource::AsVector2(data["Box"]);
            SetRectangle(cube.x, cube.y);
        }
        //need to set rectangle
        LoadMaterial(data);
        LoadMass(data);
    }

    void ColliderRectangle::Save(const Json::Value& data)
    {
    }

    void ColliderRectangle::EditPrimitive(CommandRegistry* registry)
    {
    }
}
