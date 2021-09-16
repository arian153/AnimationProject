#include "Triangle.hpp"
#include "../../Utility/Utility.hpp"
#include "../../../Core/Utility/CoreDef.hpp"
#include "../../../Graphics/Utility/PrimitiveRenderer.hpp"

namespace CS460
{
    Triangle::Triangle()
    {
        type = ePrimitiveType::Triangle;
    }

    Triangle::Triangle(const Vector2& p0, const Vector2& p1, const Vector2& p2)
    {
        type = ePrimitiveType::Triangle;
        SetTriangle(p0, p1, p2);
    }

    Triangle::Triangle(const Vector3& p0, const Vector3& p1, const Vector3& p2)
    {
        type = ePrimitiveType::Triangle;
        SetTriangle(p0, p1, p2);
    }

    Triangle::~Triangle()
    {
    }

    void Triangle::Initialize()
    {
    }

    void Triangle::Shutdown()
    {
    }

    void Triangle::SetUnit()
    {
        vertices[0] = Vector2(0.0f, 0.0f);
        vertices[1] = Vector2(1.0f, 0.0f);
        vertices[2] = Vector2(0.0f, 1.0f);
    }

    Vector3 Triangle::Support(const Vector3& direction)
    {
        Vector2 sub_space_direction;
        sub_space_direction.x = direction.x;
        sub_space_direction.y = direction.y;
        sub_space_direction.SetNormalize();
        Vector2 result;
        Real    p = Math::REAL_NEGATIVE_MAX;
        for (size_t i = 0; i < 3; ++i)
        {
            Real projection = vertices[i].DotProduct(sub_space_direction);
            if (projection > p)
            {
                result = vertices[i];
                p      = projection;
            }
        }
        return Vector3(result.x, result.y, 0.0f);
    }

