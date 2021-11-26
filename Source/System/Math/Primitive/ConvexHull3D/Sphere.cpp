#include "Sphere.hpp"
#include "../../Utility/Utility.hpp"
#include "../../../Core/Utility/CoreDef.hpp"
#include "../../../Graphics/Utility/PrimitiveRenderer.hpp"

namespace CS460
{
    Sphere::Sphere()
    {
        type = ePrimitiveType::Sphere;
    }

    Sphere::Sphere(const Vector3& position, const Quaternion& orientation, Real radius)
        : Primitive(position, orientation), radius(radius)
    {
        type = ePrimitiveType::Sphere;
    }

    Sphere::~Sphere()
    {
    }

    void Sphere::Initialize()
    {
        SetUnit();
    }

    void Sphere::Shutdown()
    {
    }

    void Sphere::SetUnit()
    {
        radius = 0.5f;
    }

    Vector3 Sphere::Support(const Vector3& direction)
    {
        return radius * direction;
    }

    bool Sphere::TestRayIntersection(const Ray& local_ray, Real& minimum_t, Real& maximum_t) const
    {
        //Vector3 l  = -local_ray.position;
        //Real    tc = l.DotProduct(local_ray.direction);
        //if (tc < 0.0f)
        //{
        //    return false;
        //}
        //Real d2             = (tc * tc) - (l.DotProduct(l));
        //Real radius_squared = radius * radius;
        //if (d2 > radius_squared)
        //{
        //    return false;
        //}

        ////solve for t1c
        //Real t1c  = sqrtf(radius_squared - d2);
        //minimum_t = tc - t1c;
        //maximum_t = tc + t1c;
        //return true;

        Real a = (local_ray.direction.x * local_ray.direction.x / (radius * radius))
                + (local_ray.direction.y * local_ray.direction.y / (radius * radius))
                + (local_ray.direction.z * local_ray.direction.z / (radius * radius));
        Real b = (2.0f * local_ray.direction.x * local_ray.position.x / (radius * radius))
                + (2.0f * local_ray.direction.y * local_ray.position.y / (radius * radius))
                + (2.0f * local_ray.direction.z * local_ray.position.z / (radius * radius));
        Real c = (local_ray.position.x * local_ray.position.x / (radius * radius))
                + (local_ray.position.y * local_ray.position.y / (radius * radius))
                + (local_ray.position.z * local_ray.position.z / (radius * radius)) - 1.0f;
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

    Vector3 Sphere::GetNormal(const Vector3& local_point_on_primitive)
    {
        return local_point_on_primitive.Normalize();
    }

    void Sphere::DrawPrimitive(PrimitiveRenderer* renderer, eRenderingMode mode, const Color& color) const
    {
        I32 index       = static_cast<I32>(renderer->VerticesSize(mode));
        int stack_count = renderer->SPHERICAL_STACK_COUNT;
        int slice_count = renderer->SPHERICAL_SLICE_COUNT;
        renderer->ReserveVertices(renderer->SPHERICAL_VERTICES_COUNT, mode);
        Vector3 axis_vector = Math::Vector3::Y_AXIS;

        //top vertex
        Vector3 top_vertex_local_pos = axis_vector;
        top_vertex_local_pos         = top_vertex_local_pos * radius;

        //modify rotation, translation
        top_vertex_local_pos = orientation.Rotate(top_vertex_local_pos);
        top_vertex_local_pos += position;

        //bottom vertex
        Vector3 bottom_vertex_local_pos = -axis_vector;
        bottom_vertex_local_pos         = bottom_vertex_local_pos * radius;

        //modify rotation, translation
        bottom_vertex_local_pos = orientation.Rotate(bottom_vertex_local_pos);
        bottom_vertex_local_pos += position;
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
                vertex_local_pos   = orientation.Rotate(vertex_local_pos);
                vertex_local_pos += position;
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

    bool Sphere::HasIntersection(const Sphere& sphere) const
    {
        Real sq_d = position.DistanceSquaredTo(sphere.position);
        return sq_d <= (radius + sphere.radius) * (radius + sphere.radius);
    }
}
