#pragma once

#include <d3d11.h>

namespace CS460
{
    class IndexBufferDX11
    {
    public:
        IndexBufferDX11();
        ~IndexBufferDX11();

    protected:
        ID3D11Buffer*        m_index_buffer   = nullptr;
        ID3D11DeviceContext* m_device_context = nullptr;
    };
}
