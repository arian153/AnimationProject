#include "Box.hpp"
#include "../../Utility/Utility.hpp"
#include "../../../Core/Utility/CoreDef.hpp"
#include "../../../Graphics/Utility/PrimitiveRenderer.hpp"

namespace CS460
{
    Box::Box()
    {
        type = ePrimitiveType::Box;
    }

    Box::~Box()
    {
    }

    void Box::Initialize()
    {
        SetUnit();
    }

    void Box::Shutdown()
    {
    }

    void Box::SetUnit()
    {
        Real w = 0.5f;
        Real h = 0.5f;
        Real d = 0.5f;

        vertices[0].Set(+w, +h, +d);
        vertices[1].Set(+w, +h, -d);
        vertices[2].Set(+w, -h, +d);
        vertices[3].Set(+w, -h, -d);
        vertices[4].Set(-w, +h, +d);
        vertices[5].Set(-w, +h, -d);
        vertices[6].Set(-w, -h, +d);
        vertices[7].Set(-w, -h, -d);
    }

    Vector3 Box::Support(const Vector3& direction)
    {
        Real    p = Math::REAL_NEGATIVE_MAX;
        Vector3 result;
        for (size_t i = 0; i < 8; ++i)
        {
            Real projection = vertices[i].DotProduct(direction);
            if (projection > p)
            {
                result = vertices[i];
                p      = projection;
            }
        }
        return result;
    }

    bool Box::TestRayIntersection(const Ray& local_ray, Real& minimum_t, Real& maximum_t) const
    {
        Real    t_min, t_max, t_y_min, t_y_max, t_z_min, t_z_max;
        Vector3 box_min = vertices[7];
        Vector3 box_max = vertices[0];
        Vector3 inv_dir = local_ray.direction;
        inv_dir.SetInverse();
        if (inv_dir.x >= 0.0f)
        {
            t_min = (box_min.x - local_ray.position.x) * inv_dir.x;
            t_max = (box_max.x - local_ray.position.x) * inv_dir.x;
        }
        else
        {
            t_min = (box_max.x - local_ray.position.x) * inv_dir.x;
            t_max = (box_min.x - local_ray.position.x) * inv_dir.x;
        }
        if (inv_dir.y >= 0.0f)
        {
            t_y_min = (box_min.y - local_ray.position.y) * inv_dir.y;
            t_y_max = (box_max.y - local_ray.position.y) * inv_dir.y;
        }
        else
        {
            t_y_min = (box_max.y - local_ray.position.y) * inv_dir.y;
            t_y_max = (box_min.y - local_ray.position.y) * inv_dir.y;
        }
        if ((t_min > t_y_max) || (t_y_min > t_max))
        {
            return false;
        }
        if (t_y_min > t_min)
        {
            t_min = t_y_min;
        }
        if (t_y_max < t_max)
        {
            t_max = t_y_max;
        }
        if (inv_dir.z >= 0.0f)
        {
            t_z_min = (box_min.z - local_ray.position.z) * inv_dir.z;
            t_z_max = (box_max.z - local_ray.position.z) * inv_dir.z;
        }
        else
        {
            t_z_min = (box_max.z - local_ray.position.z) * inv_dir.z;
            t_z_max = (box_min.z - local_ray.position.z) * inv_dir.z;
        }
        if ((t_min > t_z_max) || (t_z_min > t_max))
        {
            return false;
        }
        if (t_z_min > t_min)
        {
            t_min = t_z_min;
        }
        if (t_z_max < t_max)
        {
            t_max = t_z_max;
        }
        if (t_min < 0.0f && t_max < 0.0f)
        {
            return false;
        }
        if (t_min <= 0.0f)
        {
            t_min = 0.0f;
        }
        minimum_t = t_min;
        maximum_t = t_max;
        return true;
    }

