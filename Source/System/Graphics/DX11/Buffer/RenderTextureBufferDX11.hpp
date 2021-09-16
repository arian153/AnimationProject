#pragma once
#include <d3d11.h>
#include "../../../Core/Utility/CoreDef.hpp"

namespace CS460
{
    class Color;
    class RendererCommon;

    class RenderTextureBufferDX11
    {
    public:
        RenderTextureBufferDX11();
        ~RenderTextureBufferDX11();

        bool BuildTextureBuffer(size_t width, size_t height);
        bool BuildVertexBuffer(size_t width, size_t height);
        bool ResizeVertexBuffer(size_t width, size_t height) const;
        void SetRenderTarget() const;
        void ClearRenderTarget(const Color& color) const;

        ID3D11ShaderResourceView* GetTextureResource() const;
        ID3D11DeviceContext* GetDeviceContext() const;

    protected:
        ID3D11Texture2D*          m_render_target_texture = nullptr;
        ID3D11RenderTargetView*   m_render_target_view    = nullptr;
        ID3D11ShaderResourceView* m_shader_resource_view  = nullptr;
        ID3D11Buffer*             m_vertex_buffer         = nullptr;
        ID3D11Buffer*             m_index_buffer          = nullptr;
        ID3D11Device*             m_device                = nullptr;
        ID3D11DeviceContext*      m_device_context        = nullptr;
        RendererCommon*           m_renderer              = nullptr;

        U32 m_vertex_count = 0;
        U32 m_index_count  = 0;
    };
}
