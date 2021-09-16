#include "ColliderTruncated.hpp"
#include "../../../Graphics/Utility/PrimitiveRenderer.hpp"
#include "../../BroadPhase/BoundingAABB.hpp"
#include "../../../../Manager/Resource/ResourceType/JsonResource.hpp"
#include "../../../../External/JSONCPP/json/json.h"

namespace CS460
{
    ColliderTruncated::ColliderTruncated()
    {
        m_type = eColliderType::Truncated;
    }

    ColliderTruncated::~ColliderTruncated()
    {
    }

    void ColliderTruncated::Initialize()
    {
        SetTruncated(1.0f, Vector2(0.5f, 0.5f), 0.5f);
    }

    void ColliderTruncated::Shutdown()
    {
    }

    Vector3 ColliderTruncated::Support(const Vector3& direction)
    {
        Vector2 radius = Radius();
        Vector3 axis_vector;
        axis_vector.y = HalfHeight();
        Vector3 result;
        Vector3 ellipse_dir = direction;
        ellipse_dir.y       = 0.0f;
        ellipse_dir.SetNormalize();
        Vector3 ellipse_radius(radius.x, 0.0f, radius.y);
        Vector3 ellipse_vector = ellipse_radius.HadamardProduct(ellipse_radius);
        ellipse_vector         = ellipse_vector.HadamardProduct(ellipse_dir);
        Real denom             = ellipse_radius.HadamardProduct(ellipse_dir).Length();
        ellipse_vector         = Math::IsZero(denom) ? ellipse_vector * 0.0f : ellipse_vector / denom;
        Real top_support       = direction.DotProduct(m_ratio * ellipse_vector + axis_vector);
        Real bot_support       = direction.DotProduct(ellipse_vector - axis_vector);
        if (top_support > bot_support)
        {
            result = m_ratio * ellipse_vector + axis_vector;
        }
        else
        {
            result = ellipse_vector - axis_vector;
        }
        return result;
    }