    bool Triangle::TestRayIntersection(const Ray& local_ray, Real& minimum_t, Real& maximum_t) const
    {
        minimum_t = -1.0f;
        maximum_t = -1.0f;
        //Quadratic elements
        Real triangle_min_t = Math::REAL_POSITIVE_MAX;
        Real triangle_max_t = Math::REAL_NEGATIVE_MAX;

        //plane elements
        Vector3 normal(0.0f, 0.0f, 1.0f);
        Vector3 pc          = -local_ray.position;
        Real    denominator = normal.DotProduct(local_ray.direction);
        if (Math::IsZero(denominator) == true)
        {
            //ray is parallel to plane.
            if (Math::IsZero(pc.DotProduct(normal)) == true)
            {
                //ray is on the plane.
                Vector2 dir(local_ray.direction.x, local_ray.direction.y);
                Vector2 pos(local_ray.position.x, local_ray.position.y);
                //last edge
                Vector2 p0      = vertices[0];
                Vector2 p1      = vertices[1];
                Vector2 p2      = vertices[2];
                Vector2 edge1   = p1 - p0;
                Vector2 edge2   = p2 - p1;
                Vector2 edge3   = p0 - p2;
                Real    inv_dir = 1.0f / dir.DotProduct(dir);
                if (Math::IsZero(dir.CrossProduct(edge1)) == true)
                {
                    if (Math::IsZero((p0 - pos).CrossProduct(dir)) == true)
                    {
                        triangle_min_t = (p0 - pos).DotProduct(dir) * inv_dir;
                        triangle_max_t = (p1 - pos).DotProduct(dir) * inv_dir;
                    }
                }
                else
                {
                    Real t = (p0 - pos).CrossProduct(edge1) / (dir.CrossProduct(edge1));
                    if (triangle_min_t > t)
                    {
                        triangle_min_t = t;
                    }
                    if (triangle_max_t < t)
                    {
                        triangle_max_t = t;
                    }
                }
                if (Math::IsZero(dir.CrossProduct(edge2)) == true)
                {
                    if (Math::IsZero((p1 - pos).CrossProduct(dir)) == true)
                    {
                        triangle_min_t = (p1 - pos).DotProduct(dir) * inv_dir;
                        triangle_max_t = (p2 - pos).DotProduct(dir) * inv_dir;
                    }
                }
                else
                {
                    Real t = (p1 - pos).CrossProduct(edge2) / (dir.CrossProduct(edge2));
                    if (triangle_min_t > t)
                    {
                        triangle_min_t = t;
                    }
                    if (triangle_max_t < t)
                    {
                        triangle_max_t = t;
                    }
                }
                if (Math::IsZero(dir.CrossProduct(edge3)) == true)
                {
                    if (Math::IsZero((p2 - pos).CrossProduct(dir)) == true)
                    {
                        triangle_min_t = (p2 - pos).DotProduct(dir) * inv_dir;
                        triangle_max_t = (p0 - pos).DotProduct(dir) * inv_dir;
                    }
                }
                else
                {
                    Real t = (p2 - pos).CrossProduct(edge3) / (dir.CrossProduct(edge3));
                    if (triangle_min_t > t)
                    {
                        triangle_min_t = t;
                    }
                    if (triangle_max_t < t)
                    {
                        triangle_max_t = t;
                    }
                }
            }
            else
            {
                return false;
            }
            minimum_t = triangle_min_t;
            maximum_t = triangle_max_t;
        }
        else
        {
            //ray-plane intersect one point.
            Real    plane_t            = pc.DotProduct(normal) / denominator;
            Vector3 plane_intersection = local_ray.position + local_ray.direction * plane_t;
            //define ellipse.
            Vector2 v2edge1 = vertices[1] - vertices[0];
            Vector2 v2edge2 = vertices[2] - vertices[0];
            Vector3 edge1(v2edge1.x, v2edge1.y);
            Vector3 edge2(v2edge2.x, v2edge2.y);
            Vector3 h = local_ray.direction.CrossProduct(edge2);
            Real    a = edge1.DotProduct(h);
            if (Math::IsZero(a))
            {
                return false;
            }
            Real    f = 1.0f / a;
            Vector3 s = local_ray.position - Vector3(vertices[0].x, vertices[1].y);
            Real    u = f * (s.DotProduct(h));
            if (u < 0.0f || u > 1.0f)
            {
                return false;
            }
            Vector3 q = s.CrossProduct(edge1);
            Real    v = f * local_ray.direction.DotProduct(q);
            if (v < 0.0f || u + v > 1.0f)
            {
                return false;
            }

            // At this stage we can compute t to find out where the intersection point is on the line.
            triangle_min_t = f * edge2.DotProduct(q);
            if (triangle_min_t > Math::EPSILON) // ray intersection
            {
                minimum_t = maximum_t = triangle_min_t;
            }
            else // This means that there is a line intersection but not a ray intersection.
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

    Vector3 Triangle::GetNormal(const Vector3& local_point_on_primitive)
    {
        return local_point_on_primitive;
    }

    void Triangle::DrawPrimitive(PrimitiveRenderer* renderer, eRenderingMode mode, const Color& color) const
    {
        I32 index = static_cast<I32>(renderer->VerticesSize(mode));
        I32 count = 3;
        renderer->ReserveVertices(count, mode);
        for (size_t i = 0; i < 3; ++i)
        {
            //local space to world space
            Vector3 vertex(vertices[i]);
            vertex = orientation.Rotate(vertex);
            vertex += position;

            //push to renderer
            renderer->PushVertex(vertex, mode, color);
        }
        if (mode == eRenderingMode::Dot)
        {
            for (I32 i = 0; i < count; ++i)
            {
                renderer->PushIndex(index + i, mode);
            }
        }
        else if (mode == eRenderingMode::Line)
        {
            for (int i = 0; i < count; ++i)
            {
                I32  j = i + 1 < count ? i + 1 : 0;
                renderer->PushLineIndices(index + i, index + j);
            }
        }
        else if (mode == eRenderingMode::Face)
        {
            renderer->PushFaceIndices(index + 0, index + 2, index + 1);
        }
    }

    void Triangle::SetTriangle(const Vector2& p0, const Vector2& p1, const Vector2& p2)
    {
        position    = (p0 + p1 + p2) / 3.0f;
        auto c      = position.GrepVec2(Math::Vector::X, Math::Vector::Y);
        vertices[0] = p0 - c;
        vertices[1] = p1 - c;
        vertices[2] = p2 - c;
    }

    void Triangle::SetTriangle(const Vector3& p0, const Vector3& p1, const Vector3& p2)
    {
        Vector3    ab     = p1 - p0;
        Vector3    ac     = p2 - p0;
        Vector3    normal = ab.CrossProduct(ac).Normalize();
        Quaternion rotation(normal, Math::Vector3::Z_AXIS);
        position    = (p0 + p1 + p2) / 3.0f;
        auto v0     = p0 - position;
        auto v1     = p1 - position;
        auto v2     = p2 - position;
        vertices[0] = rotation.Rotate(v0);
        vertices[1] = rotation.Rotate(v1);
        vertices[2] = rotation.Rotate(v2);
        orientation = rotation.Inverse();
    }

    Vector3 Triangle::Vertex(size_t i) const
    {
        return orientation.Rotate(Vector3(vertices[i])) + position;
    }

    Vector3 Triangle::ClosestPoint(const Vector3& point) const
    {
        Vector3 a = Vertex(0);
        Vector3 b = Vertex(1);
        Vector3 c = Vertex(2);

        // Check if P in vertex region outside A
        Vector3 ab = b - a;
        Vector3 ac = c - a;
        Vector3 ap = point - a;
        Real    d1 = DotProduct(ab, ap);
        Real    d2 = DotProduct(ac, ap);
        if (d1 <= 0.0f && d2 <= 0.0f)
        {
            // barycentric coordinates (1,0,0)
            return a;
        }

        // Check if P in vertex region outside B
        Vector3 bp = point - b;
        Real    d3 = DotProduct(ab, bp);
        Real    d4 = DotProduct(ac, bp);
        if (d3 >= 0.0f && d4 <= d3)
        {
            // barycentric coordinates (0,1,0)
            return b;
        }

        // Check if P in edge region of AB, if so return projection of P onto AB
        Real vc = d1 * d4 - d3 * d2;
        if (vc <= 0.0f && d1 >= 0.0f && d3 <= 0.0f)
        {
            Real v = d1 / (d1 - d3);
            // barycentric coordinates (1-v,v,0)
            return a + v * ab;
        }

        // Check if P in vertex region outside C
        Vector3 cp = point - c;
        Real    d5 = DotProduct(ab, cp);
        Real    d6 = DotProduct(ac, cp);
        if (d6 >= 0.0f && d5 <= d6)
        {
            // barycentric coordinates (0,0,1)
            return c;
        }

        // Check if P in edge region of AC, if so return projection of P onto AC
        Real vb = d5 * d2 - d1 * d6;
        if (vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f)
        {
            Real w = d2 / (d2 - d6);
            // barycentric coordinates (1-w,0,w)
            return a + w * ac;
        }

        // Check if P in edge region of BC, if so return projection of P onto BC
        Real va = d3 * d6 - d5 * d4;
        if (va <= 0.0f && (d4 - d3) >= 0.0f && (d5 - d6) >= 0.0f)
        {
            Real w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
            // barycentric coordinates (0,1-w,w)
            return b + w * (c - b);
        }

        // P inside face region. Compute Q through its barycentric coordinates (u,v,w)
        Real denom = 1.0f / (va + vb + vc);
        Real v     = vb * denom;
        Real w     = vc * denom;
        // = u*a + v*b + w*c, u = va * denom = 1.0f - v - w
        return a + ab * v + ac * w;
    }

    Real Triangle::Distance(const Vector3& point) const
    {
        return sqrtf(DistanceSquared(point));
    }

    Real Triangle::DistanceSquared(const Vector3& point) const
    {
        return (point - ClosestPoint(point)).LengthSquared();
    }

    Vector3 Triangle::ClosestPoint(const Vector3& point, const Vector3& p0, const Vector3& p1, const Vector3& p2)
    {
        Vector3 a(p0), b(p1), c(p2);
        
        // Check if P in vertex region outside A
        Vector3 ab = b - a;
        Vector3 ac = c - a;
        Vector3 ap = point - a;
        Real    d1 = DotProduct(ab, ap);
        Real    d2 = DotProduct(ac, ap);
        if (d1 <= 0.0f && d2 <= 0.0f)
        {
            // barycentric coordinates (1,0,0)
            return a;
        }

        // Check if P in vertex region outside B
        Vector3 bp = point - b;
        Real    d3 = DotProduct(ab, bp);
        Real    d4 = DotProduct(ac, bp);
        if (d3 >= 0.0f && d4 <= d3)
        {
            // barycentric coordinates (0,1,0)
            return b;
        }

        // Check if P in edge region of AB, if so return projection of P onto AB
        Real vc = d1 * d4 - d3 * d2;
        if (vc <= 0.0f && d1 >= 0.0f && d3 <= 0.0f)
        {
            Real v = d1 / (d1 - d3);
            // barycentric coordinates (1-v,v,0)
            return a + v * ab;
        }

        // Check if P in vertex region outside C
        Vector3 cp = point - c;
        Real    d5 = DotProduct(ab, cp);
        Real    d6 = DotProduct(ac, cp);
        if (d6 >= 0.0f && d5 <= d6)
        {
            // barycentric coordinates (0,0,1)
            return c;
        }

        // Check if P in edge region of AC, if so return projection of P onto AC
        Real vb = d5 * d2 - d1 * d6;
        if (vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f)
        {
            Real w = d2 / (d2 - d6);
            // barycentric coordinates (1-w,0,w)
            return a + w * ac;
        }

        // Check if P in edge region of BC, if so return projection of P onto BC
        Real va = d3 * d6 - d5 * d4;
        if (va <= 0.0f && (d4 - d3) >= 0.0f && (d5 - d6) >= 0.0f)
        {
            Real w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
            // barycentric coordinates (0,1-w,w)
            return b + w * (c - b);
        }

        // P inside face region. Compute Q through its barycentric coordinates (u,v,w)
        Real denom = 1.0f / (va + vb + vc);
        Real v     = vb * denom;
        Real w     = vc * denom;
        // = u*a + v*b + w*c, u = va * denom = 1.0f - v - w
        return a + ab * v + ac * w;
    }

    Real Triangle::DistanceSquared(const Vector3& point, const Vector3& p0, const Vector3& p1, const Vector3& p2)
    {
        return (point - ClosestPoint(point, p0, p1, p2)).LengthSquared();
    }

    Real Triangle::Distance(const Vector3& point, const Vector3& p0, const Vector3& p1, const Vector3& p2)
    {
        return sqrtf(DistanceSquared(point, p0, p1, p2));
    }

    bool Triangle::IsContainPoint(const Vector3& point, const Vector3& p0, const Vector3& p1, const Vector3& p2)
    {
        Vector3 edge01 = p1 - p0;
        Vector3 edge12 = p2 - p1;
        Vector3 normal = edge01.CrossProduct(edge12);
        Vector3 w_test = edge01.CrossProduct(point - p0);
        if (w_test.DotProduct(normal) < 0.0f)
        {
            return false;
        }
        w_test = edge12.CrossProduct(point - p1);
        if (w_test.DotProduct(normal) < 0.0f)
        {
            return false;
        }
        Vector3 edge3 = p0 - p2;
        w_test        = edge3.CrossProduct(point - p2);
        if (w_test.DotProduct(normal) < 0.0f)
        {
            return false;
        }
        normal.SetNormalize();
        return true;
    }

    Vector3 Triangle::Normal(const Vector3& p0, const Vector3& p1, const Vector3& p2)
    {
        Vector3 edge01 = p1 - p0;
        Vector3 edge02 = p2 - p0;
        return edge01.CrossProduct(edge02);
    }
}
