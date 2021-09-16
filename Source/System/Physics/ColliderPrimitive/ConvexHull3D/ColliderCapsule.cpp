#include "ColliderCapsule.hpp"
#include "../../../Graphics/Utility/PrimitiveRenderer.hpp"
#include "../../BroadPhase/BoundingAABB.hpp"
#include "../../../../Manager/Resource/ResourceType/JsonResource.hpp"
#include "../../../../External/JSONCPP/json/json.h"

namespace CS460
{
    ColliderCapsule::ColliderCapsule()
    {
        m_type = eColliderType::Capsule;
    }

    ColliderCapsule::~ColliderCapsule()
    {
    }

    void ColliderCapsule::Initialize()
    {
        SetCapsule(0.5f, Vector3(0.25f, 0.25f, 0.25f));
    }

    void ColliderCapsule::Shutdown()
    {
    }

    Vector3 ColliderCapsule::Support(const Vector3& direction)
    {
        Vector3 axis_vector;
        axis_vector.y  = HalfHeight();
        Vector3 radius = Radius();
        Vector3 result = radius.HadamardProduct(radius);
        result         = result.HadamardProduct(direction);
        Real denom     = radius.HadamardProduct(direction).Length();
        result         = Math::IsZero(denom) ? result * 0.0f : result / denom;

        Real top = direction.DotProduct(result + axis_vector);
        Real bot = direction.DotProduct(result - axis_vector);
        if (top > bot)
        {
            result += axis_vector;
        }
        else
        {
            result -= axis_vector;
        }
        return result;
    }

