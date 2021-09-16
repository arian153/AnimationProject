#include "RenderTextureBufferDX11.hpp"
#include "../../Common/Buffer/RenderTextureBufferCommon.hpp"
#include "../../Common/Vertex/TextureVertexCommon.hpp"
#include "../../Common/Renderer/RendererCommon.hpp"
#include "../../DataType/Color.hpp"

namespace CS460
{
    RenderTextureBufferDX11::RenderTextureBufferDX11()
    {
    }

    RenderTextureBufferDX11::~RenderTextureBufferDX11()
    {
    }

    bool RenderTextureBufferDX11::BuildTextureBuffer(size_t width, size_t height)
    {
        if (m_shader_resource_view != nullptr)
        {
            m_shader_resource_view->Release();
            m_shader_resource_view = nullptr;
        }
        if (m_render_target_view != nullptr)
        {
            m_render_target_view->Release();
            m_render_target_view = nullptr;
        }
        if (m_render_target_texture != nullptr)
        {
            m_render_target_texture->Release();
            m_render_target_texture = nullptr;
        }
        // Initialize the render target texture description.
        D3D11_TEXTURE2D_DESC texture_desc;
        ZeroMemory(&texture_desc, sizeof(texture_desc));
        // Setup the render target texture description.
        texture_desc.Width            = static_cast<U32>(width);
        texture_desc.Height           = static_cast<U32>(height);
        texture_desc.MipLevels        = 1;
        texture_desc.ArraySize        = 1;
        texture_desc.Format           = DXGI_FORMAT_R32G32B32A32_FLOAT;
        texture_desc.SampleDesc.Count = 1;
        texture_desc.Usage            = D3D11_USAGE_DEFAULT;
        texture_desc.BindFlags        = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
        texture_desc.CPUAccessFlags   = 0;
        texture_desc.MiscFlags        = 0;
        // Create the render target texture.
        HRESULT result = m_device->CreateTexture2D(&texture_desc, nullptr, &m_render_target_texture);
        if (FAILED(result))
        {
            return false;
        }
        // Setup the description of the render target view.
        D3D11_RENDER_TARGET_VIEW_DESC render_target_view_desc;
        render_target_view_desc.Format             = texture_desc.Format;
        render_target_view_desc.ViewDimension      = D3D11_RTV_DIMENSION_TEXTURE2D;
        render_target_view_desc.Texture2D.MipSlice = 0;
        // Create the render target view.
        result = m_device->CreateRenderTargetView(m_render_target_texture, &render_target_view_desc, &m_render_target_view);
        if (FAILED(result))
        {
            return false;
        }
        // Setup the description of the shader resource view.
        D3D11_SHADER_RESOURCE_VIEW_DESC shader_resource_view_desc;
        shader_resource_view_desc.Format                    = texture_desc.Format;
        shader_resource_view_desc.ViewDimension             = D3D11_SRV_DIMENSION_TEXTURE2D;
        shader_resource_view_desc.Texture2D.MostDetailedMip = 0;
        shader_resource_view_desc.Texture2D.MipLevels       = 1;
        // Create the shader resource view.
        result = m_device->CreateShaderResourceView(m_render_target_texture, &shader_resource_view_desc, &m_shader_resource_view);
        if (FAILED(result))
        {
            return false;
        }
        return true;
    }

