#pragma once
#include "../../GraphicsAPI.hpp"
#include IncludeVertexAPI
#include "../../../Math/Utility/MathDef.hpp"

namespace CS460
{
    class Vector2;
    class Vector3;

    class VertexCommon : public VertexAPI
    {
    public:
        VertexCommon();
        VertexCommon(Real px, Real py, Real pz, Real u, Real v);
        VertexCommon(Real px, Real py, Real pz, Real u, Real v, Real nx, Real ny, Real nz);
        VertexCommon(Real px, Real py, Real pz, Real u, Real v, Real nx, Real ny, Real nz, Real tx, Real ty, Real tz);

        explicit VertexCommon(const Vector3& p);
        VertexCommon(const Vector3& p, const Vector2& _uv);
        VertexCommon(const Vector3& p, const Vector2& _uv, const Vector3& n);
        VertexCommon(const Vector3& p, const Vector2& _uv, const Vector3& n, const Vector3& t);
        VertexCommon(const Vector3& p, const Vector2& _uv, const Vector3& n, const Vector3& t, const Vector3& b);

        void CalculateBinormal();
        void CalculateTangentAndBinormal();

        Vector3 GetPosition() const;
        Vector2 GetUV() const;
        Vector3 GetNormal() const;
        Vector3 GetTangent() const;
        Vector3 GetBinormal() const;

        void SetPosition(const Vector3& p);
        void SetUV(const Vector2& _uv);
        void SetNormal(const Vector3& n);
        void SetTangent(const Vector3& t);
        void SetBinormal(const Vector3& b);

    public:
    };
}
