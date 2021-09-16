#include "Polygon.hpp"
#include "../../Utility/Utility.hpp"
#include "../../../Core/Utility/CoreDef.hpp"
#include "../../../Graphics/Utility/PrimitiveRenderer.hpp"

namespace CS460
{
    Polygon::Polygon()
    {
        type = ePrimitiveType::Polygon;
    }

    Polygon::~Polygon()
    {
    }

    void Polygon::Initialize()
    {
    }

    void Polygon::Shutdown()
    {
    }

    void Polygon::SetUnit()
    {
    }

    Vector3 Polygon::Support(const Vector3& direction)
    {
        Vector2 sub_space_direction;
        sub_space_direction.x = direction.x;
        sub_space_direction.y = direction.y;
        sub_space_direction.SetNormalize();
        Vector2 result;
        Real    p = Math::REAL_NEGATIVE_MAX;
        for (auto& vertex : *vertices)
        {
            Real projection = vertex.DotProduct(sub_space_direction);
            if (projection > p)
            {
                result = vertex;
                p      = projection;
            }
        }
        return Vector3(result.x, result.y, 0.0f);
    }

    bool Polygon::TestRayIntersection(const Ray& local_ray, Real& minimum_t, Real& maximum_t) const
    {
        minimum_t = -1.0f;
        maximum_t = -1.0f;
        //Quadratic elements
        Real polygon_min_t = Math::REAL_POSITIVE_MAX;
        Real polygon_max_t = Math::REAL_NEGATIVE_MAX;
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
                Real    inv_dir = 1.0f / dir.DotProduct(dir);
                size_t  size    = vertices->size();
                for (size_t i = 0; i < size; ++i)
                {
                    size_t  j    = i + 1 < size ? i + 1 : 0;
                    Vector2 p0   = vertices->at(i);
                    Vector2 p1   = vertices->at(j);
                    Vector2 edge = p1 - p0;
                    if (Math::IsZero(dir.CrossProduct(edge)) == true)
                    {
                        if (Math::IsZero((p0 - pos).CrossProduct(dir)) == true)
                        {
                            polygon_min_t = (p0 - pos).DotProduct(dir) * inv_dir;
                            polygon_max_t = (p1 - pos).DotProduct(dir) * inv_dir;
                            break;
                        }
                    }
                    else
                    {
                        Real t = (p0 - pos).CrossProduct(edge) / (dir.CrossProduct(edge));
                        if (polygon_min_t > t)
                        {
                            polygon_min_t = t;
                        }
                        if (polygon_max_t < t)
                        {
                            polygon_max_t = t;
                        }
                    }
                }

                //find min max t.
                minimum_t = polygon_min_t;
                maximum_t = polygon_max_t;
            }
            else
            {
                return false;
            }
        }
        else
        {
            //ray-plane intersect one point.
            Real    plane_t            = pc.DotProduct(normal) / denominator;
            Vector3 plane_intersection = local_ray.position + local_ray.direction * plane_t;
            //define ellipse.
            bool is_rectangle_contain_point = false;
            if (is_rectangle_contain_point)
            {
                minimum_t = maximum_t = plane_t;
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


    Vector3 Polygon::GetNormal(const Vector3& local_point_on_primitive)
    {
        size_t  size = vertices->size();
        Vector2 v(local_point_on_primitive);
        for (size_t i = 0; i < size; ++i)
        {
            size_t  j = i + 1 < size ? i + 1 : 0;
            Vector2 p0 = vertices->at(i);
            Vector2 p1 = vertices->at(j);
            Vector2 edge = p1 - p0;
            if (p0.IsEqual(p1) == false)
            {
                Real tx = (v.x - p0.x) / edge.x;
                Real ty = (v.y - p0.y) / edge.y;
                Real t = Math::IsEqual(tx, ty) ? tx : (!Math::IsZero(tx) ? tx : ty);

                //is point on edge ?
                if (t <= 1.0f && t >= 0.0f)
                {
                    return Vector3(-v.y, v.x);
                }
            }
        }
        return Math::Vector3::Z_AXIS;
    }

    void Polygon::DrawPrimitive(PrimitiveRenderer* renderer, eRenderingMode mode, const Color& color) const
    {
    }
}