    bool ColliderTruncated::TestRayIntersection(const Ray& local_ray, Real& minimum_t, Real& maximum_t) const
    {
        Real    half_height     = Height() * 0.5f;
        Vector2 radius          = Radius();
        Real    height          = Height();
        minimum_t               = -1.0f;
        maximum_t               = -1.0f;
        Real denominator_x      = 1.0f / (radius.x * radius.x);
        Real denominator_y      = 1.0f / (height * height);
        Real denominator_z      = 1.0f / (radius.y * radius.y);
        Real ratio_multiplier_a = (m_ratio - 1.0f) * (m_ratio - 1.0f);
        Real ratio_multiplier_b = (m_ratio + 1.0f) * (m_ratio + 1.0f);
        Real ratio_multiplier_c = m_ratio * m_ratio - 1.0f;
        //make quadratic
        Real a = local_ray.direction.x * local_ray.direction.x * denominator_x
                + local_ray.direction.z * local_ray.direction.z * denominator_z
                - local_ray.direction.y * local_ray.direction.y * ratio_multiplier_a * denominator_y;
        Real b = 2.0f * local_ray.direction.x * local_ray.position.x * denominator_x
                + 2.0f * local_ray.direction.z * local_ray.position.z * denominator_z
                - 2.0f * local_ray.direction.y * local_ray.position.y * ratio_multiplier_a * denominator_y
                - local_ray.direction.y * ratio_multiplier_c / height;
        Real c = local_ray.position.x * local_ray.position.x * denominator_x
                + local_ray.position.z * local_ray.position.z * denominator_z
                - local_ray.position.y * local_ray.position.y * ratio_multiplier_a * denominator_y
                - local_ray.position.y * ratio_multiplier_c / height
                - 0.25f * ratio_multiplier_b;
        Real truncated_min_t, truncated_max_t;
        bool b_disc_dir         = !Math::IsZero(local_ray.direction.y);
        Real top_disc_t         = b_disc_dir ? (half_height - local_ray.position.y) / local_ray.direction.y : -1.0f;
        Real bot_disc_t         = b_disc_dir ? (-half_height - local_ray.position.y) / local_ray.direction.y : -1.0f;
        Real top_disc_a         = local_ray.direction.x * top_disc_t + local_ray.position.x;
        Real top_disc_b         = local_ray.direction.z * top_disc_t + local_ray.position.z;
        Real bot_disc_a         = local_ray.direction.x * bot_disc_t + local_ray.position.x;
        Real bot_disc_b         = local_ray.direction.z * bot_disc_t + local_ray.position.z;
        bool b_contain_top_disc = b_disc_dir ? top_disc_a * top_disc_a * denominator_x + top_disc_b * top_disc_b * denominator_z <= m_ratio * m_ratio : false;
        bool b_contain_bot_disc = b_disc_dir ? bot_disc_a * bot_disc_a * denominator_x + bot_disc_b * bot_disc_b * denominator_z <= 1.0f : false;
        if (Math::SolveQuadratic(a, b, c, truncated_max_t, truncated_min_t) == true)
        {
            if (truncated_min_t > truncated_max_t)
            {
                Real temp       = truncated_min_t;
                truncated_min_t = truncated_max_t;
                truncated_max_t = temp;
            }
            if (bot_disc_t > top_disc_t)
            {
                Real temp          = top_disc_t;
                top_disc_t         = bot_disc_t;
                bot_disc_t         = temp;
                bool temp2         = b_contain_top_disc;
                b_contain_top_disc = b_contain_bot_disc;
                b_contain_bot_disc = temp2;
            }
            minimum_t            = truncated_min_t;
            maximum_t            = truncated_max_t;
            Real min_axis_height = local_ray.position.y + local_ray.direction.y * truncated_min_t;
            Real max_axis_height = local_ray.position.y + local_ray.direction.y * truncated_max_t;
            if (min_axis_height > half_height)
            {
                if (max_axis_height > half_height)
                {
                    if (b_contain_top_disc && b_contain_bot_disc)
                    {
                        minimum_t = bot_disc_t;
                        maximum_t = top_disc_t;
                    }
                    else
                    {
                        return false;
                    }
                }
                if (b_contain_top_disc)
                {
                    minimum_t = top_disc_t;
                }
                if (b_contain_bot_disc)
                {
                    minimum_t = bot_disc_t;
                }
            }
            else if (min_axis_height < -half_height)
            {
                if (max_axis_height < -half_height)
                {
                    if (b_contain_top_disc && b_contain_bot_disc)
                    {
                        minimum_t = bot_disc_t;
                        maximum_t = top_disc_t;
                    }
                    else
                    {
                        return false;
                    }
                }
                if (b_contain_top_disc)
                {
                    minimum_t = top_disc_t;
                }
                if (b_contain_bot_disc)
                {
                    minimum_t = bot_disc_t;
                }
            }
            if (max_axis_height > half_height)
            {
                if (min_axis_height < -half_height)
                {
                    if (b_contain_top_disc && b_contain_bot_disc)
                    {
                        minimum_t = bot_disc_t;
                        maximum_t = top_disc_t;
                    }
                    else
                    {
                        return false;
                    }
                }
                if (b_contain_bot_disc)
                {
                    maximum_t = bot_disc_t;
                }
                if (b_contain_top_disc)
                {
                    maximum_t = top_disc_t;
                }
            }
            else if (max_axis_height < -half_height)
            {
                if (min_axis_height > half_height)
                {
                    if (b_contain_top_disc && b_contain_bot_disc)
                    {
                        minimum_t = bot_disc_t;
                        maximum_t = top_disc_t;
                    }
                    else
                    {
                        return false;
                    }
                }
                if (b_contain_bot_disc)
                {
                    maximum_t = bot_disc_t;
                }
                if (b_contain_top_disc)
                {
                    maximum_t = top_disc_t;
                }
            }
        }
        else
        {
            if (m_ratio < 1.0f)
            {
                if (b_contain_bot_disc)
                {
                    minimum_t = bot_disc_t;
                    if (b_contain_top_disc)
                    {
                        maximum_t = top_disc_t;
                    }
                    else
                    {
                        if (Math::IsEqual(truncated_max_t, truncated_min_t))
                        {
                            maximum_t = truncated_max_t;
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
                if (b_contain_top_disc)
                {
                    minimum_t = top_disc_t;
                    if (b_contain_bot_disc)
                    {
                        maximum_t = bot_disc_t;
                    }
                    else
                    {
                        if (Math::IsEqual(truncated_max_t, truncated_min_t))
                        {
                            maximum_t = truncated_max_t;
                        }
                    }
                }
                else
                {
                    return false;
                }
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

    Vector3 ColliderTruncated::GetNormal(const Vector3& local_point_on_collider)
    {
        Real    half_height   = HalfHeight();
        Vector2 radius        = Radius();
        Real    height        = Height();
        Real    n_ratio       = (m_ratio + 1.0f) * 0.5f + local_point_on_collider.y * (m_ratio - 1.0f) / height;
        Real    x_max         = local_point_on_collider.x / n_ratio;
        Real    z_max         = local_point_on_collider.z / n_ratio;
        Real    denominator_x = 1.0f / (radius.x * radius.x);
        Real    denominator_y = 1.0f / (height * height);
        Real    denominator_z = 1.0f / (radius.y * radius.y);
        Real    t             = 2.0f * (1.0f - m_ratio) * (local_point_on_collider.x * x_max * denominator_x + local_point_on_collider.z * z_max * denominator_z) * denominator_y;
        Real    point_radius  = local_point_on_collider.x * local_point_on_collider.x * denominator_x + local_point_on_collider.z * local_point_on_collider.z * denominator_z;
        Vector3 normal;
        if (Math::IsEqual(local_point_on_collider.y, half_height) && point_radius < m_ratio * m_ratio)
        {
            normal.y = Math::Signum(local_point_on_collider.y);
        }
        else if (Math::IsEqual(local_point_on_collider.y, -half_height) && point_radius < 1.0f)
        {
            normal.y = Math::Signum(local_point_on_collider.y);
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

    void ColliderTruncated::SetMassData(Real density)
    {
        if (Math::IsNotEqual(m_material.density, density))
        {
            m_material.density = density;
            m_material.type    = Physics::eMaterial::UserType;
        }
        Real a, b, h;
        m_mass             = density * GetVolume();
        Real ratio         = (m_ratio * m_ratio + m_ratio + 1.0f);
        Real ratio_multi_a = (m_ratio * m_ratio * m_ratio * m_ratio + m_ratio * m_ratio * m_ratio + m_ratio * m_ratio + m_ratio + 1.0f) / ratio;
        Real ratio_multi_b = (m_ratio * m_ratio * m_ratio * m_ratio + 4.0f * m_ratio * m_ratio * m_ratio + 10.0f * m_ratio * m_ratio + 4.0f * m_ratio + 1.0f) / (ratio * ratio);
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
        Real it_xx = 0.15f * m_mass * b * b * ratio_multi_a + 0.0375f * m_mass * h * h * ratio_multi_b;
        Real it_zz = 0.15f * m_mass * a * a * ratio_multi_a + 0.0375f * m_mass * h * h * ratio_multi_b;
        Real it_yy = 0.15f * m_mass * (a * a + b * b) * ratio_multi_a;
        m_local_inertia_tensor.SetZero();
        m_local_inertia_tensor.SetDiagonal(it_xx, it_yy, it_zz);
        m_centroid = Vector3(0.0f, ((3.0f * m_ratio * m_ratio + 2.0f * m_ratio + 1.0f) * h / (4.0f * ratio)) - HalfHeight(), 0.0f);
    }

    Real ColliderTruncated::GetVolume()
    {
        Real ratio = m_ratio * m_ratio + m_ratio + 1.0f;
        if (m_collider_set != nullptr)
        {
            return Math::PI * m_scaled_radius.x * m_scaled_radius.y * m_scaled_height * ratio / 3.0f;
        }
        return Math::PI * m_radius.x * m_radius.y * m_height * ratio / 3.0f;
    }

    void ColliderTruncated::SetScaleData(const Vector3& scale)
    {
        Vector3 new_scale = scale.HadamardProduct(m_local.scale);
        m_scaled_height   = m_height * new_scale.y;
        m_scaled_radius   = m_radius.HadamardProduct(Vector2(new_scale.x, new_scale.z));
    }

    void ColliderTruncated::SetUnit()
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

    void ColliderTruncated::UpdateBoundingVolume()
    {
        Vector3 obb_vertices[8];
        Real    w = m_ratio > 1.0f ? m_scaled_radius.x * m_ratio : m_scaled_radius.x;
        Real    h = m_scaled_height * 0.5f;
        Real    d = m_ratio > 1.0f ? m_scaled_radius.y * m_ratio : m_scaled_radius.y;
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

    void ColliderTruncated::Draw(PrimitiveRenderer* renderer, eRenderingMode mode, const Color& color) const
    {
        I32 index       = static_cast<I32>(renderer->VerticesSize(mode));
        int stack_count = renderer->CYLINDRICAL_STACK_COUNT;
        int slice_count = renderer->CYLINDRICAL_SLICE_COUNT;
        renderer->ReserveVertices(renderer->CYLINDRICAL_VERTICES_COUNT, mode);
        Real       height         = Height();
        Vector2    radius         = Radius();
        Transform* body_transform = GetBodyTransform();
        Real       stack_height   = height / stack_count;
        Real       radius_step    = (m_ratio - 1.0f) / stack_count;
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

    Real ColliderTruncated::HalfHeight() const
    {
        if (m_collider_set != nullptr)
        {
            return m_scaled_height * 0.5f;
        }
        return m_height * 0.5f;
    }

    Real ColliderTruncated::Height() const
    {
        if (m_collider_set != nullptr)
        {
            return m_scaled_height;
        }
        return m_height;
    }

    Vector2 ColliderTruncated::Radius() const
    {
        if (m_collider_set != nullptr)
        {
            return m_scaled_radius;
        }
        return m_radius;
    }

    Real ColliderTruncated::Ratio() const
    {
        return m_ratio;
    }

    void ColliderTruncated::SetTruncated(Real height, const Vector2& radius, Real ratio)
    {
        m_height = height;
        m_radius = radius;
        m_ratio  = ratio;
        UpdatePrimitive();
    }

    void ColliderTruncated::SetHeight(Real height)
    {
        m_height = height;
        UpdatePrimitive();
    }

    void ColliderTruncated::SetRadius(const Vector2& radius)
    {
        m_radius = radius;
        UpdatePrimitive();
    }

    void ColliderTruncated::SetRatio(Real ratio)
    {
        m_ratio = ratio;
        UpdatePrimitive();
    }

    void ColliderTruncated::Clone(ColliderPrimitive* origin)
    {
        if (origin != this && origin != nullptr && origin->Type() == m_type)
        {
            ColliderTruncated* truncated = static_cast<ColliderTruncated*>(origin);
            //collider local space data
            m_local = truncated->m_local;
            //collider mass data
            m_centroid             = truncated->m_centroid;
            m_mass                 = truncated->m_mass;
            m_local_inertia_tensor = truncated->m_local_inertia_tensor;
            m_material             = truncated->m_material;
            //truncated data
            m_radius        = truncated->m_radius;
            m_height        = truncated->m_height;
            m_ratio         = truncated->m_ratio;
            m_scaled_radius = truncated->m_scaled_radius;
            m_scaled_height = truncated->m_scaled_height;
        }
    }

    void ColliderTruncated::Load(const Json::Value& data)
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
        if (JsonResource::HasMember(data, "Ratio") && data["Ratio"].isDouble())
        {
            m_ratio = data["Ratio"].asFloat();
        }
        SetTruncated(m_height, m_radius, m_ratio);
        LoadMaterial(data);
        LoadMass(data);
    }

    void ColliderTruncated::Save(const Json::Value& data)
    {
    }

    void ColliderTruncated::EditPrimitive(CommandRegistry* registry)
    {
    }
}
