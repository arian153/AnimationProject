#pragma once
#include "../../../Core/Utility/CoreDef.hpp"
#include "../../../Math/Utility/MathDef.hpp"
#include "../../../Math/Algebra/Vector2.hpp"
#include "../../../Math/Algebra/Vector3.hpp"
#include "../../../Math/Algebra/Vector4.hpp"

namespace CS460
{
    struct IVector4
    {
        I32 x = -1;
        I32 y = -1;
        I32 z = -1;
        I32 w = -1;
    };

    class SkinnedVertexCommon
    {
    public:
        SkinnedVertexCommon();
        SkinnedVertexCommon(const Vector3& p, const Vector4& w, const IVector4& i);
        SkinnedVertexCommon(const Vector3& p, const Vector2& uv, const Vector4& w, const IVector4& i);
        SkinnedVertexCommon(const Vector3& p, const Vector2& uv, const Vector3& n, const Vector4& w, const IVector4& i);
        SkinnedVertexCommon(const Vector3& p, const Vector2& uv, const Vector3& n, const Vector3& t, const Vector4& w, const IVector4& i);
        SkinnedVertexCommon(const Vector3& p, const Vector2& uv, const Vector3& n, const Vector3& t, const Vector3& b, const Vector4& w, const IVector4& i);

        void CalculateBinormal();
        void CalculateTangentAndBinormal();

    public:
        Vector3  position;
        Vector2  tex;
        Vector3  normal;
        Vector3  tangent;
        Vector3  binormal;
        Vector4  weights;
        IVector4 indices;
    };
}
