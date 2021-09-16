#include "NormalVertexDX11.hpp"
#include "../ConverterDX11.hpp"
#include "../../../Math/Algebra/Vector3.hpp"
#include "../../../Math/Structure/Basis.hpp"
#include "../../Common/Vertex/NormalVertexCommon.hpp"

namespace CS460
{
    NormalVertexDX11::NormalVertexDX11()
        : position(), normal(), tangent(), binormal()
    {
    }

    NormalVertexDX11::~NormalVertexDX11()
    {
    }

    NormalVertexCommon::NormalVertexCommon()
    {
    }

    NormalVertexCommon::NormalVertexCommon(Real px, Real py, Real pz)
    {
        position.x = px;
        position.y = py;
        position.z = pz;
    }

    NormalVertexCommon::NormalVertexCommon(Real px, Real py, Real pz, Real nx, Real ny, Real nz)
    {
        position.x = px;
        position.y = py;
        position.z = pz;
        normal.x   = nx;
        normal.y   = ny;
        normal.z   = nz;
    }

    NormalVertexCommon::NormalVertexCommon(Real px, Real py, Real pz, Real nx, Real ny, Real nz, Real tx, Real ty, Real tz)
    {
        position.x = px;
        position.y = py;
        position.z = pz;
        normal.x   = nx;
        normal.y   = ny;
        normal.z   = nz;
        tangent.x  = tx;
        tangent.y  = ty;
        tangent.z  = tz;
        binormal   = ConverterDX11::ToXMFloat3(CrossProduct(Vector3(tx, ty, tz), Vector3(nx, ny, nz)).Normalize());
    }

    NormalVertexCommon::NormalVertexCommon(const Vector3& p)
    {
        position = ConverterDX11::ToXMFloat3(p);
    }

    NormalVertexCommon::NormalVertexCommon(const Vector3& p, const Vector3& n)
    {
        position = ConverterDX11::ToXMFloat3(p);
        normal   = ConverterDX11::ToXMFloat3(n);
    }

    NormalVertexCommon::NormalVertexCommon(const Vector3& p, const Vector3& n, const Vector3& t)
    {
        position = ConverterDX11::ToXMFloat3(p);
        normal   = ConverterDX11::ToXMFloat3(n);
        tangent  = ConverterDX11::ToXMFloat3(t);
        binormal = ConverterDX11::ToXMFloat3(CrossProduct(t, n).Normalize());
    }

    NormalVertexCommon::NormalVertexCommon(const Vector3& p, const Vector3& n, const Vector3& t, const Vector3& b)
    {
        position = ConverterDX11::ToXMFloat3(p);
        normal   = ConverterDX11::ToXMFloat3(n);
        tangent  = ConverterDX11::ToXMFloat3(t);
        binormal = ConverterDX11::ToXMFloat3(b);
    }

    Vector3 NormalVertexCommon::GetPosition() const
    {
        return ConverterDX11::ToVector3(position);
    }

    Vector3 NormalVertexCommon::GetNormal() const
    {
        return ConverterDX11::ToVector3(normal);
    }

    Vector3 NormalVertexCommon::GetTangent() const
    {
        return ConverterDX11::ToVector3(tangent);
    }

    void NormalVertexCommon::SetPosition(const Vector3& p)
    {
        position = ConverterDX11::ToXMFloat3(p);
    }

    void NormalVertexCommon::SetNormal(const Vector3& n)
    {
        normal = ConverterDX11::ToXMFloat3(n);
    }

    void NormalVertexCommon::SetTangent(const Vector3& t)
    {
        tangent = ConverterDX11::ToXMFloat3(t);
    }

    void NormalVertexCommon::CalculateBinormal()
    {
        Vector3 n  = ConverterDX11::ToVector3(normal);
        Vector3 t  = ConverterDX11::ToVector3(tangent);
        Vector3 b  = CrossProduct(t, n).Normalize();
        binormal.x = b.x;
        binormal.y = b.y;
        binormal.z = b.z;
    }

    void NormalVertexCommon::CalculateTangentAndBinormal()
    {
        Vector3 n = ConverterDX11::ToVector3(normal);
        Basis   basis;
        basis.CalculateBasisQuaternion(n);
        normal   = ConverterDX11::ToXMFloat3(basis.i);
        tangent  = ConverterDX11::ToXMFloat3(basis.j);
        binormal = ConverterDX11::ToXMFloat3(basis.k);
    }
}
