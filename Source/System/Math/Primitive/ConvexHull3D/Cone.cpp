#include"Cone.hpp"
#include "../../Utility/Utility.hpp"
#include "../../../Core/Utility/CoreDef.hpp"
#include "../../../Graphics/Utility/PrimitiveRenderer.hpp"

namespace CS460
{
    Cone::Cone()
    {
        type = ePrimitiveType::Cone;
    }

    Cone::Cone(const Vector3& position, const Quaternion& orientation, Real radius, Real height)
        : Primitive(position, orientation), radius(radius), height(height)
    {
        type = ePrimitiveType::Cone;
    }

    Cone::~Cone()
    {
    }

    void Cone::Initialize()
    {
    }

    void Cone::Shutdown()
    {
    }

    void Cone::SetUnit()
    {
        Real division;
        if (radius * 2.0f > height)
        {
            division = radius * 2.0f;
        }
        else
        {
            division = height;
        }

        if (division > 0.0f)
        {
            radius /= division;
            height /= division;
        }
    }

    Vector3 Cone::Support(const Vector3& direction)
    {
        Vector3 axis_vector;
        axis_vector.y       = height * 0.5f;
        axis_vector.z       = 0.0f;
        axis_vector.x       = 0.0f;
        Vector3 disc_vector = direction;
        disc_vector.y       = 0.0f;
        disc_vector.SetNormalize();
        disc_vector *= radius;
        Real    point_support = direction.DotProduct(axis_vector);
        Real    disc_support  = direction.DotProduct(disc_vector);
        Vector3 result;
        if (point_support > disc_support)
        {
            result = axis_vector;
        }
        else
        {
            result = disc_vector - axis_vector;
        }
        return result;
    }

    bool Cone::TestRayIntersection(const Ray& local_ray, Real& minimum_t, Real& maximum_t) const
    {
        Real half_height           = height * 0.5f;
        minimum_t                  = -1.0f;
        maximum_t                  = -1.0f;
        Real axis_direction        = local_ray.direction.y;
        Real none_axis_direction_x = local_ray.direction.x;
        Real none_axis_direction_z = local_ray.direction.z;
        Real axis_position         = local_ray.position.y;
        Real none_axis_position_x  = local_ray.position.x;
        Real none_axis_position_z  = local_ray.position.z;
        Real a                     = (radius * radius * axis_direction * axis_direction / (height * height))
                - (none_axis_direction_x * none_axis_direction_x + none_axis_direction_z * none_axis_direction_z);
        Real b = (2.0f * radius * radius * axis_position * axis_direction / (height * height))
                - (radius * radius * axis_direction / height)
                - (2.0f * (none_axis_position_x * none_axis_direction_x + none_axis_position_z * none_axis_direction_z));
        Real c = (radius * radius * axis_position * axis_position / (height * height))
                - (radius * radius * axis_position / height)
                + (radius * radius / 4.0f)
                - (none_axis_position_x * none_axis_position_x + none_axis_position_z * none_axis_position_z);
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
                maximum_t = (-half_height - axis_position) / axis_direction;
            }
            else
            {
                if (min_axis_height < -half_height)
                {
                    //test_disc
                    Real disc_t = (-half_height - axis_position) / axis_direction;
                    Real disc_x = none_axis_direction_x * disc_t + none_axis_position_x;
                    Real disc_z = none_axis_direction_z * disc_t + none_axis_position_z;
                    if ((disc_x * disc_x + disc_z * disc_z) <= radius * radius)
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
                    Real disc_t = (-half_height - axis_position) / axis_direction;
                    Real disc_x = none_axis_direction_x * disc_t + none_axis_position_x;
                    Real disc_z = none_axis_direction_z * disc_t + none_axis_position_z;
                    if ((disc_x * disc_x + disc_z * disc_z) <= radius * radius)
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
                        minimum_t = (-half_height - axis_position) / axis_direction;
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
                Real disc_t = (-half_height - axis_position) / axis_direction;
                Real disc_x = none_axis_direction_x * disc_t + none_axis_position_x;
                Real disc_z = none_axis_direction_z * disc_t + none_axis_position_z;
                if ((disc_x * disc_x + disc_z * disc_z) <= radius * radius)
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

    Vector3 Cone::GetNormal(const Vector3& local_point_on_primitive)
    {
        Real    half_height    = height * 0.5f;
        Real    point_height   = local_point_on_primitive.y;
        Real    radius_squared = radius * radius;
        Real    point_radius   = (local_point_on_primitive.x * local_point_on_primitive.x) + (local_point_on_primitive.z * local_point_on_primitive.z);
        Vector3 normal;
        if (Math::IsEqual(point_height, half_height)
            || (Math::IsEqual(point_height, -half_height)
                && radius_squared > point_radius))
        {
            // point on top point or bottom disc 
            normal.y = Math::Signum(point_height);
        }
        else if (Math::IsEqual(point_height, -half_height)
            && Math::IsEqual(radius_squared, point_radius))
        {
            // point on cylinder disc.
            normal   = local_point_on_primitive;
            normal.y = 0.0f;
            normal.SetNormalize();
        }
        else
        {
            Vector3 point_vec  = local_point_on_primitive;
            point_vec.y        = 0.0f;
            Vector3 radius_vec = point_vec.Unit() * radius;
            Real    t          = radius * radius / (height * height);
            normal             = radius_vec;
            normal.y           = height * t;
            normal.SetNormalize();
        }
        return normal;
    }

    void Cone::DrawPrimitive(PrimitiveRenderer* renderer, eRenderingMode mode, const Color& color) const
    {
        I32 index       = static_cast<I32>(renderer->VerticesSize(mode));
        int stack_count = renderer->CYLINDRICAL_STACK_COUNT;
        int slice_count = renderer->CYLINDRICAL_SLICE_COUNT;
        renderer->ReserveVertices(renderer->CYLINDRICAL_VERTICES_COUNT, mode);
        Vector2 radius       = Radius();
        Real    stack_height = height / stack_count;
        Real    radius_step  = -1.0f / stack_count;
        I32     ring_count   = stack_count + 1;
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
                vertex_local_pos   = orientation.Rotate(vertex_local_pos);
                vertex_local_pos += position;
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
            Vector3 vertex_center_pos;
            vertex_center_pos.x = 0.0f;
            vertex_center_pos.y = -0.5f * height;
            vertex_center_pos.z = 0.0f;
            vertex_center_pos   = orientation.Rotate(vertex_center_pos);
            vertex_center_pos += position;
            I32 center_index = static_cast<I32>(renderer->VerticesSize(mode));
            renderer->PushVertex(vertex_center_pos, mode, color);

            for (I32 i = 0; i < slice_count; ++i)
            {
                renderer->PushFaceIndices(center_index, index + i, index + i + 1);
            }
        }
    }

    Vector2 Cone::Radius() const
    {
        return Vector2(radius, radius);
    }
}
