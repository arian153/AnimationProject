#include "Tetrahedron.hpp"
#include "../../Utility/Utility.hpp"
#include "../../../Core/Utility/CoreDef.hpp"
#include "../../../Graphics/Utility/PrimitiveRenderer.hpp"

namespace CS460
{
    Tetrahedron::Tetrahedron()
    {
        type = ePrimitiveType::Tetrahedron;
    }

    Tetrahedron::~Tetrahedron()
    {
    }

    void Tetrahedron::Initialize()
    {
        SetUnit();
    }

    void Tetrahedron::Shutdown()
    {
    }

    void Tetrahedron::SetUnit()
    {
        vertices[0] = Math::Vector3::ORIGIN;
        vertices[1] = Math::Vector3::X_AXIS;
        vertices[2] = Math::Vector3::Y_AXIS;
        vertices[3] = Math::Vector3::Z_AXIS;
    }

    Vector3 Tetrahedron::Support(const Vector3& direction)
    {
        Real    p = Math::REAL_NEGATIVE_MAX;
        Vector3 result;
        for (size_t i = 0; i < 4; ++i)
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

    bool Tetrahedron::TestRayIntersection(const Ray& local_ray, Real& minimum_t, Real& maximum_t) const
    {
        minimum_t = local_ray.direction.DotProduct(local_ray.position);
        maximum_t = -1.0f;
        return false;
    }

    Vector3 Tetrahedron::GetNormal(const Vector3& local_point_on_primitive)
    {
        return local_point_on_primitive;
    }

    void Tetrahedron::DrawPrimitive(PrimitiveRenderer* renderer, eRenderingMode mode, const Color& color) const
    {
        I32 index = static_cast<I32>(renderer->VerticesSize(mode));
        renderer->ReserveVertices(4, mode);
        Vector3 world_vertices[4];
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
            for (I32 i = 0; i < 4; ++i)
            {
                renderer->PushIndex(index + i, mode);
            }
        }
        else if (mode == eRenderingMode::Line)
        {
            renderer->PushLineIndices(index, index + 1);
            renderer->PushLineIndices(index, index + 2);
            renderer->PushLineIndices(index, index + 3);
            renderer->PushLineIndices(index + 1, index + 2);
            renderer->PushLineIndices(index + 2, index + 3);
            renderer->PushLineIndices(index + 3, index + 1);
        }
        else if (mode == eRenderingMode::Face)
        {
            renderer->PushFaceIndices(index, index + 1, index + 2);
            renderer->PushFaceIndices(index, index + 2, index + 3);
            renderer->PushFaceIndices(index, index + 3, index + 1);
            renderer->PushFaceIndices(index + 1, index + 2, index + 3);
        }
    }
}
