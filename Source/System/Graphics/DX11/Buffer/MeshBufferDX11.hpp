#pragma once
#include <d3d11.h>

namespace CS460
{
    class MeshBufferDX11
    {
    public:
        MeshBufferDX11();
        ~MeshBufferDX11();

    public:
        ID3D11Buffer*        m_vertex_buffer  = nullptr;
        ID3D11Buffer*        m_index_buffer   = nullptr;
        ID3D11DeviceContext* m_device_context = nullptr;
    };
}
