#include "MeshData.hpp"

#include "Color.hpp"
#include "../../Math/Algebra/Vector2.hpp"
#include "../../Math/Algebra/Vector3.hpp"

namespace CS460
{
    MeshData::MeshData()
    {
    }

    MeshData::~MeshData()
    {
    }

    void MeshData::Convert(std::vector<ColorVertexCommon>& new_vertices, const Color& color)
    {
        new_vertices.reserve(vertices.size());
        for (auto& vertex : vertices)
        {
            new_vertices.emplace_back(vertex.GetPosition(), color);
        }
    }

    void MeshData::Convert(std::vector<TextureVertexCommon>& new_vertices)
    {
        new_vertices.reserve(vertices.size());
        for (auto& vertex : vertices)
        {
            new_vertices.emplace_back(vertex.GetPosition(), vertex.GetUV());
        }
    }

    void MeshData::Convert(std::vector<NormalVertexCommon>& new_vertices)
    {
        new_vertices.reserve(vertices.size());
        for (auto& vertex : vertices)
        {
            new_vertices.emplace_back(vertex.GetPosition(), vertex.GetNormal(), vertex.GetTangent(), vertex.GetBinormal());
        }
    }

    void MeshData::Normalize(Vector3& min, Vector3& max)
    {
        U32 size = (U32)vertices.size();
        min      = vertices[0].GetPosition();
        max      = vertices[0].GetPosition();
        for (U32 i = 1; i < size; ++i)
        {
            Vector3 point = vertices[i].GetPosition();
            min.x         = min.x > point.x ? point.x : min.x;
            min.y         = min.y > point.y ? point.y : min.y;
            min.z         = min.z > point.z ? point.z : min.z;
            max.x         = max.x < point.x ? point.x : max.x;
            max.y         = max.y < point.y ? point.y : max.y;
            max.z         = max.z < point.z ? point.z : max.z;
        }
        Vector3 scale        = max - min;
        Real    scale_factor = scale.x;
        scale_factor         = scale_factor < scale.y ? scale.y : scale_factor;
        scale_factor         = scale_factor < scale.z ? scale.z : scale_factor;
        Real    vertex_scale = scale_factor == 0.0f ? 0.0f : 2.0f / scale_factor;
        Vector3 center       = min + scale * 0.5f;
        for (auto& vertex : vertices)
        {
            vertex.SetPosition(vertex_scale * (vertex.GetPosition() - center));
        }
        Real    normal_scale = scale_factor > vertex_scale ? vertex_scale : scale_factor * 0.1f;
        Vector3 scale_center = vertex_scale * center;
    }

    Vector3 MeshData::GetFaceNormal(U32 a, U32 b, U32 c)
    {
        Vector3 edge_ab = vertices[b].GetPosition() - vertices[a].GetPosition();
        Vector3 edge_ac = vertices[c].GetPosition() - vertices[a].GetPosition();
        return CrossProduct(edge_ab, edge_ac).Normalize();
    }
}
