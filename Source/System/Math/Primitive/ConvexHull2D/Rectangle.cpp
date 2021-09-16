#include "Rectangle.hpp"
#include "../../Utility/Utility.hpp"
#include "../../../Core/Utility/CoreDef.hpp"
#include "../../../Graphics/Utility/PrimitiveRenderer.hpp"

namespace CS460
{
    Rectangle::Rectangle()
    {
        type = ePrimitiveType::Rectangle;
    }

    Rectangle::~Rectangle()
    {
    }

    void Rectangle::Initialize()
    {
    }

    void Rectangle::Shutdown()
    {
    }

    void Rectangle::SetUnit()
    {
        Real w = 0.5f;
        Real h = 0.5f;

        vertices[0].Set(+w, +h);
        vertices[1].Set(+w, -h);
        vertices[2].Set(-w, +h);
        vertices[3].Set(-w, -h);
    }

    Vector3 Rectangle::Support(const Vector3& direction)
    {
        Vector2 sub_space_direction;
        sub_space_direction.x = direction.x;
        sub_space_direction.y = direction.y;
        sub_space_direction.SetNormalize();
        Vector2 result;
        Real    p = Math::REAL_NEGATIVE_MAX;
        for (size_t i = 0; i < 4; ++i)
        {
            Real projection = vertices[i].DotProduct(sub_space_direction);
            if (projection > p)
            {
                result = vertices[i];
                p = projection;
            }
        }
        return Vector3(result.x, result.y, 0.0f);
    }

    bool Rectangle::TestRayIntersection(const Ray& local_ray, Real& minimum_t, Real& maximum_t) const
    {
        //add a ray-collider intersection test code.
        minimum_t = -1.0f;
        maximum_t = -1.0f;
        //plane elements
        Vector3 normal(0.0f, 0.0f, 1.0f);
        Vector3 pc = -local_ray.position;
        Real    denominator = normal.DotProduct(local_ray.direction);
        Real    t_y_min, t_y_max;
        Vector2 rect_min = vertices[3];
        Vector2 rect_max = vertices[0];
        Vector3 inv_dir = local_ray.direction;
        inv_dir.SetInverse();
        if (Math::IsZero(denominator) == true)
        {
            //ray is parallel to plane.
            if (Math::IsZero(pc.DotProduct(normal)) == true)
            {
                //ray is on the plane.
                if (inv_dir.x >= 0.0f)
                {
                    minimum_t = (rect_min.x - local_ray.position.x) * inv_dir.x;
                    maximum_t = (rect_max.x - local_ray.position.x) * inv_dir.x;
                }
                else
                {
                    minimum_t = (rect_max.x - local_ray.position.x) * inv_dir.x;
                    maximum_t = (rect_min.x - local_ray.position.x) * inv_dir.x;
                }
                if (inv_dir.y >= 0.0f)
                {
                    t_y_min = (rect_min.y - local_ray.position.y) * inv_dir.y;
                    t_y_max = (rect_max.y - local_ray.position.y) * inv_dir.y;
                }
                else
                {
                    t_y_min = (rect_max.y - local_ray.position.y) * inv_dir.y;
                    t_y_max = (rect_min.y - local_ray.position.y) * inv_dir.y;
                }
                if ((minimum_t > t_y_max) || (t_y_min > maximum_t))
                {
                    return false;
                }
                if (t_y_min > minimum_t)
                {
                    minimum_t = t_y_min;
                }
                if (t_y_max < maximum_t)
                {
                    maximum_t = t_y_max;
                }
            }
            else
            {
                return false;
            }
        }
        else
        {
            //ray-plane intersect one point.
            Real    plane_t = pc.DotProduct(normal) / denominator;
            Vector3 plane_intersection = local_ray.position + local_ray.direction * plane_t;
            //define rectangle.
            if (rect_min.x <= plane_intersection.x && rect_max.x >= plane_intersection.x &&
                rect_min.y <= plane_intersection.y && rect_max.y >= plane_intersection.y)
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

    Vector3 Rectangle::GetNormal(const Vector3& local_point_on_primitive)
    {
        Vector2 rect_min = vertices[3];
        Vector2 rect_max = vertices[0];
        if (Math::IsEqual(local_point_on_primitive.x, rect_min.x))
        {
            return Vector3(-1.0f);
        }
        if (Math::IsEqual(local_point_on_primitive.x, rect_max.x))
        {
            return Vector3(1.0f);
        }
        if (Math::IsEqual(local_point_on_primitive.y, rect_min.y))
        {
            return Vector3(0.0f, -1.0f);
        }
        if (Math::IsEqual(local_point_on_primitive.y, rect_max.y))
        {
            return Vector3(0.0f, 1.0f);
        }
        return Math::Vector3::Z_AXIS;
    }

    void Rectangle::DrawPrimitive(PrimitiveRenderer* renderer, eRenderingMode mode, const Color& color) const
    {
        I32 index = static_cast<I32>(renderer->VerticesSize(mode));
        I32 count = 4;
        renderer->ReserveVertices(count, mode);
        Vector3 world_vertices[4];
        for (size_t i = 0; i < 4; ++i)
        {
            //local space to world space
            Vector3 vertex(vertices[i].x, vertices[i].y);
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
                I32 j = i + 1 < count ? i + 1 : 0;
                renderer->PushLineIndices(index + i, index + j);
            }
        }
        else if (mode == eRenderingMode::Face)
        {
            renderer->PushFaceIndices(index + 0, index + 1, index + 2);
            renderer->PushFaceIndices(index + 0, index + 1, index + 3);
        }
    }
}
