#include "ColliderTriangle.hpp"
#include "../../../Graphics/Utility/PrimitiveRenderer.hpp"
#include "../../BroadPhase/BoundingAABB.hpp"
#include "../../../../Manager/Resource/ResourceType/JsonResource.hpp"
#include "../../../../External/JSONCPP/json/json.h"

namespace CS460
{
    ColliderTriangle::ColliderTriangle()
    {
        m_is_2D = true;
    }

    ColliderTriangle::~ColliderTriangle()
    {
    }

    void ColliderTriangle::Initialize()
    {
    }

    void ColliderTriangle::Shutdown()
    {
    }

    Vector3 ColliderTriangle::Support(const Vector3& direction)
    {
        Real    p = Math::REAL_NEGATIVE_MAX;
        Vector3 result;
        for (size_t i = 0; i < 3; ++i)
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

    bool ColliderTriangle::TestRayIntersection(const Ray& local_ray, Real& minimum_t, Real& maximum_t) const
    {
        minimum_t = -1.0f;
        maximum_t = -1.0f;
        //Quadratic elements
        Real triangle_min_t = Math::REAL_POSITIVE_MAX;
        Real triangle_max_t = Math::REAL_NEGATIVE_MAX;
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
                //last edge
                Vector2 p0      = Vertex(0);
                Vector2 p1      = Vertex(1);
                Vector2 p2      = Vertex(2);
                Vector2 edge1   = p1 - p0;
                Vector2 edge2   = p2 - p1;
                Vector2 edge3   = p0 - p2;
                Real    inv_dir = 1.0f / dir.DotProduct(dir);
                if (Math::IsZero(dir.CrossProduct(edge1)) == true)
                {
                    if (Math::IsZero((p0 - pos).CrossProduct(dir)) == true)
                    {
                        triangle_min_t = (p0 - pos).DotProduct(dir) * inv_dir;
                        triangle_max_t = (p1 - pos).DotProduct(dir) * inv_dir;
                    }
                }
                else
                {
                    Real t = (p0 - pos).CrossProduct(edge1) / (dir.CrossProduct(edge1));
                    if (triangle_min_t > t)
                    {
                        triangle_min_t = t;
                    }
                    if (triangle_max_t < t)
                    {
                        triangle_max_t = t;
                    }
                }
                if (Math::IsZero(dir.CrossProduct(edge2)) == true)
                {
                    if (Math::IsZero((p1 - pos).CrossProduct(dir)) == true)
                    {
                        triangle_min_t = (p1 - pos).DotProduct(dir) * inv_dir;
                        triangle_max_t = (p2 - pos).DotProduct(dir) * inv_dir;
                    }
                }
                else
                {
                    Real t = (p1 - pos).CrossProduct(edge2) / (dir.CrossProduct(edge2));
                    if (triangle_min_t > t)
                    {
                        triangle_min_t = t;
                    }
                    if (triangle_max_t < t)
                    {
                        triangle_max_t = t;
                    }
                }
                if (Math::IsZero(dir.CrossProduct(edge3)) == true)
                {
                    if (Math::IsZero((p2 - pos).CrossProduct(dir)) == true)
                    {
                        triangle_min_t = (p2 - pos).DotProduct(dir) * inv_dir;
                        triangle_max_t = (p0 - pos).DotProduct(dir) * inv_dir;
                    }
                }
                else
                {
                    Real t = (p2 - pos).CrossProduct(edge3) / (dir.CrossProduct(edge3));
                    if (triangle_min_t > t)
                    {
                        triangle_min_t = t;
                    }
                    if (triangle_max_t < t)
                    {
                        triangle_max_t = t;
                    }
                }
            }
            else
            {
                return false;
            }
            minimum_t = triangle_min_t;
            maximum_t = triangle_max_t;
        }
        else
        {
            //ray-plane intersect one point.
            Real    plane_t            = pc.DotProduct(normal) / denominator;
            Vector3 plane_intersection = local_ray.position + local_ray.direction * plane_t;
            //define ellipse.
            Vector2 v2edge1 = Vertex(1) - Vertex(0);
            Vector2 v2edge2 = Vertex(2) - Vertex(0);
            Vector3 edge1(v2edge1.x, v2edge1.y);
            Vector3 edge2(v2edge2.x, v2edge2.y);
            Vector3 h = local_ray.direction.CrossProduct(edge2);
            Real    a = edge1.DotProduct(h);
            if (Math::IsZero(a))
            {
                return false;
            }
            Real    f = 1.0f / a;
            Vector3 s = local_ray.position - Vector3(Vertex(0).x, Vertex(1).y);
            Real    u = f * (s.DotProduct(h));
            if (u < 0.0f || u > 1.0f)
            {
                return false;
            }
            Vector3 q = s.CrossProduct(edge1);
            Real    v = f * local_ray.direction.DotProduct(q);
            if (v < 0.0f || u + v > 1.0f)
            {
                return false;
            }
            // At this stage we can compute t to find out where the intersection point is on the line.
            triangle_min_t = f * edge2.DotProduct(q);
            if (triangle_min_t > Math::EPSILON) // ray intersection
            {
                minimum_t = maximum_t = triangle_min_t;
            }
            else // This means that there is a line intersection but not a ray intersection.
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

    Vector3 ColliderTriangle::GetNormal(const Vector3& local_point_on_collider)
    {
        Vector2 v(local_point_on_collider);
        for (size_t i = 0; i < 3; ++i)
        {
            size_t  j    = i + 1 < 3 ? i + 1 : 0;
            Vector2 p0   = Vertex(i);
            Vector2 p1   = Vertex(j);
            Vector2 edge = p1 - p0;
            if (p0.IsEqual(p1) == false)
            {
                Real tx = (v.x - p0.x) / edge.x;
                Real ty = (v.y - p0.y) / edge.y;
                Real t  = Math::IsEqual(tx, ty) ? tx : (!Math::IsZero(tx) ? tx : ty);
                //is point on edge ?
                if (t <= 1.0f && t >= 0.0f)
                {
                    return Vector3(-v.y, v.x);
                }
            }
        }
        return Math::Vector3::Z_AXIS;
    }

    void ColliderTriangle::SetMassData(Real density)
    {
        if (Math::IsNotEqual(m_material.density, density))
        {
            m_material.density = density;
            m_material.type    = Physics::eMaterial::UserType;
        }
        Real area  = 0.0f;
        Real it_xx = 0.0f;
        Real it_yy = 0.0f;
        Real it_zz = 0.0f;
        Real inv3  = 1.0f / 3.0f;
        for (size_t i1 = 0; i1 < 3; ++i1)
        {
            // Triangle vertices, third vertex implied as (0, 0)
            Vector3 p1, p2;
            size_t  i2 = i1 + 1 < 3 ? i1 + 1 : 0;
            if (m_collider_set != nullptr)
            {
                p1 = Vector3(m_scaled_vertices[i1].x, m_scaled_vertices[i1].y);
                p2 = Vector3(m_scaled_vertices[i2].x, m_scaled_vertices[i2].y);
            }
            else
            {
                p1 = Vector3(m_vertices[i1].x, m_vertices[i1].y);
                p2 = Vector3(m_vertices[i2].x, m_vertices[i2].y);
            }
            Real triangle_area = 0.5f * p1.CrossProduct(p2).Length();
            area += triangle_area;
            // Use area to weight the centroid average, not just vertex position
            m_centroid += (p1 + p2) * triangle_area * inv3;
            Real it_x = p1.x * p1.x + p2.x * p1.x + p2.x * p2.x;
            Real it_y = p1.y * p1.y + p2.y * p1.y + p2.y * p2.y;
            it_xx += 0.25f * inv3 * p1.CrossProduct(p2).Length() * it_y;
            it_yy += 0.25f * inv3 * p1.CrossProduct(p2).Length() * it_x;
            it_zz += 0.25f * inv3 * p1.CrossProduct(p2).Length() * (it_x + it_y);
        }
        m_centroid *= 1.0f / area;
        m_mass = density * area;
        m_local_inertia_tensor.SetZero();
        m_local_inertia_tensor.SetDiagonal(it_xx * density, it_yy * density, it_zz * density);
    }

    Real ColliderTriangle::GetVolume()
    {
        Vector3 edge01, edge02;
        if (m_collider_set != nullptr)
        {
            auto v2edge01 = m_scaled_vertices[1] - m_scaled_vertices[0];
            auto v2edge02 = m_scaled_vertices[2] - m_scaled_vertices[0];
            edge01        = Vector3(v2edge01.x, v2edge01.y, 0.0f);
            edge02        = Vector3(v2edge02.x, v2edge02.y, 0.0f);
        }
        else
        {
            auto v2edge01 = m_vertices[1] - m_vertices[0];
            auto v2edge02 = m_vertices[2] - m_vertices[0];
            edge01        = Vector3(v2edge01.x, v2edge01.y, 0.0f);
            edge02        = Vector3(v2edge02.x, v2edge02.y, 0.0f);
        }
        return edge01.CrossProduct(edge02).Length() * 0.5f;
    }

    void ColliderTriangle::SetScaleData(const Vector3& scale)
    {
        for (size_t i = 0; i < 3; ++i)
        {
            m_scaled_vertices[i] = m_vertices[i].HadamardProduct(Vector2(scale.HadamardProduct(m_local.scale)));
        }
    }

    void ColliderTriangle::SetUnit()
    {
        Vector2 max_bound, min_bound;
        max_bound = Math::REAL_NEGATIVE_MAX;
        min_bound = Math::REAL_POSITIVE_MAX;
        for (size_t i = 0; i < 3; ++i)
        {
            for (size_t j = 0; j < 2; ++j)
            {
                Real value = Vertex(i)[j];
                if (max_bound[j] < value)
                {
                    max_bound[j] = value;
                }
                if (min_bound[j] > value)
                {
                    min_bound[j] = value;
                }
            }
        }
        auto scale_factor = (max_bound - min_bound).Inverse();
        for (size_t i = 0; i < 3; ++i)
        {
            m_vertices[i] = m_vertices[i].HadamardProduct(scale_factor);
        }
        UpdatePrimitive();
    }

    void ColliderTriangle::UpdateBoundingVolume()
    {
        Vector3 obb_vertices[6];
        obb_vertices[0].Set(m_scaled_vertices[0].x, m_scaled_vertices[0].y, Physics::Primitive::BOUNDING_VOLUME_MARGIN);
        obb_vertices[1].Set(m_scaled_vertices[0].x, m_scaled_vertices[0].y, -Physics::Primitive::BOUNDING_VOLUME_MARGIN);
        obb_vertices[2].Set(m_scaled_vertices[1].x, m_scaled_vertices[1].y, Physics::Primitive::BOUNDING_VOLUME_MARGIN);
        obb_vertices[3].Set(m_scaled_vertices[1].x, m_scaled_vertices[1].y, -Physics::Primitive::BOUNDING_VOLUME_MARGIN);
        obb_vertices[4].Set(m_scaled_vertices[2].x, m_scaled_vertices[2].y, Physics::Primitive::BOUNDING_VOLUME_MARGIN);
        obb_vertices[5].Set(m_scaled_vertices[2].x, m_scaled_vertices[2].y, -Physics::Primitive::BOUNDING_VOLUME_MARGIN);

        bool has_body = m_rigid_body != nullptr;

        Vector3 min = has_body
                          ? m_rigid_body->LocalToWorldPoint(m_local.LocalToWorldPoint(obb_vertices[0]))
                          : m_local.LocalToWorldPoint(obb_vertices[0]);
        Vector3 max = min;

        if (has_body)
        {
            for (int i = 1; i < 6; ++i)
            {
                Vector3 vertex = m_rigid_body->LocalToWorldPoint(m_local.LocalToWorldPoint(obb_vertices[i]));
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
            for (int i = 1; i < 6; ++i)
            {
                Vector3 vertex = m_local.LocalToWorldPoint(obb_vertices[i]);
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

    void ColliderTriangle::Draw(PrimitiveRenderer* renderer, eRenderingMode mode, const Color& color) const
    {
        I32 index = static_cast<I32>(renderer->VerticesSize(mode));
        I32 count = 3;
        renderer->ReserveVertices(count, mode);
        Transform* body_transform = GetBodyTransform();
        for (size_t i = 0; i < 3; ++i)
        {
            //local space to world space
            Vector3 vertex(Vertex(i));
            vertex = LocalToWorldPoint(vertex);
            vertex = body_transform->LocalToWorldPoint(vertex);
            //push to renderer
            renderer->PushVertex(vertex, mode, color);
        }
        if (mode == eRenderingMode::Dot)
        {
            for (I32 i = 0; i < count; ++i)
            {
                renderer->PushIndex(index + i, mode);
            }
        }
        else if (mode == eRenderingMode::Line)
        {
            for (int i = 0; i < count; ++i)
            {
                I32 j = i + 1 < count ? i + 1 : 0;
                renderer->PushLineIndices(index + i, index + j);
            }
        }
        else if (mode == eRenderingMode::Face)
        {
            renderer->PushFaceIndices(index + 0, index + 1, index + 2);
        }
    }

    Vector2 ColliderTriangle::Vertex(size_t i) const
    {
        if (m_collider_set != nullptr)
        {
            return m_scaled_vertices[i];
        }
        return m_vertices[i];
    }

    void ColliderTriangle::SetTriangle(const Vector2& v0, const Vector2& v1, const Vector2& v2)
    {
        m_vertices[0] = v0;
        m_vertices[1] = v1;
        m_vertices[2] = v2;
        UpdatePrimitive();
    }

    void ColliderTriangle::Clone(ColliderPrimitive* origin)
    {
        if (origin != this && origin != nullptr && origin->Type() == m_type)
        {
            ColliderTriangle* triangle = static_cast<ColliderTriangle*>(origin);
            //collider local space data
            m_local = triangle->m_local;
            //collider mass data
            m_centroid             = triangle->m_centroid;
            m_mass                 = triangle->m_mass;
            m_local_inertia_tensor = triangle->m_local_inertia_tensor;
            m_material             = triangle->m_material;
            //triangle
            std::memcpy(m_vertices, triangle->m_vertices, sizeof m_vertices);
            std::memcpy(m_scaled_vertices, triangle->m_scaled_vertices, sizeof m_scaled_vertices);
        }
    }

    void ColliderTriangle::Load(const Json::Value& data)
    {
        LoadTransform(data);
        //triangle data
        if (JsonResource::HasMember(data, "Vertices") && data["Vertices"].isArray() && data["Vertices"].size() == 3)
        {
            for (int i = 0; i < 3; ++i)
            {
                if (JsonResource::IsVector2(data["Vertices"][i]))
                {
                    m_vertices[i] = JsonResource::AsVector2(data["Vertices"][i]);
                }
            }
        }
        SetTriangle(m_vertices[0], m_vertices[1], m_vertices[2]);
        LoadMaterial(data);
        LoadMass(data);
    }

    void ColliderTriangle::Save(const Json::Value& data)
    {
    }

    void ColliderTriangle::EditPrimitive(CommandRegistry* registry)
    {
    }
}
