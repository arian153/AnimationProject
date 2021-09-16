#pragma once
#include <d3d11.h>

namespace CS460
{
    class ConstantBufferDX11
    {
    public:
        ConstantBufferDX11();
        ~ConstantBufferDX11();

    protected:
        ID3D11Buffer*        m_constant_buffer = nullptr;
        ID3D11DeviceContext* m_device_context  = nullptr;
    };
}
