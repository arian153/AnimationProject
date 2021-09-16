#pragma once
#include <directxmath.h>

namespace CS460
{
    class InstanceDataDX11
    {
    public:
        InstanceDataDX11();
        ~InstanceDataDX11();

    public:
        DirectX::XMFLOAT4X4 world;
        DirectX::XMFLOAT4   color;
    };
}
