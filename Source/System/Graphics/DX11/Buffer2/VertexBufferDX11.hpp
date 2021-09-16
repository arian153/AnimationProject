#pragma once

#include <d3d11.h>

namespace CS460
{
    class VertexBufferDX11
    {
    public:
        VertexBufferDX11();
        ~VertexBufferDX11();

    protected:
        ID3D11Buffer*          m_vertex_buffer  = nullptr;
        ID3D11DeviceContext*   m_device_context = nullptr;
        D3D_PRIMITIVE_TOPOLOGY m_topology       = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
    };
}
