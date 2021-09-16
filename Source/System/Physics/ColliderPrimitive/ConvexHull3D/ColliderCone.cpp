#include "ColliderCone.hpp"
#include "../../../Graphics/Utility/PrimitiveRenderer.hpp"
#include "../../BroadPhase/BoundingAABB.hpp"
#include "../../../../Manager/Resource/ResourceType/JsonResource.hpp"
#include "../../../../External/JSONCPP/json/json.h"

namespace CS460
{
    ColliderCone::ColliderCone()
    {
        m_type = eColliderType::Cone;
    }

    ColliderCone::~ColliderCone()
    {
    }

    void ColliderCone::Initialize()
    {
        SetCone(1.0f, Vector2(0.5f, 0.5f));
    }

    void ColliderCone::Shutdown()
    {
    }

    Vector3 ColliderCone::Support(const Vector3& direction)
    {
        Vector2 radius = Radius();
        Vector3 axis_vector;
        axis_vector.y       = HalfHeight();
        Vector3 ellipse_dir = direction;
        ellipse_dir.y       = 0.0f;
        ellipse_dir.SetNormalize();
        Vector3 ellipse_radius(radius.x, 0.0f, radius.y);
        Vector3 ellipse_vector = ellipse_radius.HadamardProduct(ellipse_radius);
        ellipse_vector         = ellipse_vector.HadamardProduct(ellipse_dir);
        Real denom             = ellipse_radius.HadamardProduct(ellipse_dir).Length();
        ellipse_vector         = Math::IsZero(denom) ? ellipse_vector * 0.0f : ellipse_vector / denom;
        Real point_support     = direction.DotProduct(axis_vector);
        Real ellipse_support   = direction.DotProduct(ellipse_vector - axis_vector);
        return point_support >= ellipse_support ? axis_vector : ellipse_vector - axis_vector;
    }

