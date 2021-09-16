#include "ColliderDome.hpp"
#include "../../../Graphics/Utility/PrimitiveRenderer.hpp"
#include "../../BroadPhase/BoundingAABB.hpp"
#include "../../../../Manager/Resource/ResourceType/JsonResource.hpp"
#include "../../../../External/JSONCPP/json/json.h"

namespace CS460
{
    ColliderDome::ColliderDome()
    {
        m_type = eColliderType::Dome;
    }

    ColliderDome::~ColliderDome()
    {
    }

    void ColliderDome::Initialize()
    {
        SetDome(Vector3(0.5f, 0.5f, 0.5f));
    }

    void ColliderDome::Shutdown()
    {
    }

    Vector3 ColliderDome::Support(const Vector3& direction)
    {
        Vector3 radius = Radius();
        Vector3 result = radius.HadamardProduct(radius);
        result         = result.HadamardProduct(direction);
        Real denom     = radius.HadamardProduct(direction).Length();
        result         = Math::IsZero(denom) ? result * 0.0f : result / denom;
        //check hemisphere
        if (result.y <= 0.0f)
        {
            Vector3 ellipse_dir = direction;
            ellipse_dir.y       = 0.0f;
            ellipse_dir.SetNormalize();
            Vector3 ellipse_radius = radius;
            ellipse_radius.y       = 0.0f;
            result                 = ellipse_radius.HadamardProduct(ellipse_radius);
            result                 = result.HadamardProduct(ellipse_dir);
            denom                  = ellipse_radius.HadamardProduct(ellipse_dir).Length();
            result                 = Math::IsZero(denom) ? result * 0.0f : result / denom;
        }
        return result;
    }

