#include "DeferredBufferDX11.hpp"
#include "../../../Core/Utility/CoreDef.hpp"
#include "../../../Math/Utility/MathDef.hpp"
#include "../../DataType/Color.hpp"
#include "../../Common/Buffer/DeferredBufferCommon.hpp"
#include "../../Common/Renderer/RendererCommon.hpp"
#include "../../Common/Vertex/TextureVertexCommon.hpp"

namespace CS460
{
    DeferredBufferDX11::DeferredBufferDX11()
        : m_render_target_texture_array{}, m_render_target_view_array{}, m_shader_resource_view_array{}, m_viewport()
    {
        for (U32 i = 0; i < BUFFER_COUNT; i++)
        {
            m_render_target_texture_array[i] = nullptr;
            m_render_target_view_array[i]    = nullptr;
            m_shader_resource_view_array[i]  = nullptr;
        }
    }

    DeferredBufferDX11::~DeferredBufferDX11()
    {
    }

    void DeferredBufferDX11::SetDevice(ID3D11Device* device)
    {
        m_device = device;
    }

    void DeferredBufferDX11::SetDeviceContext(ID3D11DeviceContext* device_context)
    {
        m_device_context = device_context;
    }

    bool DeferredBufferDX11::BuildBuffer(U32 texture_width, U32 texture_height)
    {
        // Store the width and height of the render texture.
        m_texture_width  = texture_width;
        m_texture_height = texture_height;
        // Initialize the render target texture description.
        D3D11_TEXTURE2D_DESC texture_desc;
        ZeroMemory(&texture_desc, sizeof(texture_desc));
        // Setup the render target texture description.
        texture_desc.Width            = texture_width;
        texture_desc.Height           = texture_height;
        texture_desc.MipLevels        = 1;
        texture_desc.ArraySize        = 1;
        texture_desc.Format           = DXGI_FORMAT_R32G32B32A32_FLOAT;
        texture_desc.SampleDesc.Count = 1;
        texture_desc.Usage            = D3D11_USAGE_DEFAULT;
        texture_desc.BindFlags        = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
        texture_desc.CPUAccessFlags   = 0;
        texture_desc.MiscFlags        = 0;
        // Create the render target textures.
        HRESULT result;
        for (U32 i = 0; i < BUFFER_COUNT; i++)
        {
            result = m_device->CreateTexture2D(&texture_desc, nullptr, &m_render_target_texture_array[i]);
            if (FAILED(result))
            {
                return false;
            }
        }
        // Setup the description of the render target view.
        D3D11_RENDER_TARGET_VIEW_DESC render_target_view_desc;
        render_target_view_desc.Format             = texture_desc.Format;
        render_target_view_desc.ViewDimension      = D3D11_RTV_DIMENSION_TEXTURE2D;
        render_target_view_desc.Texture2D.MipSlice = 0;
        // Create the render target views.
        for (U32 i = 0; i < BUFFER_COUNT; i++)
        {
            result = m_device->CreateRenderTargetView(m_render_target_texture_array[i], &render_target_view_desc, &m_render_target_view_array[i]);
            if (FAILED(result))
            {
                return false;
            }
        }
        // Setup the description of the shader resource view.
        D3D11_SHADER_RESOURCE_VIEW_DESC shader_resource_view_desc;
        shader_resource_view_desc.Format                    = texture_desc.Format;
        shader_resource_view_desc.ViewDimension             = D3D11_SRV_DIMENSION_TEXTURE2D;
        shader_resource_view_desc.Texture2D.MostDetailedMip = 0;
        shader_resource_view_desc.Texture2D.MipLevels       = 1;
        // Create the shader resource views.
        for (U32 i = 0; i < BUFFER_COUNT; i++)
        {
            result = m_device->CreateShaderResourceView(m_render_target_texture_array[i], &shader_resource_view_desc, &m_shader_resource_view_array[i]);
            if (FAILED(result))
            {
                return false;
            }
        }
        // Initialize the description of the depth buffer.
        D3D11_TEXTURE2D_DESC depth_buffer_desc;
        ZeroMemory(&depth_buffer_desc, sizeof(depth_buffer_desc));
        // Set up the description of the depth buffer.
        depth_buffer_desc.Width              = texture_width;
        depth_buffer_desc.Height             = texture_height;
        depth_buffer_desc.MipLevels          = 1;
        depth_buffer_desc.ArraySize          = 1;
        depth_buffer_desc.Format             = DXGI_FORMAT_D24_UNORM_S8_UINT;
        depth_buffer_desc.SampleDesc.Count   = 1;
        depth_buffer_desc.SampleDesc.Quality = 0;
        depth_buffer_desc.Usage              = D3D11_USAGE_DEFAULT;
        depth_buffer_desc.BindFlags          = D3D11_BIND_DEPTH_STENCIL;
        depth_buffer_desc.CPUAccessFlags     = 0;
        depth_buffer_desc.MiscFlags          = 0;
        // Create the texture for the depth buffer using the filled out description.
        result = m_device->CreateTexture2D(&depth_buffer_desc, nullptr, &m_depth_stencil_buffer);
        if (FAILED(result))
        {
            return false;
        }
        // Initialize the depth stencil view description.
        D3D11_DEPTH_STENCIL_VIEW_DESC depth_stencil_view_desc;
        ZeroMemory(&depth_stencil_view_desc, sizeof(depth_stencil_view_desc));
        // Set up the depth stencil view description.
        depth_stencil_view_desc.Format             = DXGI_FORMAT_D24_UNORM_S8_UINT;
        depth_stencil_view_desc.ViewDimension      = D3D11_DSV_DIMENSION_TEXTURE2D;
        depth_stencil_view_desc.Texture2D.MipSlice = 0;
        // Create the depth stencil view.
        result = m_device->CreateDepthStencilView(m_depth_stencil_buffer, &depth_stencil_view_desc, &m_depth_stencil_view);
        if (FAILED(result))
        {
            return false;
        }
        // Setup the viewport for rendering.
        m_viewport.Width    = (Real)texture_width;
        m_viewport.Height   = (Real)texture_height;
        m_viewport.MinDepth = 0.0f;
        m_viewport.MaxDepth = 1.0f;
        m_viewport.TopLeftX = 0.0f;
        m_viewport.TopLeftY = 0.0f;
        return true;
    }

