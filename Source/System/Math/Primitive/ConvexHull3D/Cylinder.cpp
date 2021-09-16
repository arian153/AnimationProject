#include "Cylinder.hpp"
#include "../../Utility/Utility.hpp"
#include "../../../Core/Utility/CoreDef.hpp"
#include "../../../Graphics/Utility/PrimitiveRenderer.hpp"

namespace CS460
{
    Cylinder::Cylinder()
    {
        type = ePrimitiveType::Cylinder;
    }

    Cylinder::~Cylinder()
    {
    }

    void Cylinder::Initialize()
    {
    }

    void Cylinder::Shutdown()
    {
    }

    void Cylinder::SetUnit()
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

    Vector3 Cylinder::Support(const Vector3& direction)
    {
        Vector3 axis_vector(0.0f, height * 0.5f, 0.0f);
        Vector3 disc_vector = direction;
        disc_vector.y       = 0.0f;
        disc_vector.SetNormalize();
        disc_vector *= radius;
        Real    top_support = direction.DotProduct(disc_vector + axis_vector);
        Real    bot_support = direction.DotProduct(disc_vector - axis_vector);
        Vector3 result;
        if (top_support > bot_support)
        {
            result = disc_vector + axis_vector;
        }
        else
        {
            result = disc_vector - axis_vector;
        }
        return result;
    }

    bool Cylinder::TestRayIntersection(const Ray& local_ray, Real& minimum_t, Real& maximum_t) const
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
        Real a                     = none_axis_direction_x * none_axis_direction_x + none_axis_direction_z * none_axis_direction_z;
        Real b                     = 2.0f * (none_axis_direction_x * none_axis_position_x + none_axis_direction_z * none_axis_position_z);
        Real c                     = (none_axis_position_x * none_axis_position_x + none_axis_position_z * none_axis_position_z) - (radius * radius);
        Real cylinder_min_t, cylinder_max_t;
        if (Math::SolveQuadratic(a, b, c, cylinder_max_t, cylinder_min_t) == true)
        {
            if (cylinder_min_t > cylinder_max_t)
            {
                Real temp      = cylinder_min_t;
                cylinder_min_t = cylinder_max_t;
                cylinder_max_t = temp;
            }
            Real min_axis_height = local_ray.position.y + local_ray.direction.y * cylinder_min_t;
            Real max_axis_height = local_ray.position.y + local_ray.direction.y * cylinder_max_t;
            minimum_t            = cylinder_min_t;
            maximum_t            = cylinder_max_t;
            if (min_axis_height > half_height)
            {
                if (max_axis_height > half_height)
                {
                    return false;
                }
                Real disc_t = (half_height - axis_position) / axis_direction;
                Real disc_x = none_axis_direction_x * disc_t + none_axis_position_x;
                Real disc_z = none_axis_direction_z * disc_t + none_axis_position_z;
                if ((disc_x * disc_x + disc_z * disc_z) <= radius * radius)
                {
                    minimum_t = disc_t;
                }
                else
                {
                    //return result;
                }
            }
            if (max_axis_height < -half_height)
            {
                if (min_axis_height < -half_height)
                {
                    return false;
                }
                Real disc_t = (half_height - axis_position) / axis_direction;
                Real disc_x = none_axis_direction_x * disc_t + none_axis_position_x;
                Real disc_z = none_axis_direction_z * disc_t + none_axis_position_z;
                if ((disc_x * disc_x + disc_z * disc_z) <= radius * radius)
                {
                    minimum_t = disc_t;
                }
                else
                {
                }
            }
            if (max_axis_height > half_height)
            {
                Real disc_t = (half_height - axis_position) / axis_direction;
                Real disc_x = none_axis_direction_x * disc_t + none_axis_position_x;
                Real disc_z = none_axis_direction_z * disc_t + none_axis_position_z;
                if ((disc_x * disc_x + disc_z * disc_z) <= radius * radius)
                {
                    //minimum_t = cylinder_min_t;
                    maximum_t = disc_t;
                }
                else
                {
                    return false;
                }
            }
            if (min_axis_height < -half_height)
            {
                Real disc_t = (-half_height - axis_position) / axis_direction;
                Real disc_x = none_axis_direction_x * disc_t + none_axis_position_x;
                Real disc_z = none_axis_direction_z * disc_t + none_axis_position_z;
                if ((disc_x * disc_x + disc_z * disc_z) <= radius * radius)
                {
                    minimum_t = disc_t;
                    //maximum_t = cylinder_max_t;
                }
                else
                {
                    return false;
                }
            }
        }
        else
        {
            Real disc_t = (-half_height - axis_position) / axis_direction;
            Real disc_x = none_axis_direction_x * disc_t + none_axis_position_x;
            Real disc_z = none_axis_direction_z * disc_t + none_axis_position_z;
            if ((disc_x * disc_x + disc_z * disc_z) <= radius * radius)
            {
                minimum_t = disc_t;
                maximum_t = (half_height - axis_position) / axis_direction;
                if (minimum_t > maximum_t)
                {
                    Real temp = minimum_t;
                    minimum_t = maximum_t;
                    maximum_t = temp;
                }
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

    Vector3 Cylinder::GetNormal(const Vector3& local_point_on_primitive)
    {
        Real half_height    = height * 0.5f;
        Real point_height   = local_point_on_primitive.y;
        Real radius_squared = radius * radius;
        Real point_radius   = (local_point_on_primitive.x * local_point_on_primitive.x)
                + (local_point_on_primitive.z * local_point_on_primitive.z);
        Vector3 normal;
        if ((Math::IsEqual(point_height, half_height) || Math::IsEqual(point_height, -half_height))
            && radius_squared > point_radius)
        {
            // point on top disc or bottom disc 
            // also point in disc.
            normal.y = Math::Signum(point_height);
        }
        else
        {
            // point on cylinder disc.
            normal   = local_point_on_primitive;
            normal.y = 0.0f;
            normal.SetNormalize();
        }
        return normal;
    }

    void Cylinder::DrawPrimitive(PrimitiveRenderer* renderer, eRenderingMode mode, const Color& color) const
    {
        I32 index       = static_cast<I32>(renderer->VerticesSize(mode));
        int stack_count = renderer->CYLINDRICAL_STACK_COUNT;
        int slice_count = renderer->CYLINDRICAL_SLICE_COUNT;
        renderer->ReserveVertices(renderer->CYLINDRICAL_VERTICES_COUNT, mode);
        Vector2 radius       = Radius();
        Real    stack_height = height / stack_count;
        I32     ring_count   = stack_count + 1;
        for (I32 i = 0; i < ring_count; ++i)
        {
            Real y       = -0.5f * height + i * stack_height;
            Real d_theta = Math::TWO_PI / slice_count;
            for (I32 j = 0; j <= slice_count; ++j)
            {
                Real    c = cosf(j * d_theta);
                Real    s = sinf(j * d_theta);
                Vector3 vertex_local_pos;
                vertex_local_pos.x = radius.x * c;
                vertex_local_pos.y = y;
                vertex_local_pos.z = radius.y * s;
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
        }
    }

    Vector2 Cylinder::Radius() const
    {
        return Vector2(radius, radius);
    }
}
