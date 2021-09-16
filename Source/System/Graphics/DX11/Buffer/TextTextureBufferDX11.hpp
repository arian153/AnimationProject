#pragma once
#include <d3d11.h>
#include <dwrite_2.h>
#include <d2d1_2.h>
#include <string>
#include "../../DataType/Color.hpp"
#include "../../../Math/Algebra/Vector3.hpp"

namespace CS460
{
    class TextureCommon;

    class TextTextureBufferDX11
    {
    public:
        TextTextureBufferDX11();
        ~TextTextureBufferDX11();

        ID3D11ShaderResourceView* GetTextResource() const;

        bool SetUpBrush();
        bool SetUpSpriteText();
        bool SetUpTextFormats();
        bool SetUpTextureSize();
        bool SetUpVertexBuffer() const;
        bool SetUpFontTexture();
        bool SetUpTextLayout();

    protected:
        std::wstring* m_text     = nullptr;
        std::wstring* m_font     = nullptr;
        Color         m_color    = ColorDef::Pure::White;
        Real          m_dxgi_dpi = 96.0f;
        Vector3       m_position = Vector3();

        const Real DPI_RATIO = 0.1875; //3 / 16
        const Real TEX_RATIO = 512.0f;

        //component created
        IDWriteTextFormat*        m_text_format     = nullptr;
        IDWriteTextLayout*        m_text_layout     = nullptr;
        ID2D1SolidColorBrush*     m_brush           = nullptr;
        ID3D11Texture2D*          m_font_texture    = nullptr;
        ID3D11ShaderResourceView* m_font_resource   = nullptr;
        IDXGISurface*             m_texture_surface = nullptr;
        ID2D1Bitmap1*             m_texture_target  = nullptr;

        D2D1_SIZE_F m_render_target_size;

        //from system
        ID2D1DeviceContext1* m_d2d_device_context = nullptr;
        IDWriteFactory2*     m_write_factory      = nullptr;
        ID3D11Device*        m_d3d_device         = nullptr;
        ID3D11DeviceContext* m_d3d_device_context = nullptr;

        //for draw texture
        U32  m_vertex_count   = 0;
        U32  m_index_count    = 0;
        U32  m_texture_width  = 512;
        U32  m_texture_height = 512;
        Real m_font_size      = 72.0f;

        ID3D11Buffer* m_vertex_buffer = nullptr;
        ID3D11Buffer* m_index_buffer  = nullptr;
    };
}
