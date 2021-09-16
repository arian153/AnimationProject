#pragma once
#include "../../Core/Utility/CoreDef.hpp"
#include <vector>

#include "MeshInfo.hpp"
#include "../Common/Vertex/ColorVertexCommon.hpp"
#include "../Common/Vertex/NormalVertexCommon.hpp"
#include "../Common/Vertex/VertexCommon.hpp"
#include "../Common/Vertex/TextureVertexCommon.hpp"

namespace CS460
{
    class MeshData
    {
    public:
        MeshData();
        ~MeshData();

        void Convert(std::vector<ColorVertexCommon>& new_vertices, const Color& color);
        void Convert(std::vector<TextureVertexCommon>& new_vertices);
        void Convert(std::vector<NormalVertexCommon>& new_vertices);

        void    Normalize(Vector3& min, Vector3& max);
        Vector3 GetFaceNormal(U32 a, U32 b, U32 c);

    public:
        std::vector<VertexCommon>      vertices;
        std::vector<GeometryFaceIndex> faces;
        std::vector<U32>               indices;
        bool                           b_resource = false;
        U32                            stride     = sizeof(VertexCommon);
    };
}
