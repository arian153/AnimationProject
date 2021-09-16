#pragma once
#include <directxmath.h>

namespace CS460
{
    class Color;
    class Vector2;
    class Vector3;
    class Vector4;
    class Quaternion;
    class Matrix33;
    class Matrix44;

    namespace ConverterDX11
    {
        Vector3 ToVector3(const DirectX::XMVECTOR& xmvector);
        Vector3 ToVector3(const DirectX::XMFLOAT3& xmfloat3);

        Vector4 ToVector4(const DirectX::XMVECTOR& xmvector);
        Vector4 ToVector4(const DirectX::XMFLOAT4& xmfloat4);

        Quaternion ToQuaternion(const DirectX::XMVECTOR& xmvector);
        Quaternion ToQuaternion(const DirectX::XMFLOAT4& xmfloat4);

        Matrix44 ToMatrix44(const DirectX::XMMATRIX& xmmatrix);
        Matrix44 ToMatrix44(const DirectX::XMFLOAT4X4& xmfloat44);

        Color ToColor(const DirectX::XMFLOAT4& xmfloat4);

        DirectX::XMVECTOR ToXMVector(const Vector3& vector);
        DirectX::XMVECTOR ToXMVector(const Vector4& vector);
        DirectX::XMVECTOR ToXMVector(const Quaternion& quaternion);
        DirectX::XMMATRIX ToXMMatrix(const Matrix44& matrix);

        DirectX::XMFLOAT2 ToXMFloat2(const Vector2& vector);
        DirectX::XMFLOAT3 ToXMFloat3(const Vector3& vector);
        DirectX::XMFLOAT4 ToXMFloat4(const Vector4& vector);
        DirectX::XMFLOAT4 ToXMFloat4(const Quaternion& quaternion);
        DirectX::XMFLOAT4 ToXMFloat4(const Color& color);

        DirectX::XMFLOAT4X4 ToXMFloat4X4(const Matrix44& matrix);
        DirectX::XMFLOAT3X3 ToXMFloat3X3(const Matrix33& matrix);
    }
}