    bool RenderTextureBufferDX11::BuildVertexBuffer(size_t width, size_t height)
    {
        Real half_x    = static_cast<Real>(width >> 1);
        Real half_y    = static_cast<Real>(height >> 1);
        Real right     = half_x;
        Real left      = -half_x;
        Real top       = half_y;
        Real bottom    = -half_y;
        m_vertex_count = 4;
        m_index_count  = 6;
        // Create the vertex array.
        TextureVertexCommon* vertices = new TextureVertexCommon[ m_vertex_count ];
        if (vertices == nullptr)
        {
            return false;
        }
        // Create the index array.
        U32* indices = new U32[ m_index_count ];
        if (indices == nullptr)
        {
            return false;
        }
        // Load the vertex array with data.
        // First triangle.
        vertices[0] = TextureVertexCommon(left, top, 0.0f, 0.0f, 0.0f);
        vertices[1] = TextureVertexCommon(right, bottom, 0.0f, 1.0f, 1.0f);
        vertices[2] = TextureVertexCommon(left, bottom, 0.0f, 0.0f, 1.0f);
        vertices[3] = TextureVertexCommon(right, top, 0.0f, 1.0f, 0.0f);
        // Load the index array with data.
        indices[0] = 0;
        indices[1] = 1;
        indices[2] = 2;
        indices[3] = 0;
        indices[4] = 3;
        indices[5] = 1;
        // Set up the description of the vertex buffer.
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
        // Now finally create the vertex buffer.
        HRESULT result = m_device->CreateBuffer(&vertex_buffer_desc, &vertex_data, &m_vertex_buffer);
        if (FAILED(result))
        {
            return false;
        }
        // Set up the description of the index buffer.
        D3D11_BUFFER_DESC index_buffer_desc;
        index_buffer_desc.Usage               = D3D11_USAGE_DEFAULT;
        index_buffer_desc.ByteWidth           = sizeof(U32) * m_index_count;
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
        result = m_device->CreateBuffer(&index_buffer_desc, &index_data, &m_index_buffer);
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

    bool RenderTextureBufferDX11::ResizeVertexBuffer(size_t width, size_t height) const
    {
        Real half_x = static_cast<Real>(width >> 1);
        Real half_y = static_cast<Real>(height >> 1);
        Real right  = half_x;
        Real left   = -half_x;
        Real top    = half_y;
        Real bottom = -half_y;
        // Create the vertex array.
        TextureVertexCommon* vertices = new TextureVertexCommon[ m_vertex_count ];
        // Load the vertex array with data.
        vertices[0] = TextureVertexCommon(left, top, 0.0f, 0.0f, 0.0f);
        vertices[1] = TextureVertexCommon(right, bottom, 0.0f, 1.0f, 1.0f);
        vertices[2] = TextureVertexCommon(left, bottom, 0.0f, 0.0f, 1.0f);
        vertices[3] = TextureVertexCommon(right, top, 0.0f, 1.0f, 0.0f);
        // Lock the vertex buffer so it can be written to.
        D3D11_MAPPED_SUBRESOURCE mapped_resource;
        // mapping
        HRESULT result = m_device_context->Map(m_vertex_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource);
        if (FAILED(result))
        {
            return false;
        }
        // Get a pointer to the data in the vertex buffer.
        TextureVertexCommon* vertices_ptr = (TextureVertexCommon*)mapped_resource.pData;
        // Copy the data into the vertex buffer.
        memcpy(vertices_ptr, (void*)vertices, (sizeof(TextureVertexCommon) * m_vertex_count));
        // Unlock the vertex buffer.
        m_device_context->Unmap(m_vertex_buffer, 0);
        // Release the vertex array as it is no longer needed.
        delete[] vertices;
        vertices = nullptr;
        return true;
    }

    void RenderTextureBufferDX11::SetRenderTarget() const
    {
        if (m_render_target_view != nullptr)
        {
            // Bind the render target view and depth stencil buffer to the output render pipeline.
            m_device_context->OMSetRenderTargets(1, &m_render_target_view, m_renderer->GetDepthStencilView());
        }
    }

    void RenderTextureBufferDX11::ClearRenderTarget(const Color& color) const
    {
        if (m_render_target_view != nullptr)
        {
            // Setup the color to clear the buffer to.
            Real color_array[ 4 ] = {color.r, color.g, color.b, color.a};
            // Clear the back buffer.
            m_device_context->ClearRenderTargetView(m_render_target_view, color_array);
            // Clear the depth buffer.
            m_device_context->ClearDepthStencilView(m_renderer->GetDepthStencilView(), D3D11_CLEAR_DEPTH, 1.0f, 0);
        }
    }

    ID3D11ShaderResourceView* RenderTextureBufferDX11::GetTextureResource() const
    {
        return m_shader_resource_view;
    }

    ID3D11DeviceContext* RenderTextureBufferDX11::GetDeviceContext() const
    {
        return m_device_context;
    }

    RenderTextureBufferCommon::RenderTextureBufferCommon()
    {
    }

    RenderTextureBufferCommon::~RenderTextureBufferCommon()
    {
    }

    bool RenderTextureBufferCommon::Initialize(RendererCommon* renderer, size_t width, size_t height)
    {
        m_device             = renderer->GetDevice();
        m_device_context     = renderer->GetDeviceContext();
        m_renderer           = renderer;
        if (BuildTextureBuffer(width, height) == false)
        {
            return false;
        }
        return BuildVertexBuffer(width, height);
    }

    void RenderTextureBufferCommon::Render() const
    {
        // Set vertex buffer stride and offset.
        U32 stride = sizeof(TextureVertexCommon);
        U32 offset = 0;
        // Set the vertex buffer to active in the input assembler so it can be rendered.
        m_device_context->IASetVertexBuffers(0, 1, &m_vertex_buffer, &stride, &offset);
        // Set the index buffer to active in the input assembler so it can be rendered.
        m_device_context->IASetIndexBuffer(m_index_buffer, DXGI_FORMAT_R32_UINT, 0);
        // Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
        m_device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    }

    void RenderTextureBufferCommon::Shutdown()
    {
        if (m_shader_resource_view != nullptr)
        {
            m_shader_resource_view->Release();
            m_shader_resource_view = nullptr;
        }
        if (m_render_target_view != nullptr)
        {
            m_render_target_view->Release();
            m_render_target_view = nullptr;
        }
        if (m_render_target_texture != nullptr)
        {
            m_render_target_texture->Release();
            m_render_target_texture = nullptr;
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
    }

    bool RenderTextureBufferCommon::OnResize(size_t width, size_t height)
    {
        if (BuildTextureBuffer(width, height) == false)
        {
            return false;
        }
        return ResizeVertexBuffer(width, height);
    }

    U32 RenderTextureBufferCommon::GetIndexCount() const
    {
        return m_index_count;
    }
}