    bool ColliderCapsule::TestRayIntersection(const Ray& local_ray, Real& minimum_t, Real& maximum_t) const
    {
        Real    half_height = HalfHeight();
        Vector3 capsule_a(0.0f, -half_height, 0.0f);
        Vector3 capsule_b(0.0f, half_height, 0.0f);
        Vector3 axis(0.0f, Height(), 0.0f);
        minimum_t             = -1.0f;
        maximum_t             = -1.0f;
        Real    denominator_x = 1.0f / (m_radius.x * m_radius.x);
        Real    denominator_z = 1.0f / (m_radius.z * m_radius.z);
        Real    a             = local_ray.direction.x * local_ray.direction.x * denominator_x + local_ray.direction.z * local_ray.direction.z * denominator_z;
        Real    b             = 2.0f * (local_ray.direction.x * local_ray.position.x * denominator_x + local_ray.direction.z * local_ray.position.z * denominator_z);
        Real    c             = local_ray.position.x * local_ray.position.x * denominator_x + local_ray.position.z * local_ray.position.z * denominator_z - 1.0f;
        Vector3 axis_test     = local_ray.direction - local_ray.direction.ProjectionTo(axis);
        Real    cylinder_min_t, cylinder_max_t;
        Real    sphere_a_min_t, sphere_a_max_t, sphere_b_min_t, sphere_b_max_t;
        bool    b_sphere_a = TestRayEllipsoid(local_ray, capsule_a, sphere_a_min_t, sphere_a_max_t);
        bool    b_sphere_b = TestRayEllipsoid(local_ray, capsule_b, sphere_b_min_t, sphere_b_max_t);
        if (Math::IsZero(axis_test.LengthSquared()))
        {
            if (!b_sphere_a || !b_sphere_b)
            {
                return false;
            }
            minimum_t = sphere_a_min_t < sphere_b_min_t ? sphere_a_min_t : sphere_b_min_t;
            maximum_t = sphere_a_max_t > sphere_b_max_t ? sphere_a_max_t : sphere_b_max_t;
        }
        else
        {
            if (Math::SolveQuadratic(a, b, c, cylinder_max_t, cylinder_min_t) == true)
            {
                minimum_t                  = cylinder_min_t;
                maximum_t                  = cylinder_max_t;
                Real ellipsoid_height      = half_height + m_radius.y;
                Real cylinder_min_t_height = local_ray.position.y + local_ray.direction.y * cylinder_min_t;
                Real cylinder_max_t_height = local_ray.position.y + local_ray.direction.y * cylinder_max_t;
                Real sphere_a_min_t_height = Math::REAL_MAX;
                Real sphere_a_max_t_height = Math::REAL_MAX;
                Real sphere_b_min_t_height = Math::REAL_MAX;
                Real sphere_b_max_t_height = Math::REAL_MAX;
                if (b_sphere_a == true)
                {
                    sphere_a_min_t_height = local_ray.position.y + local_ray.direction.y * sphere_a_min_t;
                    sphere_a_max_t_height = local_ray.position.y + local_ray.direction.y * sphere_a_max_t;
                }
                if (b_sphere_b == true)
                {
                    sphere_b_min_t_height = local_ray.position.y + local_ray.direction.y * sphere_b_min_t;
                    sphere_b_max_t_height = local_ray.position.y + local_ray.direction.y * sphere_b_max_t;
                }
                if (cylinder_min_t_height > ellipsoid_height && cylinder_max_t_height > ellipsoid_height)
                {
                    return false;
                }
                if (cylinder_min_t_height < -ellipsoid_height && cylinder_max_t_height < -ellipsoid_height)
                {
                    return false;
                }
                if (cylinder_min_t_height > half_height)
                {
                    if (b_sphere_b == true)
                    {
                        minimum_t = sphere_b_min_t_height > sphere_b_max_t_height ? sphere_b_min_t : sphere_b_max_t;
                    }
                    else
                    {
                        return false;
                    }
                }
                if (cylinder_min_t_height < -half_height)
                {
                    if (b_sphere_a == true)
                    {
                        minimum_t = sphere_a_min_t_height < sphere_a_max_t_height ? sphere_a_min_t : sphere_a_max_t;
                    }
                    else
                    {
                        return false;
                    }
                }
                if (cylinder_max_t_height > half_height)
                {
                    if (b_sphere_b == true)
                    {
                        maximum_t = sphere_b_min_t_height > sphere_b_max_t_height ? sphere_b_min_t : sphere_b_max_t;
                    }
                    else
                    {
                        return false;
                    }
                }
                if (cylinder_max_t_height < -half_height)
                {
                    if (b_sphere_a == true)
                    {
                        maximum_t = sphere_a_min_t_height < sphere_a_max_t_height ? sphere_a_min_t : sphere_a_max_t;
                    }
                    else
                    {
                        return false;
                    }
                }
                if (b_sphere_a == true && b_sphere_b == true)
                {
                    minimum_t = Math::Min(Math::Min(Math::Min(sphere_a_min_t, sphere_a_max_t), sphere_b_min_t), sphere_b_max_t);
                    maximum_t = Math::Max(Math::Max(Math::Max(sphere_a_min_t, sphere_a_max_t), sphere_b_min_t), sphere_b_max_t);
                }
            }
            else
            {
                return false;
            }
        }
        if (minimum_t > maximum_t)
        {
            Real temp = minimum_t;
            minimum_t = maximum_t;
            maximum_t = temp;
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

    Vector3 ColliderCapsule::GetNormal(const Vector3& local_point_on_collider)
    {
        Real    half_height = HalfHeight();
        Vector3 radius      = Radius();
        Vector3 normal;
        if (local_point_on_collider.y > half_height || local_point_on_collider.y < -half_height)
        {
            //sphere case
            Vector3 sphere_origin;
            sphere_origin.y   = half_height * Math::Signum(local_point_on_collider.y);
            Vector3 ellipsoid = local_point_on_collider - sphere_origin;
            normal.x          = 2.0f * ellipsoid.x / radius.x * radius.x;
            normal.y          = 2.0f * ellipsoid.y / radius.y * radius.y;
            normal.z          = 2.0f * ellipsoid.z / radius.z * radius.z;
            normal.SetNormalize();
        }
        else
        {
            //disc case
            normal.x = 2.0f * local_point_on_collider.x / radius.x * radius.x;
            normal.y = 0.0f;
            normal.z = 2.0f * local_point_on_collider.z / radius.z * radius.z;
            normal.SetNormalize();
        }
        return normal;
    }

    void ColliderCapsule::SetMassData(Real density)
    {
        if (Math::IsNotEqual(m_material.density, density))
        {
            m_material.density = density;
            m_material.type    = Physics::eMaterial::UserType;
        }
        Real a, b, c, h;
        m_mass = density * GetVolume();
        if (m_collider_set != nullptr)
        {
            a = m_scaled_radius.x;
            b = m_scaled_radius.z;
            c = m_scaled_radius.y;
            h = m_scaled_height;
        }
        else
        {
            a = m_radius.x;
            b = m_radius.z;
            c = m_radius.y;
            h = m_height;
        }
        Real multi_a = 2.0f * c * m_mass / (4.0f * c + 3.0f * h);
        Real multi_b = 3.0f * h * m_mass / (4.0f * c + 3.0f * h);
        Real it_xx   = multi_a * (0.4f * (b * b + c * c) + 0.75f * h * c + 0.5f * h * h) + multi_b * (0.25f * b * b + h * h / 12.0f);
        Real it_zz   = multi_a * (0.4f * (a * a + c * c) + 0.75f * h * c + 0.5f * h * h) + multi_b * (0.25f * a * a + h * h / 12.0f);
        Real it_yy   = multi_a * 0.4f * (a * a + b * b) + multi_b * 0.25f * (a * a + b * b);
        m_local_inertia_tensor.SetZero();
        m_local_inertia_tensor.SetDiagonal(it_xx, it_yy, it_zz);
        m_centroid         = Vector3(0.0f, 0.0f, 0.0f);
        m_material.density = density;
        m_material.type    = Physics::eMaterial::UserType;
    }

    Real ColliderCapsule::GetVolume()
    {
        if (m_collider_set != nullptr)
        {
            return Math::PI * m_scaled_radius.x * m_scaled_radius.z * (m_scaled_radius.y * 4.0f / 3.0f + m_scaled_height);
        }
        return Math::PI * m_radius.x * m_radius.z * (m_radius.y * 4.0f / 3.0f + m_height);
    }

    void ColliderCapsule::SetScaleData(const Vector3& scale)
    {
        m_scaled_height = m_height * scale.y;
        m_scaled_radius = m_radius.HadamardProduct(scale.HadamardProduct(m_local.scale));
    }

    void ColliderCapsule::SetUnit()
    {
        Real length = m_radius.y * 2.0f + m_height;
        if (length > 0.0f)
        {
            m_radius /= length;
            m_height /= length;
        }
        UpdatePrimitive();
    }

    void ColliderCapsule::UpdateBoundingVolume()
    {
        Vector3 obb_vertices[8];
        Real    w = m_scaled_radius.x;
        Real    h = m_scaled_radius.y + m_scaled_height * 0.5f;
        Real    d = m_scaled_radius.z;
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

    void ColliderCapsule::Draw(PrimitiveRenderer* renderer, eRenderingMode mode, const Color& color) const
    {
        I32 index       = static_cast<I32>(renderer->VerticesSize(mode));
        int stack_count = renderer->SPHERICAL_STACK_COUNT;
        int slice_count = renderer->SPHERICAL_SLICE_COUNT;
        renderer->ReserveVertices(renderer->SPHERICAL_VERTICES_COUNT, mode);
        Vector3    axis_vector    = Math::Vector3::Y_AXIS;
        Transform* body_transform = GetBodyTransform();
        Real       half_height    = HalfHeight();
        Vector3    radius         = Radius();
        //top vertex
        Vector3 top_vertex_local_pos = axis_vector;
        top_vertex_local_pos         = top_vertex_local_pos.HadamardProduct(radius);
        top_vertex_local_pos += half_height * axis_vector;
        //modify rotation, translation
        top_vertex_local_pos = LocalToWorldPoint(top_vertex_local_pos);
        top_vertex_local_pos = body_transform->LocalToWorldPoint(top_vertex_local_pos);
        //bottom vertex
        Vector3 bottom_vertex_local_pos = -axis_vector;
        bottom_vertex_local_pos         = bottom_vertex_local_pos.HadamardProduct(radius);
        bottom_vertex_local_pos -= half_height * axis_vector;
        //modify rotation, translation
        bottom_vertex_local_pos = LocalToWorldPoint(bottom_vertex_local_pos);
        bottom_vertex_local_pos = body_transform->LocalToWorldPoint(bottom_vertex_local_pos);
        renderer->PushVertex(top_vertex_local_pos, mode, color);
        Real phi_step   = Math::PI / stack_count;
        Real theta_step = Math::TWO_PI / slice_count;
        // Compute vertices for each stack ring (do not count the poles as rings).
        for (I32 i = 1; i <= stack_count - 1; ++i)
        {
            Real phi = i * phi_step;
            // Vertices of ring.
            for (I32 j = 0; j <= slice_count; ++j)
            {
                Real theta = j * theta_step;
                // spherical to cartesian
                Vector3 vertex_local_pos;
                vertex_local_pos.x = sinf(phi) * cosf(theta);
                vertex_local_pos.y = cosf(phi);
                vertex_local_pos.z = sinf(phi) * sinf(theta);
                vertex_local_pos   = vertex_local_pos.HadamardProduct(radius);
                if (i < 8)
                {
                    vertex_local_pos += half_height * axis_vector;
                }
                else
                {
                    vertex_local_pos -= half_height * axis_vector;
                }
                vertex_local_pos = LocalToWorldPoint(vertex_local_pos);
                vertex_local_pos = body_transform->LocalToWorldPoint(vertex_local_pos);
                renderer->PushVertex(vertex_local_pos, mode, color);
            }
        }
        renderer->PushVertex(bottom_vertex_local_pos, mode, color);
        if (mode == eRenderingMode::Dot)
        {
            I32 count = renderer->SPHERICAL_VERTICES_COUNT;
            for (I32 i = 0; i < count; ++i)
            {
                renderer->PushIndex(index + i, mode);
            }
        }
        else if (mode == eRenderingMode::Line)
        {
            for (I32 i = 1; i <= slice_count; ++i)
            {
                renderer->PushLineIndices(index, index + i + 1);
            }
            I32 base = 1 + index;
            I32 ring = slice_count + 1;
            for (I32 i = 0; i < stack_count - 2; ++i)
            {
                for (I32 j = 0; j < slice_count; ++j)
                {
                    renderer->PushLineIndices(base + i * ring + j, base + i * ring + j + 1);
                    renderer->PushLineIndices(base + i * ring + j, base + (i + 1) * ring + j);
                }
            }
            I32 south_pole_index = static_cast<I32>(renderer->VerticesSize(mode)) - 1;
            base                 = south_pole_index - ring;
            for (I32 i = 0; i < slice_count; ++i)
            {
                renderer->PushLineIndices(south_pole_index, base + i);
            }
        }
        else if (mode == eRenderingMode::Face)
        {
            for (I32 i = 1; i <= slice_count; ++i)
            {
                renderer->PushFaceIndices(index, index + i + 1, index + i);
            }
            I32 base = 1 + index;
            I32 ring = slice_count + 1;
            for (I32 i = 0; i < stack_count - 2; ++i)
            {
                for (I32 j = 0; j < slice_count; ++j)
                {
                    renderer->PushFaceIndices(
                                              base + i * ring + j,
                                              base + i * ring + j + 1,
                                              base + (i + 1) * ring + j);
                    renderer->PushFaceIndices(
                                              base + (i + 1) * ring + j,
                                              base + i * ring + j + 1,
                                              base + (i + 1) * ring + j + 1);
                }
            }
            I32 south_pole_index = static_cast<I32>(renderer->VerticesSize(mode)) - 1;
            base                 = south_pole_index - ring;
            for (I32 i = 0; i < slice_count; ++i)
            {
                renderer->PushFaceIndices(south_pole_index, base + i, base + i + 1);
            }
        }
    }

    Real ColliderCapsule::HalfHeight() const
    {
        if (m_collider_set != nullptr)
        {
            return m_scaled_height * 0.5f;
        }
        return m_height * 0.5f;
    }

    Real ColliderCapsule::Height() const
    {
        if (m_collider_set != nullptr)
        {
            return m_scaled_height;
        }
        return m_height;
    }

    Vector3 ColliderCapsule::Radius() const
    {
        if (m_collider_set != nullptr)
        {
            return m_scaled_radius;
        }
        return m_radius;
    }

    void ColliderCapsule::SetCapsule(Real height, const Vector3& radius)
    {
        m_radius = radius;
        m_height = height;
        UpdatePrimitive();
    }

    void ColliderCapsule::SetHeight(Real height)
    {
        m_height = height;
        UpdatePrimitive();
    }

    void ColliderCapsule::SetRadius(const Vector3& radius)
    {
        m_radius = radius;
        UpdatePrimitive();
    }

    void ColliderCapsule::Clone(ColliderPrimitive* origin)
    {
        if (origin != this && origin != nullptr && origin->Type() == m_type)
        {
            ColliderCapsule* capsule = static_cast<ColliderCapsule*>(origin);
            //collider local space data
            m_local = capsule->m_local;
            //collider mass data
            m_centroid             = capsule->m_centroid;
            m_mass                 = capsule->m_mass;
            m_local_inertia_tensor = capsule->m_local_inertia_tensor;
            m_material             = capsule->m_material;
            //capsule
            m_radius        = capsule->m_radius;
            m_height        = capsule->m_height;
            m_scaled_radius = capsule->m_scaled_radius;
            m_scaled_height = capsule->m_scaled_height;
        }
    }

    void ColliderCapsule::Load(const Json::Value& data)
    {
        LoadTransform(data);
        if (JsonResource::HasMember(data, "Radius") && JsonResource::IsVector3(data["Radius"]))
        {
            m_radius = JsonResource::AsVector3(data["Radius"]);
        }
        if (JsonResource::HasMember(data, "Height") && data["Height"].isDouble())
        {
            m_height = data["Height"].asFloat();
        }
        SetCapsule(m_height, m_radius);
        LoadMaterial(data);
        LoadMass(data);
    }

    void ColliderCapsule::Save(const Json::Value& data)
    {
    }

    void ColliderCapsule::EditPrimitive(CommandRegistry* registry)
    {
    }

    bool ColliderCapsule::TestRayEllipsoid(const Ray& ray, const Vector3& centroid, Real& min_t, Real& max_t) const
    {
        Vector3 radius = Radius();
        Vector3 co     = ray.position - centroid;
        Real    a      = (ray.direction.x * ray.direction.x / (radius.x * radius.x))
                + (ray.direction.y * ray.direction.y / (radius.y * radius.y))
                + (ray.direction.z * ray.direction.z / (radius.z * radius.z));
        Real b = (2.0f * ray.direction.x * co.x / (radius.x * radius.x))
                + (2.0f * ray.direction.y * co.y / (radius.y * radius.y))
                + (2.0f * ray.direction.z * co.z / (radius.z * radius.z));
        Real c = (co.x * co.x / (radius.x * radius.x))
                + (co.y * co.y / (radius.y * radius.y))
                + (co.z * co.z / (radius.z * radius.z)) - 1.0f;
        if (Math::SolveQuadratic(a, b, c, max_t, min_t) == true)
        {
            if (min_t < 0.0f && max_t < 0.0f)
            {
                return false;
            }
            if (min_t > max_t)
            {
                Real temp = min_t;
                min_t     = max_t;
                max_t     = temp;
            }
            return true;
        }
        return false;
    }
}
