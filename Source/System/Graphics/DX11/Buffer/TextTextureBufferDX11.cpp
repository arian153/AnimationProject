#pragma warning( disable : 6387)

#include "TextTextureBufferDX11.hpp"
#include "../../Common/Renderer/RendererCommon.hpp"
#include "../../Common/Buffer/TextTextureBufferCommon.hpp"
#include "../../Common/Vertex/TextureVertexCommon.hpp"
#include "../../../Core/Utility/CoreUtility.hpp"

namespace CS460
{
    TextTextureBufferDX11::TextTextureBufferDX11()
        : m_render_target_size()
    {
    }

    TextTextureBufferDX11::~TextTextureBufferDX11()
    {
    }

    ID3D11ShaderResourceView* TextTextureBufferDX11::GetTextResource() const
    {
        return m_font_resource;
    }

    bool TextTextureBufferDX11::SetUpBrush()
    {
        HRESULT result = m_d2d_device_context->CreateSolidColorBrush(D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f), &m_brush);
        if (FAILED(result))
        {
            return false;
        }
        return true;
    }

    bool TextTextureBufferDX11::SetUpSpriteText()
    {
        m_vertex_count                = 4;
        m_index_count                 = 12;
        TextureVertexCommon* vertices = new TextureVertexCommon[ m_vertex_count ];
        U32*                 indices  = new U32[ m_index_count ];
        // Initialize vertex array
        vertices[0] = TextureVertexCommon(-1.0f, 1.0f, 0.0f, 0.0f, 0.0f); // Top left.
        vertices[1] = TextureVertexCommon(1.0f, -1.0f, 0.0f, 1.0f, 1.0f);  // Bottom right.
        vertices[2] = TextureVertexCommon(-1.0f, -1.0f, 0.0f, 0.0f, 1.0f); // Bottom left.
        vertices[3] = TextureVertexCommon(1.0f, 1.0f, 0.0f, 1.0f, 0.0f);  // Top right.
        // Load the index array with data.
        // First triangle.
        indices[0] = 0; // Top left.
        indices[1] = 1; // Bottom right.
        indices[2] = 2; // Bottom left.
        // Second triangle.
        indices[3] = 0; // Top left.
        indices[4] = 3; //  Top right.
        indices[5] = 1; // Bottom right.
        // third triangle.
        indices[6] = 0; // Top left.
        indices[7] = 1; // Bottom right.
        indices[8] = 2; // Bottom left.
        // fourth triangle.
        indices[9]  = 0; // Top left.
        indices[10] = 3; //  Top right.
        indices[11] = 1; // Bottom right.
        // Set up the description of the static vertex buffer.
        D3D11_BUFFER_DESC vertex_buffer_desc;
        vertex_buffer_desc.Usage               = D3D11_USAGE_DYNAMIC;
        vertex_buffer_desc.ByteWidth           = sizeof(TextureVertexCommon) * m_vertex_count;
        vertex_buffer_desc.BindFlags           = D3D11_BIND_VERTEX_BUFFER;
        vertex_buffer_desc.CPUAccessFlags      = D3D11_CPU_ACCESS_WRITE;
        vertex_buffer_desc.MiscFlags           = 0;
        vertex_buffer_desc.StructureByteStride = 0;
        // Give the sub resource structure a pointer to the vertex data.
        D3D11_SUBRESOURCE_DATA vertex_data;
        vertex_data.pSysMem          = vertices;
        vertex_data.SysMemPitch      = 0;
        vertex_data.SysMemSlicePitch = 0;
        // Now create the vertex buffer.
        HRESULT result = m_d3d_device->CreateBuffer(&vertex_buffer_desc, &vertex_data, &m_vertex_buffer);
        if (FAILED(result))
        {
            return false;
        }
        // Set up the description of the static index buffer.
        D3D11_BUFFER_DESC index_buffer_desc;
        index_buffer_desc.Usage               = D3D11_USAGE_DEFAULT;
        index_buffer_desc.ByteWidth           = sizeof(unsigned long) * m_index_count;
        index_buffer_desc.BindFlags           = D3D11_BIND_INDEX_BUFFER;
        index_buffer_desc.CPUAccessFlags      = 0;
        index_buffer_desc.MiscFlags           = 0;
        index_buffer_desc.StructureByteStride = 0;
        // Give the sub resource structure a pointer to the index data.
        D3D11_SUBRESOURCE_DATA index_data;
        index_data.pSysMem          = indices;
        index_data.SysMemPitch      = 0;
        index_data.SysMemSlicePitch = 0;
        // Create the index buffer.
        result = m_d3d_device->CreateBuffer(&index_buffer_desc, &index_data, &m_index_buffer);
        if (FAILED(result))
        {
            return false;
        }
        delete[] vertices;
        vertices = nullptr;
        delete[] indices;
        indices = nullptr;
        return true;
    }

    bool TextTextureBufferDX11::SetUpTextFormats()
    {
        if (m_text_format != nullptr)
        {
            m_text_format->Release();
            m_text_format = nullptr;
        }
        HRESULT result = m_write_factory->CreateTextFormat(
                                                           m_font->c_str(),
                                                           nullptr,
                                                           DWRITE_FONT_WEIGHT_NORMAL,
                                                           DWRITE_FONT_STYLE_NORMAL,
                                                           DWRITE_FONT_STRETCH_NORMAL,
                                                           m_font_size,
                                                           L"en-GB",
                                                           &m_text_format
                                                          );
        if (FAILED(result))
        {
            return false;
        }
        result = m_text_format->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
        if (FAILED(result))
        {
            return false;
        }
        result = m_text_format->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
        if (FAILED(result))
        {
            return false;
        }
        return true;
    }

    bool TextTextureBufferDX11::SetUpTextureSize()
    {
        U32         length;
        std::size_t found  = m_text->find_first_of(L"\n");
        if (found != std::string::npos)
        {
            length = static_cast<U32>(found);
        }
        else
        {
            length = static_cast<U32>(m_text->length());
        }
        m_texture_width     = (length + 1) * static_cast<U32>(m_font_size);
        U32 number_of_lines = static_cast<U32>(std::count(m_text->begin(), m_text->end(), '\n'));
        m_texture_height    = (number_of_lines + 1) * static_cast<U32>(m_font_size + (m_font_size * 0.5f));
        return true;
    }

    bool TextTextureBufferDX11::SetUpVertexBuffer() const
    {
        Real width_scale  = static_cast<Real>(m_texture_width) / TEX_RATIO;
        Real height_scale = static_cast<Real>(m_texture_height) / TEX_RATIO;
        Real left         = -width_scale;
        Real right        = width_scale;
        Real top          = height_scale;
        Real bottom       = -height_scale;
        // Create the vertex array.
        TextureVertexCommon* vertices = new TextureVertexCommon[ m_vertex_count ];
        // Load the vertex array with data.
        vertices[0] = TextureVertexCommon(left, top, 0.0f, 0.0f, 0.0f); // Top left.
        vertices[1] = TextureVertexCommon(right, bottom, 0.0f, 1.0f, 1.0f);  // Bottom right.
        vertices[2] = TextureVertexCommon(left, bottom, 0.0f, 0.0f, 1.0f); // Bottom left.
        vertices[3] = TextureVertexCommon(right, top, 0.0f, 1.0f, 0.0f);  // Top right.
        // Lock the vertex buffer so it can be written to.
        D3D11_MAPPED_SUBRESOURCE mapped_resource;
        // mapping
        HRESULT result = m_d3d_device_context->Map(m_vertex_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource);
        if (FAILED(result))
        {
            return false;
        }
        // Get a pointer to the data in the vertex buffer.
        TextureVertexCommon* vertices_ptr = (TextureVertexCommon*)mapped_resource.pData;
        // Copy the data into the vertex buffer.
        memcpy(vertices_ptr, (void*)vertices, (sizeof(TextureVertexCommon) * m_vertex_count));
        // Unlock the vertex buffer.
        m_d3d_device_context->Unmap(m_vertex_buffer, 0);
        // Release the vertex array as it is no longer needed.
        delete[] vertices;
        vertices = nullptr;
        return true;
    }

    bool TextTextureBufferDX11::SetUpFontTexture()
    {
        if (m_font_texture != nullptr)
        {
            m_font_texture->Release();
            m_font_texture = nullptr;
        }
        if (m_font_resource != nullptr)
        {
            m_font_resource->Release();
            m_font_resource = nullptr;
        }
        if (m_texture_target != nullptr)
        {
            m_texture_target->Release();
            m_texture_target = nullptr;
        }
        CD3D11_TEXTURE2D_DESC texture_desc(
                                           DXGI_FORMAT_B8G8R8A8_UNORM,
                                           m_texture_width,        // Width
                                           m_texture_height,        // Height
                                           1,          // MipLevels
                                           1,          // ArraySize
                                           D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET
                                          );
        HRESULT result = m_d3d_device->CreateTexture2D(&texture_desc, nullptr, &m_font_texture);
        if (FAILED(result))
        {
            return false;
        }
        CD3D11_SHADER_RESOURCE_VIEW_DESC resource_desc(m_font_texture, D3D11_SRV_DIMENSION_TEXTURE2D);
        result = m_d3d_device->CreateShaderResourceView(m_font_texture, &resource_desc, &m_font_resource);
        if (FAILED(result))
        {
            return false;
        }
        m_d2d_device_context->SetDpi(m_dxgi_dpi, m_dxgi_dpi);
        D2D1_BITMAP_PROPERTIES1 bitmap_properties = D2D1::BitmapProperties1(
                                                                            D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
                                                                            D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED),
                                                                            m_dxgi_dpi, m_dxgi_dpi);
        IDXGISurface* texture_surface = nullptr;
        m_font_texture->QueryInterface(__uuidof(IDXGISurface), (void**)&texture_surface);
        m_d2d_device_context->CreateBitmapFromDxgiSurface(texture_surface, &bitmap_properties, &m_texture_target);
        if (texture_surface != nullptr)
        {
            texture_surface->Release();
            texture_surface = nullptr;
        }
        return true;
    }

    bool TextTextureBufferDX11::SetUpTextLayout()
    {
        if (m_text_layout != nullptr)
        {
            m_text_layout->Release();
            m_text_layout = nullptr;
        }
        HRESULT result = m_write_factory->CreateTextLayout(
                                                           m_text->c_str(),      // The string to be laid out and formatted.
                                                           (UINT32)m_text->length(),  // The length of the string.
                                                           m_text_format,  // The text format to apply to the string (contains font information, etc).
                                                           (Real)m_texture_width,         // The width of the layout box.
                                                           (Real)m_texture_height,        // The height of the layout box.
                                                           &m_text_layout  // The IDWriteTextLayout interface pointer.
                                                          );
        if (FAILED(result))
        {
            return false;
        }
        return true;
    }

    TextTextureBufferCommon::TextTextureBufferCommon()
    {
    }

    TextTextureBufferCommon::~TextTextureBufferCommon()
    {
    }

    bool TextTextureBufferCommon::Initialize(RendererCommon* renderer)
    {
        m_d3d_device         = renderer->GetDevice();
        m_d3d_device_context = renderer->GetDeviceContext();
        m_d2d_device_context = renderer->GetD2DDeviceContext();
        m_write_factory      = renderer->GetDWWriteFactory();
        m_font               = new std::wstring(L"courier new");
        m_text               = new std::wstring(L"");
        bool result          = SetUpBrush();
        if (result == false)
        {
            return false;
        }
        result = SetUpSpriteText();
        if (result == false)
        {
            return false;
        }
        result = Update();
        if (result == false)
        {
            return false;
        }
        return true;
    }

    bool TextTextureBufferCommon::Update()
    {
        bool result = SetUpTextFormats();
        if (result == false)
        {
            return false;
        }
        result = SetUpTextureSize();
        if (result == false)
        {
            return false;
        }
        result = SetUpVertexBuffer();
        if (result == false)
        {
            return false;
        }
        result = SetUpFontTexture();
        if (result == false)
        {
            return false;
        }
        result = SetUpTextLayout();
        if (result == false)
        {
            return false;
        }
        return true;
    }

    void TextTextureBufferCommon::Render()
    {
        m_d2d_device_context->SetTarget(m_texture_target);
        m_render_target_size = m_d2d_device_context->GetSize();
        m_d2d_device_context->Clear(D2D1::ColorF(0.0f, 0.0f, 0.0f, 0.0f));
        m_d2d_device_context->SetTransform(D2D1::IdentityMatrix());
        m_d2d_device_context->DrawTextLayout(D2D1::Point2F(0.0f, 0.f), m_text_layout, m_brush);
        // Set vertex buffer stride and offset.
        unsigned int stride = sizeof(TextureVertexCommon);
        unsigned int offset = 0;
        // Set the vertex buffer to active in the input assembler so it can be rendered.
        m_d3d_device_context->IASetVertexBuffers(0, 1, &m_vertex_buffer, &stride, &offset);
        // Set the index buffer to active in the input assembler so it can be rendered.
        m_d3d_device_context->IASetIndexBuffer(m_index_buffer, DXGI_FORMAT_R32_UINT, 0);
        // Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
        m_d3d_device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    }

    void TextTextureBufferCommon::Shutdown()
    {
        if (m_brush != nullptr)
        {
            m_brush->Release();
            m_brush = nullptr;
        }
        if (m_text_format != nullptr)
        {
            m_text_format->Release();
            m_text_format = nullptr;
        }
        if (m_text_layout != nullptr)
        {
            m_text_layout->Release();
            m_text_layout = nullptr;
        }
        if (m_font_texture != nullptr)
        {
            m_font_texture->Release();
            m_font_texture = nullptr;
        }
        if (m_font_resource != nullptr)
        {
            m_font_resource->Release();
            m_font_resource = nullptr;
        }
        if (m_texture_surface != nullptr)
        {
            m_texture_surface->Release();
            m_texture_surface = nullptr;
        }
        if (m_texture_target != nullptr)
        {
            m_texture_target->Release();
            m_texture_target = nullptr;
        }
        if (m_vertex_buffer != nullptr)
        {
            m_vertex_buffer->Release();
            m_vertex_buffer = nullptr;
        }
        if (m_index_buffer != nullptr)
        {
            m_index_buffer->Release();
            m_index_buffer = nullptr;
        }
        if (m_text != nullptr)
        {
            m_text->clear();
            delete m_text;
            m_text = nullptr;
        }
        if (m_font != nullptr)
        {
            m_font->clear();
            delete m_font;
            m_font = nullptr;
        }
    }

    void TextTextureBufferCommon::SetText(const std::string& string) const
    {
        m_text->reserve(string.size());
        m_text->assign(ToWString(string));
    }

    void TextTextureBufferCommon::SetText(const std::wstring& string) const
    {
        m_text->reserve(string.size());
        m_text->assign(string);
    }

    void TextTextureBufferCommon::SetFont(const std::wstring& font) const
    {
        m_font->assign(font);
    }

    void TextTextureBufferCommon::SetColor(Color color)
    {
        m_color = color;
    }

    U32 TextTextureBufferCommon::GetIndexCount() const
    {
        return m_index_count;
    }

    U32 TextTextureBufferCommon::GetVertexCount() const
    {
        return m_vertex_count;
    }

    Color TextTextureBufferCommon::GetColor() const
    {
        return m_color;
    }
}
