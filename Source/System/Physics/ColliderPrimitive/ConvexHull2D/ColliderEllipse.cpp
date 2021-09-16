#include "ColliderEllipse.hpp"
#include "../../../Graphics/Utility/PrimitiveRenderer.hpp"
#include "../../BroadPhase/BoundingAABB.hpp"
#include "../../../../Manager/Resource/ResourceType/JsonResource.hpp"
#include "../../../../External/JSONCPP/json/json.h"

namespace CS460
{
    ColliderEllipse::ColliderEllipse()
    {
        m_is_2D = true;
    }

    ColliderEllipse::~ColliderEllipse()
    {
    }

    void ColliderEllipse::Initialize()
    {
        SetEllipse(Vector2(0.5f, 0.5f));
    }

    void ColliderEllipse::Shutdown()
    {
    }

    Vector3 ColliderEllipse::Support(const Vector3& direction)
    {
        Vector2 radius = Radius();
        Vector2 sub_space_direction(direction);
        sub_space_direction.SetNormalize();
        Vector2 result = radius.HadamardProduct(radius);
        result         = result.HadamardProduct(sub_space_direction);
        Real denom     = radius.HadamardProduct(sub_space_direction).Length();
        result         = Math::IsZero(denom) ? result * 0.0f : result / denom;
        return Vector3(result.x, result.y, 0.0f);
    }

