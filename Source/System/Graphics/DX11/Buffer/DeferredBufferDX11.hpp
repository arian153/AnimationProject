#pragma once
#include <d3d11.h>
#include "../../../Core/Utility/CoreDef.hpp"
#include "../../../Math/Utility/MathDef.hpp"

namespace CS460
{
    class Color;
    const U32 BUFFER_COUNT = 2;

    class DeferredBufferDX11
    {
    public:
        DeferredBufferDX11();
        ~DeferredBufferDX11();

        void SetDevice(ID3D11Device* device);
        void SetDeviceContext(ID3D11DeviceContext* device_context);
        bool BuildBuffer(U32 texture_width, U32 texture_height);
        bool BuildVertexBuffer(Real left, Real right, Real top, Real bottom);
        bool ResizeVertexBuffer(Real left, Real right, Real top, Real bottom) const;
        U32  GetIndexCount() const;

        ID3D11ShaderResourceView* GetShaderResourceView(U32 view);

    protected:
        U32 m_texture_width  = 512;
        U32 m_texture_height = 512;
        U32 m_vertex_count   = 0;
        U32 m_index_count    = 0;

        ID3D11Device*             m_device         = nullptr;
        ID3D11DeviceContext*      m_device_context = nullptr;
        ID3D11Texture2D*          m_render_target_texture_array[ BUFFER_COUNT ];
        ID3D11RenderTargetView*   m_render_target_view_array[ BUFFER_COUNT ];
        ID3D11ShaderResourceView* m_shader_resource_view_array[ BUFFER_COUNT ];
        ID3D11Texture2D*          m_depth_stencil_buffer = nullptr;
        ID3D11DepthStencilView*   m_depth_stencil_view   = nullptr;
        ID3D11Buffer*             m_vertex_buffer        = nullptr;
        ID3D11Buffer*             m_index_buffer         = nullptr;
        D3D11_VIEWPORT            m_viewport;
    };
}
