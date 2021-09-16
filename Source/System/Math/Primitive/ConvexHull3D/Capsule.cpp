#include "Capsule.hpp"
#include "../../Utility/Utility.hpp"
#include "../../../Core/Utility/CoreDef.hpp"
#include "../../../Graphics/Utility/PrimitiveRenderer.hpp"

namespace CS460
{
    Capsule::Capsule()
    {
        type = ePrimitiveType::Capsule;
    }

    Capsule::~Capsule()
    {
    }

    void Capsule::Initialize()
    {
        SetUnit();
    }

    void Capsule::Shutdown()
    {
    }

    void Capsule::SetUnit()
    {
        Real length = radius * 2.0f + height;

        if(length > 0.0f)
        {
            radius /= length;
            height /= length;
        }
    }

    Vector3 Capsule::Support(const Vector3& direction)
    {
        Vector3 result = radius * direction;
        Vector3 axis_vector;
        axis_vector.y = height * 0.5f;
        Real top      = direction.DotProduct(axis_vector);
        Real bot      = direction.DotProduct(-axis_vector);
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

    bool Capsule::TestRayIntersection(const Ray& local_ray, Real& minimum_t, Real& maximum_t) const
    {
        Vector3 axis_vector;
        axis_vector.y     = height * 0.5f;
        Vector3 capsule_a = -axis_vector;
        Vector3 capsule_b = axis_vector;
        Vector3 ab; //capsule_b - capsule_a;
        ab.y              = height;
        Vector3 ao        = local_ray.position - capsule_a;
        Real    ab_dot_d  = ab.DotProduct(local_ray.direction);
        Real    ab_dot_ao = ab.DotProduct(ao);
        Real    ab_dot_ab = ab.DotProduct(ab);
        Real    m         = ab_dot_d / ab_dot_ab;
        Real    n         = ab_dot_ao / ab_dot_ab;

        // dot(Q, Q)*t^2 + 2*dot(Q, R)*t + (dot(R, R) - r^2) = 0
        // where
        // Q = d - AB * m
        // R = AO - AB * n
        Vector3 q = local_ray.direction - (ab * m);
        Vector3 r = ao - (ab * n);
        Real    a = q.DotProduct(q);
        Real    b = 2.0f * q.DotProduct(r);
        Real    c = r.DotProduct(r) - (radius * radius);
        minimum_t = -1.0f;
        maximum_t = -1.0f;
        if (a == 0.0f)
        {
            // Special case: AB and ray direction are parallel.
            // Q = d - AB * m =>
            // Q = d - AB * (|AB|*|d|*cos(AB,d) / |AB|^2) => |d| == 1.0
            // Q = d - AB * (|AB|*cos(AB,d)/|AB|^2) =>
            // Q = d - AB * cos(AB, d) / |AB| =>
            // Q = d - unit(AB) * cos(AB, d)
            //
            // |Q| == 0 means Q = (0, 0, 0) or d = unit(AB) * cos(AB,d)
            // both d and unit(AB) are unit vectors, so cos(AB, d) = 1 => AB and d are parallel.
            Real sphere_a_min_t, sphere_a_max_t, sphere_b_min_t, sphere_b_max_t;
            if (!this->TestRaySphere(local_ray, capsule_a, sphere_a_min_t, sphere_a_max_t) ||
                !this->TestRaySphere(local_ray, capsule_b, sphere_b_min_t, sphere_b_max_t))
            {
                return false;
            }
            if (sphere_a_min_t < sphere_b_min_t)
            {
                minimum_t = sphere_a_min_t;
            }
            else
            {
                minimum_t = sphere_b_min_t;
            }
            if (sphere_a_max_t > sphere_b_max_t)
            {
                maximum_t = sphere_a_max_t;
            }
            else
            {
                maximum_t = sphere_b_max_t;
            }
        }
        else
        {
            Real discriminant = b * b - 4.0f * a * c;
            if (discriminant < 0.0f)
            {
                return false;
            }
            Real cylinder_min_t = (-b - sqrtf(discriminant)) / (2.0f * a);
            Real cylinder_max_t = (-b + sqrtf(discriminant)) / (2.0f * a);
            if (cylinder_min_t > cylinder_max_t)
            {
                Real temp      = cylinder_min_t;
                cylinder_min_t = cylinder_max_t;
                cylinder_max_t = temp;
            }
            Real t_k1 = cylinder_min_t * m + n;
            if (t_k1 < 0.0f)
            {
                Real sphere_min_t, sphere_max_t;
                if (TestRaySphere(local_ray, capsule_a, sphere_min_t, sphere_max_t))
                {
                    minimum_t = sphere_min_t;
                }
                else
                {
                    return false;
                }
            }
            else if (t_k1 > 1.0f)
            {
                Real sphere_min_t, sphere_max_t;
                if (TestRaySphere(local_ray, capsule_b, sphere_min_t, sphere_max_t))
                {
                    minimum_t = sphere_min_t;
                }
                else
                {
                    return false;
                }
            }
            else
            {
                minimum_t = cylinder_min_t;
            }
            Real t_k2 = cylinder_max_t * m + n;
            if (t_k2 < 0.0f)
            {
                Real sphere_min_t, sphere_max_t;
                if (TestRaySphere(local_ray, capsule_a, sphere_min_t, sphere_max_t))
                {
                    maximum_t = sphere_max_t;
                }
                else
                {
                    return false;
                }
            }
            else if (t_k2 > 1.0f)
            {
                Real sphere_min_t, sphere_max_t;
                if (TestRaySphere(local_ray, capsule_b, sphere_min_t, sphere_max_t))
                {
                    maximum_t = sphere_max_t;
                }
                else
                {
                    return false;
                }
            }
            else
            {
                maximum_t = cylinder_max_t;
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

    Vector3 Capsule::GetNormal(const Vector3& local_point_on_primitive)
    {
        Real    half_height = height * 0.5f;
        Vector3 normal;
        if (local_point_on_primitive.y > half_height || local_point_on_primitive.y < -half_height)
        {
            //sphere case
            Vector3 sphere_origin;
            sphere_origin.y = half_height * Math::Signum(local_point_on_primitive.y);
            normal          = (local_point_on_primitive - sphere_origin).Normalize();
        }
        else
        {
            //disc case
            normal   = local_point_on_primitive;
            normal.y = 0.0f;
            normal.SetNormalize();
        }
        return normal;
    }

    void Capsule::DrawPrimitive(PrimitiveRenderer* renderer, eRenderingMode mode, const Color& color) const
    {
        I32 index       = static_cast<I32>(renderer->VerticesSize(mode));
        int stack_count = renderer->SPHERICAL_STACK_COUNT;
        int slice_count = renderer->SPHERICAL_SLICE_COUNT;
        renderer->ReserveVertices(renderer->SPHERICAL_VERTICES_COUNT, mode);
        Vector3 axis_vector = Math::Vector3::Y_AXIS;
        Real    half_height = HalfHeight();
        Vector3 radius      = Radius();

        //top vertex
        Vector3 top_vertex_local_pos = axis_vector;
        top_vertex_local_pos         = top_vertex_local_pos.HadamardProduct(radius);
        top_vertex_local_pos += half_height * axis_vector;

        //modify rotation, translation
        top_vertex_local_pos = orientation.Rotate(top_vertex_local_pos);
        top_vertex_local_pos += position;

        //bottom vertex
        Vector3 bottom_vertex_local_pos = -axis_vector;
        bottom_vertex_local_pos         = bottom_vertex_local_pos.HadamardProduct(radius);
        bottom_vertex_local_pos -= half_height * axis_vector;

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
                vertex_local_pos   = vertex_local_pos.HadamardProduct(radius);
                if (i < 8)
                {
                    vertex_local_pos += half_height * axis_vector;
                }
                else
                {
                    vertex_local_pos -= half_height * axis_vector;
                }
                vertex_local_pos = orientation.Rotate(vertex_local_pos);
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

    Real Capsule::HalfHeight() const
    {
        return 0.5f * height;
    }

    Vector3 Capsule::Radius() const
    {
        return Vector3(radius, radius, radius);
    }


    bool Capsule::TestRaySphere(const Ray& ray, const Vector3& centroid, Real& min_t, Real& max_t) const
    {
        Vector3 co           = ray.position - centroid;
        Real    a            = ray.direction.DotProduct(ray.direction);
        Real    b            = 2.0f * co.DotProduct(ray.direction);
        Real    c            = co.DotProduct(co) - radius * radius;
        Real    discriminant = b * b - 4.0f * a * c;
        if (discriminant < 0.0f)
        {
            return false;
        }
        min_t = (-b - sqrtf(discriminant)) / (2.0f * a);
        max_t = (-b + sqrtf(discriminant)) / (2.0f * a);
        if (min_t > max_t)
        {
            Real temp = min_t;
            min_t     = max_t;
            max_t     = temp;
        }
        return true;
    }
}