    bool ColliderDome::TestRayIntersection(const Ray& local_ray, Real& minimum_t, Real& maximum_t) const
    {
        Vector3 radius        = Radius();
        Real    denominator_x = 1.0f / (radius.x * radius.x);
        Real    denominator_z = 1.0f / (radius.y * radius.y);
        //quadratic
        Real a = (local_ray.direction.x * local_ray.direction.x * denominator_x)
                + (local_ray.direction.y * local_ray.direction.y / (radius.y * radius.y))
                + (local_ray.direction.z * local_ray.direction.z * denominator_z);
        Real b = (2.0f * local_ray.direction.x * local_ray.position.x * denominator_x)
                + (2.0f * local_ray.direction.y * local_ray.position.y / (radius.y * radius.y))
                + (2.0f * local_ray.direction.z * local_ray.position.z * denominator_z);
        Real c = (local_ray.position.x * local_ray.position.x * denominator_x)
                + (local_ray.position.y * local_ray.position.y / (radius.y * radius.y))
                + (local_ray.position.z * local_ray.position.z * denominator_z) - 1.0f;
        if (Math::SolveQuadratic(a, b, c, maximum_t, minimum_t) == true)
        {
            Real max_height = local_ray.direction.y * maximum_t + local_ray.position.y;
            Real min_height = local_ray.direction.y * minimum_t + local_ray.position.y;
            if (max_height < 0.0f)
            {
                if (min_height < 0.0f)
                {
                    return false;
                }
                Real disc_t = -local_ray.position.y / local_ray.direction.y;
                Real disc_a = local_ray.direction.x * disc_t + local_ray.position.x;
                Real disc_b = local_ray.direction.z * disc_t + local_ray.position.z;
                if (disc_a * disc_a * denominator_x + disc_b * disc_b * denominator_z <= 1.0f)
                {
                    maximum_t = disc_t;
                }
            }
            if (min_height < 0.0f)
            {
                Real disc_t = -local_ray.position.y / local_ray.direction.y;
                Real disc_a = local_ray.direction.x * disc_t + local_ray.position.x;
                Real disc_b = local_ray.direction.z * disc_t + local_ray.position.z;
                if (disc_a * disc_a * denominator_x + disc_b * disc_b * denominator_z <= 1.0f)
                {
                    minimum_t = disc_t;
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
        return false;
    }

    Vector3 ColliderDome::GetNormal(const Vector3& local_point_on_collider)
    {
        return local_point_on_collider;
    }

    void ColliderDome::SetMassData(Real density)
    {
        if (Math::IsNotEqual(m_material.density, density))
        {
            m_material.density = density;
            m_material.type    = Physics::eMaterial::UserType;
        }
        Real a, b, c;
        m_mass = density * GetVolume();
        if (m_collider_set != nullptr)
        {
            a = m_scaled_radius.x;
            b = m_scaled_radius.z;
            c = m_scaled_radius.y;
        }
        else
        {
            a = m_radius.x;
            b = m_radius.z;
            c = m_radius.y;
        }
        //0.059375 = 19/320
        Real it_xx = 0.2f * m_mass * b * b + 0.059375f * m_mass * c * c;
        Real it_zz = 0.2f * m_mass * a * a + 0.059375f * m_mass * c * c;
        Real it_yy = 0.2f * m_mass * (a * a + b * b);
        m_local_inertia_tensor.SetZero();
        m_local_inertia_tensor.SetDiagonal(it_xx, it_yy, it_zz);
        m_centroid.Set(0.0f, 0.375f * c, 0.0f);
    }

    Real ColliderDome::GetVolume()
    {
        if (m_collider_set != nullptr)
        {
            return 2.0f / 3.0f * Math::PI * m_scaled_radius.x * m_scaled_radius.y * m_scaled_radius.z;
        }
        return 2.0f / 3.0f * Math::PI * m_radius.x * m_radius.y * m_radius.z;
    }

    void ColliderDome::SetScaleData(const Vector3& scale)
    {
        m_scaled_radius = m_radius.HadamardProduct(scale.HadamardProduct(m_local.scale));
    }

    void ColliderDome::SetUnit()
    {
        m_radius.SetNormalize();
        UpdatePrimitive();
    }

    void ColliderDome::UpdateBoundingVolume()
    {
        Real    bounding_factor = m_scaled_radius.Length();
        Vector3 pos             = m_rigid_body != nullptr ? m_rigid_body->LocalToWorldPoint(m_local.position) : m_local.position;
        Vector3 min_max(bounding_factor, bounding_factor, bounding_factor);
        m_bounding_volume->Set(-min_max + pos, min_max + pos);

        Vector3 obb_vertices[8];
        Real    w = m_scaled_radius.x;
        Real    h = m_scaled_radius.y;
        Real    d = m_scaled_radius.z;
        obb_vertices[0].Set(+w, +h, +d);
        obb_vertices[1].Set(+w, +h, -d);
        obb_vertices[2].Set(+w, 0, +d);
        obb_vertices[3].Set(+w, 0, -d);
        obb_vertices[4].Set(-w, +h, +d);
        obb_vertices[5].Set(-w, +h, -d);
        obb_vertices[6].Set(-w, 0, +d);
        obb_vertices[7].Set(-w, 0, -d);

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

    void ColliderDome::Draw(PrimitiveRenderer* renderer, eRenderingMode mode, const Color& color) const
    {
        I32 index               = static_cast<I32>(renderer->VerticesSize(mode));
        int stack_count         = renderer->SPHERICAL_STACK_COUNT + 1; //for equal divide.
        int slice_count         = renderer->SPHERICAL_SLICE_COUNT;
        I32 half_stack_count    = stack_count / 2;
        I32 dome_vertices_count = (stack_count / 2) * (slice_count + 1) + 1;
        renderer->ReserveVertices((size_t)dome_vertices_count, mode);
        Vector3    axis_vector    = Math::Vector3::Y_AXIS;
        Transform* body_transform = GetBodyTransform();
        Vector3    radius         = Radius();
        //top vertex
        Vector3 top_vertex_local_pos = axis_vector;
        top_vertex_local_pos         = top_vertex_local_pos.HadamardProduct(radius);
        //modify rotation, translation
        top_vertex_local_pos = LocalToWorldPoint(top_vertex_local_pos);
        top_vertex_local_pos = body_transform->LocalToWorldPoint(top_vertex_local_pos);
        renderer->PushVertex(top_vertex_local_pos, mode, color);
        Real phi_step   = Math::PI / stack_count;
        Real theta_step = Math::TWO_PI / slice_count;
        // Compute vertices for each stack ring (do not count the poles as rings).
        for (I32 i = 1; i <= half_stack_count; ++i)
        {
            Real phi = i * phi_step;
            for (I32 j = 0; j <= slice_count; ++j)
            {
                Real theta = j * theta_step;
                // spherical to cartesian
                Vector3 vertex_local_pos;
                vertex_local_pos.x = sinf(phi) * cosf(theta);
                vertex_local_pos.y = cosf(phi);
                vertex_local_pos.z = sinf(phi) * sinf(theta);
                vertex_local_pos   = vertex_local_pos.HadamardProduct(radius);
                vertex_local_pos   = LocalToWorldPoint(vertex_local_pos);
                vertex_local_pos   = body_transform->LocalToWorldPoint(vertex_local_pos);
                renderer->PushVertex(vertex_local_pos, mode, color);
            }
        }
        I32 base = 1 + index;
        I32 ring = slice_count + 1;
        if (mode == eRenderingMode::Dot)
        {
            for (I32 i = 0; i < dome_vertices_count; ++i)
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
            for (I32 i = 0; i < half_stack_count; ++i)
            {
                for (I32 j = 0; j < slice_count; ++j)
                {
                    renderer->PushLineIndices(base + i * ring + j, base + i * ring + j + 1);
                    if (i < half_stack_count - 1)
                    {
                        renderer->PushLineIndices(base + i * ring + j, base + (i + 1) * ring + j);
                    }
                }
            }
        }
        else if (mode == eRenderingMode::Face)
        {
            for (I32 i = 1; i <= slice_count; ++i)
            {
                renderer->PushFaceIndices(index, index + i + 1, index + i);
            }
            for (I32 i = 0; i < half_stack_count - 1; ++i)
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
        }
    }

    Vector3 ColliderDome::Radius() const
    {
        if (m_collider_set != nullptr)
        {
            return m_scaled_radius;
        }
        return m_radius;
    }

    void ColliderDome::SetDome(const Vector3& radius)
    {
        m_radius = radius;
        UpdatePrimitive();
    }

    void ColliderDome::Clone(ColliderPrimitive* origin)
    {
        if (origin != this && origin != nullptr && origin->Type() == m_type)
        {
            ColliderDome* dome = static_cast<ColliderDome*>(origin);
            //collider local space data
            m_local = dome->m_local;
            //collider mass data
            m_centroid             = dome->m_centroid;
            m_mass                 = dome->m_mass;
            m_local_inertia_tensor = dome->m_local_inertia_tensor;
            m_material             = dome->m_material;
            //dome
            m_radius        = dome->m_radius;
            m_scaled_radius = dome->m_scaled_radius;
        }
    }

    void ColliderDome::Load(const Json::Value& data)
    {
        LoadTransform(data);
        if (JsonResource::HasMember(data, "Radius") && JsonResource::IsVector3(data["Radius"]))
        {
            m_radius = JsonResource::AsVector3(data["Radius"]);
        }
        SetDome(m_radius);
        LoadMaterial(data);
        LoadMass(data);
    }

    void ColliderDome::Save(const Json::Value& data)
    {
    }

    void ColliderDome::EditPrimitive(CommandRegistry* registry)
    {
    }
}
