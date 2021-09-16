#include "ConverterDX11.hpp"
#include "../../Math/Algebra/Vector2.hpp"
#include "../../Math/Algebra/Vector3.hpp"
#include "../../Math/Algebra/Vector4.hpp"
#include "../../Math/Algebra/Quaternion.hpp"
#include "../../Math/Algebra/Matrix44.hpp"
#include "../DataType/Color.hpp"
#include "../../Math/Algebra/Matrix33.hpp"

namespace CS460
{
    namespace ConverterDX11
    {
        Vector3 ToVector3(const DirectX::XMVECTOR& xmvector)
        {
            return Vector3(xmvector.m128_f32[0], xmvector.m128_f32[1], xmvector.m128_f32[2]);
        }

        Vector3 ToVector3(const DirectX::XMFLOAT3& xmfloat3)
        {
            return Vector3(xmfloat3.x, xmfloat3.y, xmfloat3.z);
        }

        Vector4 ToVector4(const DirectX::XMVECTOR& xmvector)
        {
            return Vector4(xmvector.m128_f32[0], xmvector.m128_f32[1], xmvector.m128_f32[2], xmvector.m128_f32[3]);
        }

        Vector4 ToVector4(const DirectX::XMFLOAT4& xmfloat4)
        {
            return Vector4(xmfloat4.x, xmfloat4.y, xmfloat4.z, xmfloat4.w);
        }

        Quaternion ToQuaternion(const DirectX::XMVECTOR& xmvector)
        {
            return Quaternion(xmvector.m128_f32[3], xmvector.m128_f32[0], xmvector.m128_f32[1], xmvector.m128_f32[2]);
        }

        Quaternion ToQuaternion(const DirectX::XMFLOAT4& xmfloat4)
        {
            return Quaternion(xmfloat4.w, xmfloat4.x, xmfloat4.y, xmfloat4.z);
        }

        DirectX::XMVECTOR ToXMVector(const Vector3& vector)
        {
            DirectX::XMFLOAT3 float3(vector.x, vector.y, vector.z);
            return DirectX::XMLoadFloat3(&float3);
        }

        DirectX::XMVECTOR ToXMVector(const Vector4& vector)
        {
            DirectX::XMFLOAT4 float4(vector.x, vector.y, vector.z, vector.w);
            return DirectX::XMLoadFloat4(&float4);
        }

        DirectX::XMVECTOR ToXMVector(const Quaternion& quaternion)
        {
            DirectX::XMFLOAT4 float4(quaternion.i, quaternion.j, quaternion.k, quaternion.r);
            return DirectX::XMLoadFloat4(&float4);
        }

        DirectX::XMFLOAT2 ToXMFloat2(const Vector2& vector)
        {
            return DirectX::XMFLOAT2(vector.x, vector.y);
        }

        DirectX::XMFLOAT3 ToXMFloat3(const Vector3& vector)
        {
            return DirectX::XMFLOAT3(vector.x, vector.y, vector.z);
        }

        DirectX::XMFLOAT4 ToXMFloat4(const Vector4& vector)
        {
            return DirectX::XMFLOAT4(vector.x, vector.y, vector.z, vector.w);
        }

        DirectX::XMFLOAT4 ToXMFloat4(const Quaternion& quaternion)
        {
            return DirectX::XMFLOAT4(quaternion.i, quaternion.j, quaternion.k, quaternion.r);
        }

        DirectX::XMFLOAT4 ToXMFloat4(const Color& color)
        {
            return DirectX::XMFLOAT4(color.r, color.g, color.b, color.a);
        }

        Matrix44 ToMatrix44(const DirectX::XMMATRIX& xmmatrix)
        {
            return Matrix44(
                            xmmatrix.r[0].m128_f32[0], xmmatrix.r[0].m128_f32[1], xmmatrix.r[0].m128_f32[2], xmmatrix.r[0].m128_f32[3],
                            xmmatrix.r[1].m128_f32[0], xmmatrix.r[1].m128_f32[1], xmmatrix.r[1].m128_f32[2], xmmatrix.r[1].m128_f32[3],
                            xmmatrix.r[2].m128_f32[0], xmmatrix.r[2].m128_f32[1], xmmatrix.r[2].m128_f32[2], xmmatrix.r[2].m128_f32[3],
                            xmmatrix.r[3].m128_f32[0], xmmatrix.r[3].m128_f32[1], xmmatrix.r[3].m128_f32[2], xmmatrix.r[3].m128_f32[3]
                           );
        }

        Matrix44 ToMatrix44(const DirectX::XMFLOAT4X4& xmfloat44)
        {
            return Matrix44(
                            xmfloat44._11, xmfloat44._12, xmfloat44._13, xmfloat44._14,
                            xmfloat44._21, xmfloat44._22, xmfloat44._23, xmfloat44._24,
                            xmfloat44._31, xmfloat44._32, xmfloat44._33, xmfloat44._34,
                            xmfloat44._41, xmfloat44._42, xmfloat44._43, xmfloat44._44
                           );
        }

        Color ToColor(const DirectX::XMFLOAT4& xmfloat4)
        {
            return Color(xmfloat4.x, xmfloat4.y, xmfloat4.z, xmfloat4.w);
        }

        DirectX::XMFLOAT4X4 ToXMFloat4X4(const Matrix44& matrix)
        {
            DirectX::XMFLOAT4X4 result(
                                       matrix.data[0], matrix.data[1], matrix.data[2], matrix.data[3],
                                       matrix.data[4], matrix.data[5], matrix.data[6], matrix.data[7],
                                       matrix.data[8], matrix.data[9], matrix.data[10], matrix.data[11],
                                       matrix.data[12], matrix.data[13], matrix.data[14], matrix.data[15]);
            return result;
        }

        DirectX::XMFLOAT3X3 ToXMFloat3X3(const Matrix33& matrix)
        {
            DirectX::XMFLOAT3X3 result(
                                       matrix.data[0], matrix.data[1], matrix.data[2],
                                       matrix.data[3], matrix.data[4], matrix.data[5],
                                       matrix.data[6], matrix.data[7], matrix.data[8]);
            return result;
        }

        DirectX::XMMATRIX ToXMMatrix(const Matrix44& matrix)
        {
            DirectX::XMFLOAT4X4 result(
                                       matrix.data[0], matrix.data[1], matrix.data[2], matrix.data[3],
                                       matrix.data[4], matrix.data[5], matrix.data[6], matrix.data[7],
                                       matrix.data[8], matrix.data[9], matrix.data[10], matrix.data[11],
                                       matrix.data[12], matrix.data[13], matrix.data[14], matrix.data[15]);
            return DirectX::XMLoadFloat4x4(&result);
        }
    }
}
