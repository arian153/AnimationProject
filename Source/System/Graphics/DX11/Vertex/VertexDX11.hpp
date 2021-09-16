#pragma once
#include <directxmath.h>

namespace CS460
{
    class VertexDX11
    {
    public:
        VertexDX11();
        ~VertexDX11();

    public:
        DirectX::XMFLOAT3 position;
        DirectX::XMFLOAT2 uv;
        DirectX::XMFLOAT3 normal;
        DirectX::XMFLOAT3 tangent;
        DirectX::XMFLOAT3 binormal;
    };
}
