#pragma once
#include "../../../Math/Utility/MathDef.hpp"
#include "../../../Math/Algebra/Vector2.hpp"
#include "../../../Math/Algebra/Vector3.hpp"
#include "../../../Math/Algebra/Vector4.hpp"

namespace CS460
{
    class SkinnedVertexCommon
    {
    public:
        SkinnedVertexCommon();
        SkinnedVertexCommon(const Vector3& p, const Vector4& w, const Vector4& i);
        SkinnedVertexCommon(const Vector3& p, const Vector2& uv, const Vector4& w, const Vector4& i);
        SkinnedVertexCommon(const Vector3& p, const Vector2& uv, const Vector3& n, const Vector4& w, const Vector4& i);
        SkinnedVertexCommon(const Vector3& p, const Vector2& uv, const Vector3& n, const Vector3& t, const Vector4& w, const Vector4& i);
        SkinnedVertexCommon(const Vector3& p, const Vector2& uv, const Vector3& n, const Vector3& t, const Vector3& b, const Vector4& w, const Vector4& i);

        void CalculateBinormal();
        void CalculateTangentAndBinormal();

    public:
        Vector3 position;
        Vector2 tex;
        Vector3 normal;
        Vector3 tangent;
        Vector3 binormal;
        Vector4 weights;
        Vector4 indices;
    };
}
