#pragma once
#include "../../Core/DataStructure/DataStructureDef.hpp"
#include "../../Math/Algebra/Vector3.hpp"

namespace CS460
{
    class MeshVertexIndexInfo
    {
    public:
        MeshVertexIndexInfo()  = default;
        ~MeshVertexIndexInfo() = default;

    public:
        U32 point_index = 0, uv_index = 0, normal_index = 0;
    };

    class MeshFaceIndexInfo
    {
    public:
        MeshFaceIndexInfo()
            : point_index_a(0), point_index_b(0), point_index_c(0), uv_index_a(0), uv_index_b(0), uv_index_c(0), normal_index_a(0), normal_index_b(0), normal_index_c(0)
        {
        }

        MeshFaceIndexInfo(const MeshVertexIndexInfo& a, const MeshVertexIndexInfo& b, const MeshVertexIndexInfo& c)
            : point_index_a(a.point_index),
              point_index_b(b.point_index),
              point_index_c(c.point_index),
              uv_index_a(a.uv_index),
              uv_index_b(b.uv_index),
              uv_index_c(c.uv_index),
              normal_index_a(a.normal_index),
              normal_index_b(b.normal_index),
              normal_index_c(c.normal_index)
        {
        }

        ~MeshFaceIndexInfo()
        {
        }

    public:
        U32 point_index_a,  point_index_b,  point_index_c;
        U32 uv_index_a,     uv_index_b,     uv_index_c;
        U32 normal_index_a, normal_index_b, normal_index_c;
    };

    class GeometryFaceIndex
    {
    public:
        GeometryFaceIndex()  = default;
        ~GeometryFaceIndex() = default;

        GeometryFaceIndex(U32 _a, U32 _b, U32 _c)
            : a(_a), b(_b), c(_c)
        {
        }

    public:
        U32 a = 0, b = 0, c = 0;
    };

    class GeometryPointIndex
    {
    public:
        GeometryPointIndex()  = default;
        ~GeometryPointIndex() = default;

        explicit GeometryPointIndex(U32 i)
            : index(i)
        {
        }

    public:
        U32 index = 0;

        std::vector<GeometryFaceIndex> faces;
    };

    class FaceNormal
    {
    public:
        FaceNormal()  = delete;
        ~FaceNormal() = default;

        FaceNormal(U32 _a, U32 _b, U32 _c)
        {
            index.a = _a;
            index.b = _b;
            index.c = _c;
        }

        FaceNormal(U32 _a, U32 _b, U32 _c, const Vector3& n, const Vector3& c)
            : normal(n), center(c)
        {
            index.a = _a;
            index.b = _b;
            index.c = _c;
        }

        explicit FaceNormal(const GeometryFaceIndex& i)
        {
            index.a = i.a;
            index.b = i.b;
            index.c = i.c;
        }

        FaceNormal(const GeometryFaceIndex& i, const Vector3& n, const Vector3& c)
            : normal(n), center(c)
        {
            index.a = i.a;
            index.b = i.b;
            index.c = i.c;
        }

    public:
        GeometryFaceIndex index;
        Vector3           normal;
        Vector3           center;
    };
}
