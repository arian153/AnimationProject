#include "ColliderSphere.hpp"
#include "../../../Graphics/Utility/PrimitiveRenderer.hpp"
#include "../../BroadPhase/BoundingAABB.hpp"
#include "../../../../Manager/Resource/ResourceType/JsonResource.hpp"
#include "../../../../External/JSONCPP/json/json.h"

namespace CS460
{
    ColliderSphere::ColliderSphere()
    {
        m_type = eColliderType::Sphere;
    }

    ColliderSphere::~ColliderSphere()
    {
    }

    void ColliderSphere::Initialize()
    {
        SetSphere(0.5f);
    }

    void ColliderSphere::Shutdown()
    {
    }

    Vector3 ColliderSphere::Support(const Vector3& direction)
    {
        return Radius() * direction;
    }

    bool ColliderSphere::TestRayIntersection(const Ray& local_ray, Real& minimum_t, Real& maximum_t) const
    {
        Real radius = Radius();
        Real a      = local_ray.direction.x * local_ray.direction.x
                + local_ray.direction.y * local_ray.direction.y
                + local_ray.direction.z * local_ray.direction.z;
        Real b = 2.0f * (local_ray.direction.x * local_ray.position.x
            + local_ray.direction.y * local_ray.position.y
            + local_ray.direction.z * local_ray.position.z);
        Real c = local_ray.position.x * local_ray.position.x
                + local_ray.position.y * local_ray.position.y
                + local_ray.position.z * local_ray.position.z - radius * radius;
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

    Vector3 ColliderSphere::GetNormal(const Vector3& local_point_on_collider)
    {
        return local_point_on_collider.Normalize();
    }

    void ColliderSphere::SetMassData(Real density)
    {
        if (Math::IsNotEqual(m_material.density, density))
        {
            m_material.density = density;
            m_material.type    = Physics::eMaterial::UserType;
        }
        m_mass  = density * GetVolume();
        Real it = m_mass * 0.4f * Radius() * Radius();
        m_local_inertia_tensor.SetZero();
        m_local_inertia_tensor.SetDiagonal(it, it, it);
        m_centroid.SetZero();
    }

    Real ColliderSphere::GetVolume()
    {
        if (m_collider_set != nullptr)
        {
            return 4.0f / 3.0f * Math::PI * m_scaled_radius * m_scaled_radius * m_scaled_radius;
        }
        return 4.0f / 3.0f * Math::PI * m_radius * m_radius * m_radius;
    }

    void ColliderSphere::SetScaleData(const Vector3& scale)
    {
        m_scaled_radius = m_radius * scale.HadamardProduct(m_local.scale).Largest();
    }

    void ColliderSphere::SetUnit()
    {
        m_radius = 0.5f;
        UpdatePrimitive();
    }

    void ColliderSphere::UpdateBoundingVolume()
    {
        Real    bounding_factor = m_scaled_radius;
        Vector3 pos = m_rigid_body != nullptr ? m_rigid_body->LocalToWorldPoint(m_local.position) : m_local.position;
        Vector3 min_max(bounding_factor, bounding_factor, bounding_factor);
        m_bounding_volume->Set(-min_max + pos, min_max + pos);
    }

    void ColliderSphere::Draw(PrimitiveRenderer* renderer, eRenderingMode mode, const Color& color) const
    {
        I32 index       = static_cast<I32>(renderer->VerticesSize(mode));
        int stack_count = renderer->SPHERICAL_STACK_COUNT;
        int slice_count = renderer->SPHERICAL_SLICE_COUNT;
        renderer->ReserveVertices(renderer->SPHERICAL_VERTICES_COUNT, mode);
        Vector3    axis_vector    = Math::Vector3::Y_AXIS;
        Transform* body_transform = GetBodyTransform();
        Real       radius         = Radius();
        //top vertex
        Vector3 top_vertex_local_pos = axis_vector;
        top_vertex_local_pos         = top_vertex_local_pos * radius;
        //modify rotation, translation
        top_vertex_local_pos = LocalToWorldPoint(top_vertex_local_pos);
        top_vertex_local_pos = body_transform->LocalToWorldPoint(top_vertex_local_pos);
        //bottom vertex
        Vector3 bottom_vertex_local_pos = -axis_vector;
        bottom_vertex_local_pos         = bottom_vertex_local_pos * radius;
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
                vertex_local_pos   = vertex_local_pos * radius;
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

    Real ColliderSphere::Radius() const
    {
        if (m_collider_set != nullptr)
        {
            return m_scaled_radius;
        }
        return m_radius;
    }

    void ColliderSphere::SetSphere(Real radius)
    {
        m_radius = radius;
        UpdatePrimitive();
    }

    void ColliderSphere::Clone(ColliderPrimitive* origin)
    {
        if (origin != this && origin != nullptr && origin->Type() == m_type)
        {
            ColliderSphere* sphere = static_cast<ColliderSphere*>(origin);
            //collider local space data
            m_local = sphere->m_local;
            //collider mass data
            m_centroid             = sphere->m_centroid;
            m_mass                 = sphere->m_mass;
            m_local_inertia_tensor = sphere->m_local_inertia_tensor;
            m_material             = sphere->m_material;
            //sphere
            m_radius        = sphere->m_radius;
            m_scaled_radius = sphere->m_scaled_radius;
        }
    }

    void ColliderSphere::Load(const Json::Value& data)
    {
        LoadTransform(data);
        if (JsonResource::HasMember(data, "Radius") && data["Radius"].isDouble())
        {
            m_radius = data["Radius"].asFloat();
        }
        SetSphere(m_radius);
        LoadMaterial(data);
        LoadMass(data);
    }

    void ColliderSphere::Save(const Json::Value& data)
    {
    }

    void ColliderSphere::EditPrimitive(CommandRegistry* registry)
    {
    }
}