    bool ColliderCone::TestRayIntersection(const Ray& local_ray, Real& minimum_t, Real& maximum_t) const
    {
        Real    half_height = Height() * 0.5f;
        Vector2 radius      = Radius();
        Real    height      = Height();
        minimum_t           = -1.0f;
        maximum_t           = -1.0f;
        Real denominator_x  = 1.0f / (radius.x * radius.x);
        Real denominator_y  = 1.0f / (height * height);
        Real denominator_z  = 1.0f / (radius.y * radius.y);
        //make quadratic
        Real a = local_ray.direction.x * local_ray.direction.x * denominator_x
                + local_ray.direction.z * local_ray.direction.z * denominator_z
                - local_ray.direction.y * local_ray.direction.y * denominator_y;
        Real b = 2.0f * local_ray.direction.x * local_ray.position.x * denominator_x
                + 2.0f * local_ray.direction.z * local_ray.position.z * denominator_z
                - 2.0f * local_ray.direction.y * local_ray.position.y * denominator_y
                + local_ray.direction.y / height;
        Real c = local_ray.position.x * local_ray.position.x * denominator_x
                + local_ray.position.z * local_ray.position.z * denominator_z
                - local_ray.position.y * local_ray.position.y * denominator_y
                + local_ray.position.y / height - 0.25f;
        Real cone_min_t, cone_max_t;
        if (Math::SolveQuadratic(a, b, c, cone_max_t, cone_min_t) == true)
        {
            if (cone_min_t > cone_max_t)
            {
                Real temp  = cone_min_t;
                cone_min_t = cone_max_t;
                cone_max_t = temp;
            }
            Real min_axis_height = local_ray.position.y + local_ray.direction.y * cone_min_t;
            Real max_axis_height = local_ray.position.y + local_ray.direction.y * cone_max_t;
            if (min_axis_height > half_height)
            {
                if (max_axis_height > half_height)
                {
                    return false;
                }
                if (max_axis_height < -half_height)
                {
                    return false;
                }
                //cone_min_t intersect shadow cone.
                minimum_t = cone_max_t;
                maximum_t = (-half_height - local_ray.position.y) / local_ray.direction.y;
            }
            else
            {
                if (min_axis_height < -half_height)
                {
                    //test_disc
                    Real disc_t = (-half_height - local_ray.position.y) / local_ray.direction.y;
                    Real disc_a = local_ray.direction.x * disc_t + local_ray.position.x;
                    Real disc_b = local_ray.direction.z * disc_t + local_ray.position.z;
                    if (disc_a * disc_a * denominator_x + disc_b * disc_b * denominator_z <= 1.0f)
                    {
                        minimum_t = disc_t;
                        maximum_t = cone_max_t;
                    }
                    else
                    {
                        return false;
                    }
                }
                else
                {
                    minimum_t = cone_min_t;
                    maximum_t = cone_max_t;
                }
                if (max_axis_height < -half_height)
                {
                    //test disc
                    Real disc_t = (-half_height - local_ray.position.y) / local_ray.direction.y;
                    Real disc_a = local_ray.direction.x * disc_t + local_ray.position.x;
                    Real disc_b = local_ray.direction.z * disc_t + local_ray.position.z;
                    if (disc_a * disc_a * denominator_x + disc_b * disc_b * denominator_z <= 1.0f)
                    {
                        maximum_t = disc_t;
                    }
                    else
                    {
                        return false;
                    }
                }
                if (max_axis_height > half_height)
                {
                    //case when intersect disc first after then intersect cone.
                    if (min_axis_height <= half_height)
                    {
                        maximum_t = minimum_t;
                        minimum_t = (-half_height - local_ray.position.y) / local_ray.direction.y;
                    }
                    else
                    {
                        //intersect shadow cone.
                        return false;
                    }
                }
            }
            if (Math::IsEqual(min_axis_height, half_height) == true)
            {
                Real disc_t = (-half_height - local_ray.position.y) / local_ray.direction.y;
                Real disc_a = local_ray.direction.x * disc_t + local_ray.position.x;
                Real disc_b = local_ray.direction.z * disc_t + local_ray.position.z;
                if (disc_a * disc_a * denominator_x + disc_b * disc_b * denominator_z <= 1.0f)
                {
                    minimum_t = disc_t;
                    if (minimum_t > maximum_t)
                    {
                        Real temp = minimum_t;
                        minimum_t = maximum_t;
                        maximum_t = temp;
                    }
                }
            }
        }
        else
        {
            return false;
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

    Vector3 ColliderCone::GetNormal(const Vector3& local_point_on_collider)
    {
        Real    half_height   = HalfHeight();
        Vector2 radius        = Radius();
        Real    height        = Height();
        Real    n_ratio       = 0.5f + -local_point_on_collider.y / height;
        Real    x_max         = local_point_on_collider.x / n_ratio;
        Real    z_max         = local_point_on_collider.z / n_ratio;
        Real    denominator_x = 1.0f / (radius.x * radius.x);
        Real    denominator_y = 1.0f / (height * height);
        Real    denominator_z = 1.0f / (radius.y * radius.y);
        Real    t             = 2.0f * (local_point_on_collider.x * x_max * denominator_x + local_point_on_collider.z * z_max * denominator_z) * denominator_y;
        Real    point_radius  = local_point_on_collider.x * local_point_on_collider.x * denominator_x + local_point_on_collider.z * local_point_on_collider.z * denominator_z;
        Vector3 normal;
        Real    point_height = local_point_on_collider.y;
        if (Math::IsEqual(point_height, half_height)
            || (Math::IsEqual(point_height, -half_height)
                && 1.0f > point_radius))
        {
            // point on top point or bottom disc 
            normal.y = Math::Signum(point_height);
        }
        else if (Math::IsEqual(point_height, -half_height)
            && Math::IsEqual(1.0f, point_radius))
        {
            // point on cylinder disc.
            normal.x = 2.0f * local_point_on_collider.x * denominator_x;
            normal.z = 2.0f * local_point_on_collider.z * denominator_z;
            normal.SetNormalize();
        }
        else
        {
            normal.x = 2.0f * local_point_on_collider.x * denominator_x;
            normal.y = height * t;
            normal.z = 2.0f * local_point_on_collider.z * denominator_z;
            normal.SetNormalize();
        }
        return normal;
    }

    void ColliderCone::SetMassData(Real density)
    {
        if (Math::IsNotEqual(m_material.density, density))
        {
            m_material.density = density;
            m_material.type    = Physics::eMaterial::UserType;
        }
        Real a, b, h;
        m_mass = density * GetVolume();
        if (m_collider_set != nullptr)
        {
            a = m_scaled_radius.x;
            b = m_scaled_radius.y;
            h = m_scaled_height;
        }
        else
        {
            a = m_radius.x;
            b = m_radius.y;
            h = m_height;
        }
        Real it_xx = 0.15f * m_mass * b * b + 0.0375f * m_mass * h * h;
        Real it_zz = 0.15f * m_mass * a * a + 0.0375f * m_mass * h * h;
        Real it_yy = 0.15f * m_mass * (a * a + b * b);
        m_local_inertia_tensor.SetZero();
        m_local_inertia_tensor.SetDiagonal(it_xx, it_yy, it_zz);
        m_centroid = Vector3(0.0f, h * 0.25f - HalfHeight(), 0.0f);
    }

    Real ColliderCone::GetVolume()
    {
        if (m_collider_set != nullptr)
        {
            return Math::PI * m_scaled_radius.x * m_scaled_radius.y * m_scaled_height / 3.0f;
        }
        return Math::PI * m_radius.x * m_radius.y * m_height / 3.0f;
    }

    void ColliderCone::SetScaleData(const Vector3& scale)
    {
        Vector3 new_scale = scale.HadamardProduct(m_local.scale);
        m_scaled_height   = m_height * new_scale.y;
        m_scaled_radius   = m_radius.HadamardProduct(Vector2(new_scale.x, new_scale.z));
    }

    void ColliderCone::SetUnit()
    {
        Real division;
        if (m_radius.Length() * 2.0f > m_height)
        {
            division = m_radius.Length() * 2.0f;
        }
        else
        {
            division = m_height;
        }
        if (division > 0.0f)
        {
            m_radius /= division;
            m_height /= division;
        }
        UpdatePrimitive();
    }

    void ColliderCone::UpdateBoundingVolume()
    {
        Vector3 obb_vertices[8];
        Real    w = m_scaled_radius.x;
        Real    h = m_scaled_height * 0.5f;
        Real    d = m_scaled_radius.y;
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

    void ColliderCone::Draw(PrimitiveRenderer* renderer, eRenderingMode mode, const Color& color) const
    {
        I32 index       = static_cast<I32>(renderer->VerticesSize(mode));
        int stack_count = renderer->CYLINDRICAL_STACK_COUNT;
        int slice_count = renderer->CYLINDRICAL_SLICE_COUNT;
        renderer->ReserveVertices(renderer->CYLINDRICAL_VERTICES_COUNT, mode);
        Real       height         = Height();
        Vector2    radius         = Radius();
        Transform* body_transform = GetBodyTransform();
        Real       stack_height   = height / stack_count;
        Real       radius_step    = - 1.0f / stack_count;
        I32        ring_count     = stack_count + 1;
        for (I32 i = 0; i < ring_count; ++i)
        {
            Real y       = -0.5f * height + i * stack_height;
            Real r       = 1.0f + i * radius_step;
            Real d_theta = Math::TWO_PI / slice_count;
            for (I32 j = 0; j <= slice_count; ++j)
            {
                Real    c = cosf(j * d_theta);
                Real    s = sinf(j * d_theta);
                Vector3 vertex_local_pos;
                vertex_local_pos.x = radius.x * r * c;
                vertex_local_pos.y = y;
                vertex_local_pos.z = radius.y * r * s;
                vertex_local_pos   = LocalToWorldPoint(vertex_local_pos);
                vertex_local_pos   = body_transform->LocalToWorldPoint(vertex_local_pos);
                renderer->PushVertex(vertex_local_pos, mode, color);
            }
        }
        I32 ring = slice_count + 1;
        if (mode == eRenderingMode::Dot)
        {
            I32 count = renderer->CYLINDRICAL_VERTICES_COUNT;
            for (I32 i = 0; i < count; ++i)
            {
                renderer->PushIndex(index + i, mode);
            }
        }
        else if (mode == eRenderingMode::Line)
        {
            for (I32 i = 0; i < stack_count; ++i)
            {
                for (I32 j = 0; j < slice_count; ++j)
                {
                    renderer->PushLineIndices(index + (i * ring + j), index + ((i + 1) * ring + j));
                    renderer->PushLineIndices(index + ((i + 1) * ring + j), index + ((i + 1) * ring + j + 1));
                    renderer->PushLineIndices(index + (i * ring + j), index + (i * ring + j + 1));
                    renderer->PushLineIndices(index + ((i + 1) * ring + j + 1), index + (i * ring + j + 1));
                }
            }
        }
        else if (mode == eRenderingMode::Face)
        {
            for (I32 i = 0; i < stack_count; ++i)
            {
                for (I32 j = 0; j < slice_count; ++j)
                {
                    renderer->PushFaceIndices(index + (i * ring + j), index + ((i + 1) * ring + j), index + (i + 1) * ring + j + 1);
                    renderer->PushFaceIndices(index + (i * ring + j), index + ((i + 1) * ring + j + 1), index + (i * ring + j + 1));
                }
            }
        }
    }

    Real ColliderCone::HalfHeight() const
    {
        if (m_collider_set != nullptr)
        {
            return m_scaled_height * 0.5f;
        }
        return m_height * 0.5f;
    }

    Real ColliderCone::Height() const
    {
        if (m_collider_set != nullptr)
        {
            return m_scaled_height;
        }
        return m_height;
    }

    Vector2 ColliderCone::Radius() const
    {
        if (m_collider_set != nullptr)
        {
            return m_scaled_radius;
        }
        return m_radius;
    }

    void ColliderCone::SetCone(Real height, const Vector2& radius)
    {
        m_height = height;
        m_radius = radius;
        UpdatePrimitive();
    }

    void ColliderCone::SetHeight(Real height)
    {
        m_height = height;
        UpdatePrimitive();
    }

    void ColliderCone::SetRadius(const Vector2& radius)
    {
        m_radius = radius;
        UpdatePrimitive();
    }

    void ColliderCone::Clone(ColliderPrimitive* origin)
    {
        if (origin != this && origin != nullptr && origin->Type() == m_type)
        {
            ColliderCone* cone = static_cast<ColliderCone*>(origin);
            //collider local space data
            m_local = cone->m_local;
            //collider mass data
            m_centroid             = cone->m_centroid;
            m_mass                 = cone->m_mass;
            m_local_inertia_tensor = cone->m_local_inertia_tensor;
            m_material             = cone->m_material;
            //cone
            m_radius        = cone->m_radius;
            m_height        = cone->m_height;
            m_scaled_radius = cone->m_scaled_radius;
            m_scaled_height = cone->m_scaled_height;
        }
    }

    void ColliderCone::Load(const Json::Value& data)
    {
        LoadTransform(data);
        if (JsonResource::HasMember(data, "Radius") && JsonResource::IsVector2(data["Radius"]))
        {
            m_radius = JsonResource::AsVector2(data["Radius"]);
        }
        if (JsonResource::HasMember(data, "Height") && data["Height"].isDouble())
        {
            m_height = data["Height"].asFloat();
        }
        SetCone(m_height, m_radius);
        LoadMaterial(data);
        LoadMass(data);
    }

    void ColliderCone::Save(const Json::Value& data)
    {
    }

    void ColliderCone::EditPrimitive(CommandRegistry* registry)
    {
    }
}