    Vector3 Box::GetNormal(const Vector3& local_point_on_primitive)
    {
        Vector3 normal;
        Real    w = (vertices[0] - vertices[4]).x;
        Real    h = (vertices[0] - vertices[2]).y;
        Real    d = (vertices[0] - vertices[1]).z;
        Vector3 size(w, h, d);
        Real    min      = Math::REAL_POSITIVE_MAX;
        Real    distance = fabsf(size.x - fabsf(local_point_on_primitive.x));
        if (distance < min)
        {
            min = distance;
            normal.Set(1.0f, 0.0f, 0.0f);
            normal *= Math::Signum(local_point_on_primitive.x);
        }
        distance = fabsf(size.y - fabsf(local_point_on_primitive.y));
        if (distance < min)
        {
            min = distance;
            normal.Set(0.0f, 1.0f, 0.0f);
            normal *= Math::Signum(local_point_on_primitive.y);
        }
        distance = fabsf(size.z - fabsf(local_point_on_primitive.z));
        if (distance < min)
        {
            normal.Set(0.0f, 0.0f, 1.0f);
            normal *= Math::Signum(local_point_on_primitive.z);
        }
        return normal;
    }

    void Box::DrawPrimitive(PrimitiveRenderer* renderer, eRenderingMode mode, const Color& color) const
    {
        I32 index = static_cast<I32>(renderer->VerticesSize(mode));
        renderer->ReserveVertices(8, mode);
        Vector3 world_vertices[8];
        std::memcpy(world_vertices, vertices, sizeof(vertices));
        for (auto& vertex : world_vertices)
        {
            //local space to world space
            vertex = orientation.Rotate(vertex);
            vertex += position;

            //push to renderer
            renderer->PushVertex(vertex, mode, color);
        }

        //add indices
        if (mode == eRenderingMode::Dot)
        {
            for (I32 i = 0; i < 8; ++i)
            {
                renderer->PushIndex(index + i, mode);
            }
        }
        else if (mode == eRenderingMode::Line)
        {
            //front
            renderer->PushLineIndices(index + 0, index + 2);
            renderer->PushLineIndices(index + 2, index + 6);
            renderer->PushLineIndices(index + 6, index + 4);
            renderer->PushLineIndices(index + 4, index + 0);
            //back
            renderer->PushLineIndices(index + 1, index + 3);
            renderer->PushLineIndices(index + 3, index + 7);
            renderer->PushLineIndices(index + 7, index + 5);
            renderer->PushLineIndices(index + 5, index + 1);
            //side
            renderer->PushLineIndices(index + 0, index + 1);
            renderer->PushLineIndices(index + 2, index + 3);
            renderer->PushLineIndices(index + 6, index + 7);
            renderer->PushLineIndices(index + 4, index + 5);
        }
        else if (mode == eRenderingMode::Face)
        {
            //front
            renderer->PushFaceIndices(index + 6, index + 2, index + 4);
            renderer->PushFaceIndices(index + 2, index + 0, index + 4);
            //back
            renderer->PushFaceIndices(index + 1, index + 3, index + 5);
            renderer->PushFaceIndices(index + 3, index + 7, index + 5);
            //left
            renderer->PushFaceIndices(index + 6, index + 5, index + 7);
            renderer->PushFaceIndices(index + 5, index + 6, index + 4);
            //right
            renderer->PushFaceIndices(index + 1, index + 0, index + 3);
            renderer->PushFaceIndices(index + 0, index + 2, index + 3);
            //top
            renderer->PushFaceIndices(index + 1, index + 4, index + 0);
            renderer->PushFaceIndices(index + 1, index + 5, index + 4);
            //bottom
            renderer->PushFaceIndices(index + 3, index + 2, index + 6);
            renderer->PushFaceIndices(index + 3, index + 6, index + 7);
        }
    }

    void Box::SetBox(Real width, Real height, Real depth)
    {
        Real w = 0.5f * width;
        Real h = 0.5f * height;
        Real d = 0.5f * depth;

        vertices[0].Set(+w, +h, +d);
        vertices[1].Set(+w, +h, -d);
        vertices[2].Set(+w, -h, +d);
        vertices[3].Set(+w, -h, -d);
        vertices[4].Set(-w, +h, +d);
        vertices[5].Set(-w, +h, -d);
        vertices[6].Set(-w, -h, +d);
        vertices[7].Set(-w, -h, -d);

    }
}
