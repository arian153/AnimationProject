#pragma once
#include <d3d11.h>

namespace CS460
{
    class DynamicMeshBufferDX11
    {
    public:
        DynamicMeshBufferDX11();
        ~DynamicMeshBufferDX11();

    protected:
        ID3D11Buffer*        m_vertex_buffer  = nullptr;
        ID3D11Buffer*        m_index_buffer   = nullptr;
        ID3D11DeviceContext* m_device_context = nullptr;
    };
}
