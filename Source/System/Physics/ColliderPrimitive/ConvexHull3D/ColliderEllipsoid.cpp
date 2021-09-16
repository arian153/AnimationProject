#include "ColliderEllipsoid.hpp"
#include "../../../Graphics/Utility/PrimitiveRenderer.hpp"
#include "../../BroadPhase/BoundingAABB.hpp"
#include "../../../../Manager/Resource/ResourceType/JsonResource.hpp"
#include "../../../../External/JSONCPP/json/json.h"

namespace CS460
{
    ColliderEllipsoid::ColliderEllipsoid()
    {
        m_type = eColliderType::Ellipsoid;
    }

    ColliderEllipsoid::~ColliderEllipsoid()
    {
    }

    void ColliderEllipsoid::Initialize()
    {
        SetEllipsoid(Vector3(0.5f, 0.5f, 0.5f));
    }

    void ColliderEllipsoid::Shutdown()
    {
    }

    Vector3 ColliderEllipsoid::Support(const Vector3& direction)
    {
        Vector3 radius = Radius();
        Vector3 result = radius.HadamardProduct(radius);
        result         = result.HadamardProduct(direction);
        result /= radius.HadamardProduct(direction).Length();
        return result;
    }

    bool ColliderEllipsoid::TestRayIntersection(const Ray& local_ray, Real& minimum_t, Real& maximum_t) const
    {
        Vector3 radius = Radius();
        Real    a      = (local_ray.direction.x * local_ray.direction.x / (radius.x * radius.x))
                + (local_ray.direction.y * local_ray.direction.y / (radius.y * radius.y))
                + (local_ray.direction.z * local_ray.direction.z / (radius.z * radius.z));
        Real b = (2.0f * local_ray.direction.x * local_ray.position.x / (radius.x * radius.x))
                + (2.0f * local_ray.direction.y * local_ray.position.y / (radius.y * radius.y))
                + (2.0f * local_ray.direction.z * local_ray.position.z / (radius.z * radius.z));
        Real c = (local_ray.position.x * local_ray.position.x / (radius.x * radius.x))
                + (local_ray.position.y * local_ray.position.y / (radius.y * radius.y))
                + (local_ray.position.z * local_ray.position.z / (radius.z * radius.z)) - 1.0f;
        if (Math::SolveQuadratic(a, b, c, maximum_t, minimum_t) == true)
        {
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

    Vector3 ColliderEllipsoid::GetNormal(const Vector3& local_point_on_collider)
    {
        Vector3 normal;
        if (m_collider_set != nullptr)
        {
            normal.x = 2.0f * local_point_on_collider.x / m_scaled_radius.x * m_scaled_radius.x;
            normal.y = 2.0f * local_point_on_collider.y / m_scaled_radius.y * m_scaled_radius.y;
            normal.z = 2.0f * local_point_on_collider.z / m_scaled_radius.z * m_scaled_radius.z;
        }
        else
        {
            normal.x = 2.0f * local_point_on_collider.x / m_radius.x * m_radius.x;
            normal.y = 2.0f * local_point_on_collider.y / m_radius.y * m_radius.y;
            normal.z = 2.0f * local_point_on_collider.z / m_radius.z * m_radius.z;
        }
        return normal;
    }

    void ColliderEllipsoid::SetMassData(Real density)
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
        Real it_xx = 0.2f * m_mass * (b * b + c * c);
        Real it_yy = 0.2f * m_mass * (a * a + b * b);
        Real it_zz = 0.2f * m_mass * (a * a + c * c);
        m_local_inertia_tensor.SetZero();
        m_local_inertia_tensor.SetDiagonal(it_xx, it_yy, it_zz);
        m_centroid.SetZero();
    }

    Real ColliderEllipsoid::GetVolume()
    {
        if (m_collider_set != nullptr)
        {
            return 4.0f / 3.0f * Math::PI * m_scaled_radius.x * m_scaled_radius.y * m_scaled_radius.z;
        }
        return 4.0f / 3.0f * Math::PI * m_radius.x * m_radius.y * m_radius.z;
    }

    void ColliderEllipsoid::SetScaleData(const Vector3& scale)
    {
        m_scaled_radius = m_radius.HadamardProduct(scale.HadamardProduct(m_local.scale));
    }

    void ColliderEllipsoid::SetUnit()
    {
        m_radius.SetNormalize();
        UpdatePrimitive();
    }

    void ColliderEllipsoid::UpdateBoundingVolume()
    {
        Vector3 obb_vertices[8];
        Real    w = m_scaled_radius.x;
        Real    h = m_scaled_radius.y;
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

    void ColliderEllipsoid::Draw(PrimitiveRenderer* renderer, eRenderingMode mode, const Color& color) const
    {
        I32 index       = static_cast<I32>(renderer->VerticesSize(mode));
        int stack_count = renderer->SPHERICAL_STACK_COUNT;
        int slice_count = renderer->SPHERICAL_SLICE_COUNT;
        renderer->ReserveVertices(renderer->SPHERICAL_VERTICES_COUNT, mode);
        Vector3    axis_vector    = Math::Vector3::Y_AXIS;
        Transform* body_transform = GetBodyTransform();
        Vector3    radius         = Radius();
        //top vertex
        Vector3 top_vertex_local_pos = axis_vector;
        top_vertex_local_pos         = top_vertex_local_pos.HadamardProduct(radius);
        //modify rotation, translation
        top_vertex_local_pos = LocalToWorldPoint(top_vertex_local_pos);
        top_vertex_local_pos = body_transform->LocalToWorldPoint(top_vertex_local_pos);
        //bottom vertex
        Vector3 bottom_vertex_local_pos = -axis_vector;
        bottom_vertex_local_pos         = bottom_vertex_local_pos.HadamardProduct(radius);
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
                vertex_local_pos   = LocalToWorldPoint(vertex_local_pos);
                vertex_local_pos   = body_transform->LocalToWorldPoint(vertex_local_pos);
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

    Vector3 ColliderEllipsoid::Radius() const
    {
        if (m_collider_set != nullptr)
        {
            return m_scaled_radius;
        }
        return m_radius;
    }

    void ColliderEllipsoid::SetEllipsoid(const Vector3& radius)
    {
        m_radius = radius;
        UpdatePrimitive();
    }

    void ColliderEllipsoid::Clone(ColliderPrimitive* origin)
    {
        if (origin != this && origin != nullptr && origin->Type() == m_type)
        {
            ColliderEllipsoid* ellipsoid = static_cast<ColliderEllipsoid*>(origin);
            //collider local space data
            m_local = ellipsoid->m_local;
            //collider mass data
            m_centroid             = ellipsoid->m_centroid;
            m_mass                 = ellipsoid->m_mass;
            m_local_inertia_tensor = ellipsoid->m_local_inertia_tensor;
            m_material             = ellipsoid->m_material;
            //ellipsoid
            m_radius        = ellipsoid->m_radius;
            m_scaled_radius = ellipsoid->m_scaled_radius;
        }
    }

    void ColliderEllipsoid::Load(const Json::Value& data)
    {
        LoadTransform(data);
        //ellipsoid data
        if (JsonResource::HasMember(data, "Radius") && JsonResource::IsVector3(data["Radius"]))
        {
            m_radius = JsonResource::AsVector3(data["Radius"]);
        }
        SetEllipsoid(m_radius);
        LoadMaterial(data);
        LoadMass(data);
    }

    void ColliderEllipsoid::Save(const Json::Value& data)
    {
    }

    void ColliderEllipsoid::EditPrimitive(CommandRegistry* registry)
    {
    }
}