    bool ColliderEllipse::TestRayIntersection(const Ray& local_ray, Real& minimum_t, Real& maximum_t) const
    {
        Vector2 radius        = Radius();
        Real    denominator_x = 1.0f / (radius.x * radius.x);
        Real    denominator_y = 1.0f / (radius.y * radius.y);
        minimum_t             = -1.0f;
        maximum_t             = -1.0f;
        //Quadratic elements
        Real a = local_ray.direction.x * local_ray.direction.x * denominator_x +
                local_ray.direction.y * local_ray.direction.y * denominator_y;
        Real b = 2.0f * local_ray.direction.x * local_ray.position.x * denominator_x +
                2.0f * local_ray.direction.y * local_ray.position.y * denominator_y;
        Real c = local_ray.position.x * local_ray.position.x * denominator_x +
                local_ray.position.y * local_ray.position.y * denominator_y - 1.0f;
        Real ellipse_min_t, ellipse_max_t;
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
                if (Math::SolveQuadratic(a, b, c, ellipse_max_t, ellipse_min_t))
                {
                    //solve intersection
                    if (Math::IsEqual(ellipse_min_t, ellipse_max_t))
                    {
                        minimum_t = maximum_t = ellipse_min_t;
                    }
                    else
                    {
                        minimum_t = ellipse_min_t;
                        maximum_t = ellipse_max_t;
                    }
                }
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
            Real ellipse_result = plane_intersection.x * plane_intersection.x * denominator_x + plane_intersection.y * plane_intersection.y * denominator_y;
            if (ellipse_result <= 1.0f)
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

    Vector3 ColliderEllipse::GetNormal(const Vector3& local_point_on_collider)
    {
        Vector2 radius         = Radius();
        Real    ellipse_result = local_point_on_collider.x * local_point_on_collider.x / (radius.x * radius.x)
                + local_point_on_collider.y * local_point_on_collider.y / (radius.y * radius.y);
        if (Math::IsEqual(ellipse_result, 1.0f))
        {
            Vector3 normal;
            normal.x = 2.0f * local_point_on_collider.x / radius.x * radius.x;
            normal.y = 2.0f * local_point_on_collider.y / radius.y * radius.y;
            normal.SetNormalize();
            return normal;
        }
        return Math::Vector3::Z_AXIS;
    }

    void ColliderEllipse::SetMassData(Real density)
    {
        if (Math::IsNotEqual(m_material.density, density))
        {
            m_material.density = density;
            m_material.type    = Physics::eMaterial::UserType;
        }
        Real a, b;
        m_mass = density * GetVolume();
        if (m_collider_set != nullptr)
        {
            a = m_scaled_radius.x;
            b = m_scaled_radius.y;
        }
        else
        {
            a = m_radius.x;
            b = m_radius.y;
        }
        Real it_xx = 0.25f * m_mass * b * b;
        Real it_yy = 0.25f * m_mass * a * a;
        Real it_zz = 0.25f * m_mass * (a * a + b * b);
        m_local_inertia_tensor.SetZero();
        m_local_inertia_tensor.SetDiagonal(it_xx, it_yy, it_zz);
        m_centroid = Math::Vector3::ORIGIN;
    }

    Real ColliderEllipse::GetVolume()
    {
        if (m_collider_set != nullptr)
        {
            return Math::PI * m_scaled_radius.x * m_scaled_radius.y;
        }
        return Math::PI * m_radius.x * m_radius.y;
    }

    void ColliderEllipse::SetScaleData(const Vector3& scale)
    {
        m_scaled_radius = m_radius.HadamardProduct(Vector2(scale.HadamardProduct(m_local.scale)));
    }

    void ColliderEllipse::SetUnit()
    {
        m_radius.SetNormalize();
        UpdatePrimitive();
    }

    void ColliderEllipse::UpdateBoundingVolume()
    {
        Vector3 obb_vertices[8];
        Real    w = m_scaled_radius.x;
        Real    h = m_scaled_radius.y;
        Real    d = Physics::Primitive::BOUNDING_VOLUME_MARGIN;
        obb_vertices[0].Set(+w, +h, +d);
        obb_vertices[1].Set(+w, +h, -d);
        obb_vertices[2].Set(+w, -h, +d);
        obb_vertices[3].Set(+w, -h, -d);
        obb_vertices[4].Set(-w, +h, +d);
        obb_vertices[5].Set(-w, +h, -d);
        obb_vertices[6].Set(-w, -h, +d);
        obb_vertices[7].Set(-w, -h, -d);

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

    void ColliderEllipse::Draw(PrimitiveRenderer* renderer, eRenderingMode mode, const Color& color) const
    {
        I32 index = static_cast<I32>(renderer->VerticesSize(mode));
        I32 count = renderer->CIRCULAR_VERTICES_COUNT;
        renderer->ReserveVertices(count, mode);
        Transform* body_transform = GetBodyTransform();
        Vector2    radius           = Radius();
        Real       radian_step      = Math::TWO_PI / static_cast<Real>(count);
        for (int i = 0; i < count; ++i)
        {
            Real    angle = static_cast<Real>(i) * radian_step;
            Vector3 vertex(cosf(angle) * radius.x, sinf(angle) * radius.y, 0.0f);
            vertex = LocalToWorldPoint(vertex);
            vertex = body_transform->LocalToWorldPoint(vertex);
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
            renderer->ReserveIndices(200, mode);
            for (int i = 0; i < count - 1; ++i)
            {
                renderer->PushLineIndices(index + i, index + i + 1);
            }
            renderer->PushLineIndices(index + count - 1, index);
        }
        else if (mode == eRenderingMode::Face)
        {
            //add a center pos
            I32     center   = static_cast<I32>(renderer->VerticesSize(mode));
            Vector3 position;
            position = LocalToWorldPoint(position);
            position = body_transform->LocalToWorldPoint(position);

            renderer->PushVertex(position, mode, color);
            for (int i = 0; i < count - 1; ++i)
            {
                renderer->PushFaceIndices(center, index + i + 1, index + i);
            }
            renderer->PushFaceIndices(center, index, index + count - 1);
        }
    }

    Vector2 ColliderEllipse::Radius() const
    {
        if (m_collider_set != nullptr)
        {
            return m_scaled_radius;
        }
        return m_radius;
    }

    void ColliderEllipse::SetEllipse(const Vector2& radius)
    {
        m_radius = radius;
        UpdatePrimitive();
    }

  
    void ColliderEllipse::Clone(ColliderPrimitive* origin)
    {
        if (origin != this && origin != nullptr && origin->Type() == m_type)
        {
            ColliderEllipse* ellipse = static_cast<ColliderEllipse*>(origin);
            //collider local space data
            m_local = ellipse->m_local;
            //collider mass data
            m_centroid             = ellipse->m_centroid;
            m_mass                 = ellipse->m_mass;
            m_local_inertia_tensor = ellipse->m_local_inertia_tensor;
            m_material             = ellipse->m_material;
            //ellipse
            m_radius        = ellipse->m_radius;
            m_scaled_radius = ellipse->m_scaled_radius;
        }
    }

    void ColliderEllipse::Load(const Json::Value& data)
    {
        LoadTransform(data);
        if (JsonResource::HasMember(data, "Radius") && JsonResource::IsVector2(data["Radius"]))
        {
            m_radius = JsonResource::AsVector2(data["Radius"]);
        }
        SetEllipse(m_radius);
        LoadMaterial(data);
        LoadMass(data);
    }

    void ColliderEllipse::Save(const Json::Value& data)
    {
    }

    void ColliderEllipse::EditPrimitive(CommandRegistry* registry)
    {
    }
}
