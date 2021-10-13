#include "SkinnedVertexCommon.hpp"

#include "../../../Math/Structure/Basis.hpp"

namespace CS460
{
    SkinnedVertexCommon::SkinnedVertexCommon()
    {
    }

    SkinnedVertexCommon::SkinnedVertexCommon(const Vector3& p, const Vector4& w, const Vector4& i)
        : position(p), weights(w), indices(i)
    {
    }

    SkinnedVertexCommon::SkinnedVertexCommon(const Vector3& p, const Vector2& uv, const Vector4& w, const Vector4& i)
        : position(p), tex(uv), weights(w), indices(i)
    {
    }

    SkinnedVertexCommon::SkinnedVertexCommon(const Vector3& p, const Vector2& uv, const Vector3& n, const Vector4& w, const Vector4& i)
        : position(p), tex(uv), normal(n), weights(w), indices(i)
    {
    }

    SkinnedVertexCommon::SkinnedVertexCommon(const Vector3& p, const Vector2& uv, const Vector3& n, const Vector3& t, const Vector4& w, const Vector4& i)
        : position(p), tex(uv), normal(n), tangent(t), weights(w), indices(i)
    {
    }

    SkinnedVertexCommon::SkinnedVertexCommon(const Vector3& p, const Vector2& uv, const Vector3& n, const Vector3& t, const Vector3& b, const Vector4& w, const Vector4& i)
        : position(p), tex(uv), normal(n), tangent(t), binormal(b), weights(w), indices(i)
    {
    }

    void SkinnedVertexCommon::CalculateBinormal()
    {
        binormal = CrossProduct(tangent, normal).Normalize();
    }

    void SkinnedVertexCommon::CalculateTangentAndBinormal()
    {
        Basis basis;
        basis.CalculateBasisQuaternion(normal);
        normal   = basis.i;
        tangent  = basis.j;
        binormal = basis.k;
    }
}