    bool DeferredBufferDX11::BuildVertexBuffer(Real left, Real right, Real top, Real bottom)
    {
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

    bool DeferredBufferDX11::ResizeVertexBuffer(Real left, Real right, Real top, Real bottom) const
    {
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

    U32 DeferredBufferDX11::GetIndexCount() const
    {
        return m_index_count;
    }

    ID3D11ShaderResourceView* DeferredBufferDX11::GetShaderResourceView(U32 view)
    {
        return m_shader_resource_view_array[view];
    }

    DeferredBufferCommon::DeferredBufferCommon()
    {
    }

    DeferredBufferCommon::~DeferredBufferCommon()
    {
    }

    bool DeferredBufferCommon::Initialize(RendererCommon* renderer, U32 texture_width, U32 texture_height)
    {
        SetDevice(renderer->GetDevice());
        SetDeviceContext(renderer->GetDeviceContext());
        bool result = BuildBuffer(texture_width, texture_height);
        if (result == false)
        {
            return false;
        }
        Real half_x = static_cast<Real>(texture_width >> 1);
        Real half_y = static_cast<Real>(texture_height >> 1);
        Real left   = -half_x;
        Real right  = half_x;
        Real top    = half_y;
        Real bottom = -half_y;
        return BuildVertexBuffer(left, right, top, bottom);
    }

    void DeferredBufferCommon::Render() const
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

    void DeferredBufferCommon::Shutdown()
    {
        if (m_depth_stencil_view)
        {
            m_depth_stencil_view->Release();
            m_depth_stencil_view = nullptr;
        }
        if (m_depth_stencil_buffer)
        {
            m_depth_stencil_buffer->Release();
            m_depth_stencil_buffer = nullptr;
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
        for (U32 i = 0; i < BUFFER_COUNT; i++)
        {
            if (m_shader_resource_view_array[i])
            {
                m_shader_resource_view_array[i]->Release();
                m_shader_resource_view_array[i] = nullptr;
            }
            if (m_render_target_view_array[i])
            {
                m_render_target_view_array[i]->Release();
                m_render_target_view_array[i] = nullptr;
            }
            if (m_render_target_texture_array[i])
            {
                m_render_target_texture_array[i]->Release();
                m_render_target_texture_array[i] = nullptr;
            }
        }
    }

    bool DeferredBufferCommon::OnResize(U32 texture_width, U32 texture_height)
    {
        ID3D11ShaderResourceView* null[ BUFFER_COUNT ] = {nullptr};
        m_device_context->PSSetShaderResources(0, BUFFER_COUNT, null);
        if (m_depth_stencil_view)
        {
            m_depth_stencil_view->Release();
            m_depth_stencil_view = nullptr;
        }
        if (m_depth_stencil_buffer)
        {
            m_depth_stencil_buffer->Release();
            m_depth_stencil_buffer = nullptr;
        }
        for (U32 i = 0; i < BUFFER_COUNT; i++)
        {
            if (m_shader_resource_view_array[i])
            {
                m_shader_resource_view_array[i]->Release();
                m_shader_resource_view_array[i] = nullptr;
            }
            if (m_render_target_view_array[i])
            {
                m_render_target_view_array[i]->Release();
                m_render_target_view_array[i] = nullptr;
            }
            if (m_render_target_texture_array[i])
            {
                m_render_target_texture_array[i]->Release();
                m_render_target_texture_array[i] = nullptr;
            }
        }
        bool result = BuildBuffer(texture_width, texture_height);
        if (result == false)
        {
            return false;
        }
        Real half_x = static_cast<Real>(texture_width >> 1);
        Real half_y = static_cast<Real>(texture_height >> 1);
        Real left   = -half_x;
        Real right  = half_x;
        Real top    = half_y;
        Real bottom = -half_y;
        return ResizeVertexBuffer(left, right, top, bottom);
    }

    void DeferredBufferCommon::SetRenderTargets() const
    {
        if (m_render_target_texture_array[0] == nullptr)
        {
            return;
        }
        //release texture
        ID3D11ShaderResourceView* null[ BUFFER_COUNT ] = {nullptr};
        m_device_context->PSSetShaderResources(0, BUFFER_COUNT, null);
        // Bind the render target view array and depth stencil buffer to the output render pipeline.
        m_device_context->OMSetRenderTargets(BUFFER_COUNT, m_render_target_view_array, m_depth_stencil_view);
        // Set the viewport.
        m_device_context->RSSetViewports(1, &m_viewport);
    }

    void DeferredBufferCommon::ClearRenderTargets(const Color& color)
    {
        if (m_render_target_texture_array[0] == nullptr)
        {
            return;
        }
        Real color_array[ 4 ];
        // Setup the color to clear the buffer to.
        color_array[0] = color.r;
        color_array[1] = color.g;
        color_array[2] = color.b;
        color_array[3] = color.a;
        // Clear the render target buffers.
        for (U32 i = 0; i < BUFFER_COUNT; i++)
        {
            m_device_context->ClearRenderTargetView(m_render_target_view_array[i], color_array);
        }
        // Clear the depth buffer.
        m_device_context->ClearDepthStencilView(m_depth_stencil_view, D3D11_CLEAR_DEPTH, 1.0f, 0);
    }
}
