#include "VertexBufferDX11.hpp"

#include "../../Common/Buffer2/InstanceBufferCommon.hpp"
#include "../../Common/Buffer2/VertexBufferCommon.hpp"
#include "../../Common/Renderer/RendererCommon.hpp"

namespace CS460
{
    VertexBufferDX11::VertexBufferDX11()
    {
    }

    VertexBufferDX11::~VertexBufferDX11()
    {
    }

    VertexBufferCommon::VertexBufferCommon()
    {
    }

    VertexBufferCommon::~VertexBufferCommon()
    {
    }

    bool VertexBufferCommon::Init(RendererCommon* renderer, const std::vector<ColorVertexCommon>& vertices, bool is_dynamic)
    {
        if (m_vertex_buffer != nullptr)
        {
            m_vertex_buffer->Release();
            m_vertex_buffer = nullptr;
        }

        // Set up the description of the static vertex buffer.
        D3D11_BUFFER_DESC vertex_buffer_desc;
        vertex_buffer_desc.Usage               = is_dynamic ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
        vertex_buffer_desc.ByteWidth           = sizeof(ColorVertexCommon) * static_cast<U32>(vertices.size());
        vertex_buffer_desc.BindFlags           = D3D11_BIND_VERTEX_BUFFER;
        vertex_buffer_desc.CPUAccessFlags      = is_dynamic ? D3D11_CPU_ACCESS_WRITE : 0;
        vertex_buffer_desc.MiscFlags           = 0;
        vertex_buffer_desc.StructureByteStride = 0;

        // Give the sub resource structure a pointer to the vertex data.
        D3D11_SUBRESOURCE_DATA vertex_data;
        vertex_data.pSysMem          = vertices.data();
        vertex_data.SysMemPitch      = 0;
        vertex_data.SysMemSlicePitch = 0;
        // Now create the vertex buffer.
        HRESULT result = renderer->GetDevice()->CreateBuffer(&vertex_buffer_desc, &vertex_data, &m_vertex_buffer);
        if (FAILED(result))
            return false;

        m_device_context = renderer->GetDeviceContext();
        m_b_dynamic      = is_dynamic;
        m_stride         = (U32)sizeof(ColorVertexCommon);
        return true;
    }

    bool VertexBufferCommon::Init(RendererCommon* renderer, const std::vector<TextureVertexCommon>& vertices, bool is_dynamic)
    {
        if (m_vertex_buffer != nullptr)
        {
            m_vertex_buffer->Release();
            m_vertex_buffer = nullptr;
        }
        // Set up the description of the static vertex buffer.
        D3D11_BUFFER_DESC vertex_buffer_desc;
        vertex_buffer_desc.Usage               = is_dynamic ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
        vertex_buffer_desc.ByteWidth           = sizeof(TextureVertexCommon) * static_cast<U32>(vertices.size());
        vertex_buffer_desc.BindFlags           = D3D11_BIND_VERTEX_BUFFER;
        vertex_buffer_desc.CPUAccessFlags      = is_dynamic ? D3D11_CPU_ACCESS_WRITE : 0;
        vertex_buffer_desc.MiscFlags           = 0;
        vertex_buffer_desc.StructureByteStride = 0;
        // Give the sub resource structure a pointer to the vertex data.
        D3D11_SUBRESOURCE_DATA vertex_data;
        vertex_data.pSysMem          = vertices.data();
        vertex_data.SysMemPitch      = 0;
        vertex_data.SysMemSlicePitch = 0;
        // Now create the vertex buffer.
        HRESULT result = renderer->GetDevice()->CreateBuffer(&vertex_buffer_desc, &vertex_data, &m_vertex_buffer);
        if (FAILED(result))
            return false;

        m_device_context = renderer->GetDeviceContext();
        m_b_dynamic      = is_dynamic;
        m_stride         = (U32)sizeof(TextureVertexCommon);
        return true;
    }

    bool VertexBufferCommon::Init(RendererCommon* renderer, const std::vector<VertexCommon>& vertices, bool is_dynamic)
    {
        if (m_vertex_buffer != nullptr)
        {
            m_vertex_buffer->Release();
            m_vertex_buffer = nullptr;
        }
        // Set up the description of the static vertex buffer.
        D3D11_BUFFER_DESC vertex_buffer_desc;
        vertex_buffer_desc.Usage               = is_dynamic ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
        vertex_buffer_desc.ByteWidth           = sizeof(VertexCommon) * static_cast<U32>(vertices.size());
        vertex_buffer_desc.BindFlags           = D3D11_BIND_VERTEX_BUFFER;
        vertex_buffer_desc.CPUAccessFlags      = is_dynamic ? D3D11_CPU_ACCESS_WRITE : 0;
        vertex_buffer_desc.MiscFlags           = 0;
        vertex_buffer_desc.StructureByteStride = 0;
        // Give the sub resource structure a pointer to the vertex data.
        D3D11_SUBRESOURCE_DATA vertex_data;
        vertex_data.pSysMem          = vertices.data();
        vertex_data.SysMemPitch      = 0;
        vertex_data.SysMemSlicePitch = 0;
        // Now create the vertex buffer.
        HRESULT result = renderer->GetDevice()->CreateBuffer(&vertex_buffer_desc, &vertex_data, &m_vertex_buffer);
        if (FAILED(result))
            return false;

        m_device_context = renderer->GetDeviceContext();
        m_b_dynamic      = is_dynamic;
        m_stride         = (U32)sizeof(VertexCommon);
        return true;
    }

    bool VertexBufferCommon::Init(RendererCommon* renderer, const std::vector<NormalVertexCommon>& vertices, bool is_dynamic)
    {
        if (m_vertex_buffer != nullptr)
        {
            m_vertex_buffer->Release();
            m_vertex_buffer = nullptr;
        }
        // Set up the description of the static vertex buffer.
        D3D11_BUFFER_DESC vertex_buffer_desc;
        vertex_buffer_desc.Usage               = is_dynamic ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
        vertex_buffer_desc.ByteWidth           = sizeof(NormalVertexCommon) * static_cast<U32>(vertices.size());
        vertex_buffer_desc.BindFlags           = D3D11_BIND_VERTEX_BUFFER;
        vertex_buffer_desc.CPUAccessFlags      = is_dynamic ? D3D11_CPU_ACCESS_WRITE : 0;
        vertex_buffer_desc.MiscFlags           = 0;
        vertex_buffer_desc.StructureByteStride = 0;
        // Give the sub resource structure a pointer to the vertex data.
        D3D11_SUBRESOURCE_DATA vertex_data;
        vertex_data.pSysMem          = vertices.data();
        vertex_data.SysMemPitch      = 0;
        vertex_data.SysMemSlicePitch = 0;
        // Now create the vertex buffer.
        HRESULT result = renderer->GetDevice()->CreateBuffer(&vertex_buffer_desc, &vertex_data, &m_vertex_buffer);
        if (FAILED(result))
            return false;

        m_device_context = renderer->GetDeviceContext();
        m_b_dynamic      = is_dynamic;
        m_stride         = (U32)sizeof(NormalVertexCommon);
        return true;
    }

    bool VertexBufferCommon::Init(RendererCommon* renderer, const std::vector<Vector3>& vertices, bool is_dynamic)
    {
        if (m_vertex_buffer != nullptr)
        {
            m_vertex_buffer->Release();
            m_vertex_buffer = nullptr;
        }
        // Set up the description of the static vertex buffer.
        D3D11_BUFFER_DESC vertex_buffer_desc;
        vertex_buffer_desc.Usage               = is_dynamic ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
        vertex_buffer_desc.ByteWidth           = sizeof(Vector3) * static_cast<U32>(vertices.size());
        vertex_buffer_desc.BindFlags           = D3D11_BIND_VERTEX_BUFFER;
        vertex_buffer_desc.CPUAccessFlags      = is_dynamic ? D3D11_CPU_ACCESS_WRITE : 0;
        vertex_buffer_desc.MiscFlags           = 0;
        vertex_buffer_desc.StructureByteStride = 0;
        // Give the sub resource structure a pointer to the vertex data.
        D3D11_SUBRESOURCE_DATA vertex_data;
        vertex_data.pSysMem          = vertices.data();
        vertex_data.SysMemPitch      = 0;
        vertex_data.SysMemSlicePitch = 0;
        // Now create the vertex buffer.
        HRESULT result = renderer->GetDevice()->CreateBuffer(&vertex_buffer_desc, &vertex_data, &m_vertex_buffer);
        if (FAILED(result))
            return false;

        m_device_context = renderer->GetDeviceContext();
        m_b_dynamic      = is_dynamic;
        m_stride         = (U32)sizeof(Vector3);
        return true;
    }

    void VertexBufferCommon::Update(const std::vector<ColorVertexCommon>& vertices) const
    {
        if (!vertices.empty() && m_b_dynamic)
        {
            // Lock the vertex buffer so it can be written to.
            D3D11_MAPPED_SUBRESOURCE mapped_resource;
            // mapping
            HRESULT result = m_device_context->Map(m_vertex_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource);
            if (FAILED(result))
            {
                return;
            }
            // Get a pointer to the data in the vertex buffer.
            ColorVertexCommon* vertices_ptr = (ColorVertexCommon*)mapped_resource.pData;
            // Copy the data into the vertex buffer.
            memcpy(vertices_ptr, (void*)vertices.data(), sizeof(ColorVertexCommon) * static_cast<U32>(vertices.size()));
            // Unlock the vertex buffer.
            m_device_context->Unmap(m_vertex_buffer, 0);
        }
    }

    void VertexBufferCommon::Update(const std::vector<TextureVertexCommon>& vertices) const
    {
        if (!vertices.empty() && m_b_dynamic)
        {
            // Lock the vertex buffer so it can be written to.
            D3D11_MAPPED_SUBRESOURCE mapped_resource;
            // mapping
            HRESULT result = m_device_context->Map(m_vertex_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource);
            if (FAILED(result))
            {
                return;
            }
            // Get a pointer to the data in the vertex buffer.
            TextureVertexCommon* vertices_ptr = (TextureVertexCommon*)mapped_resource.pData;
            // Copy the data into the vertex buffer.
            memcpy(vertices_ptr, (void*)vertices.data(), sizeof(TextureVertexCommon) * static_cast<U32>(vertices.size()));
            // Unlock the vertex buffer.
            m_device_context->Unmap(m_vertex_buffer, 0);
        }
    }

    void VertexBufferCommon::Update(const std::vector<VertexCommon>& vertices) const
    {
        if (!vertices.empty() && m_b_dynamic)
        {
            // Lock the vertex buffer so it can be written to.
            D3D11_MAPPED_SUBRESOURCE mapped_resource;
            // mapping
            HRESULT result = m_device_context->Map(m_vertex_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource);
            if (FAILED(result))
            {
                return;
            }
            // Get a pointer to the data in the vertex buffer.
            VertexCommon* vertices_ptr = (VertexCommon*)mapped_resource.pData;
            // Copy the data into the vertex buffer.
            memcpy(vertices_ptr, (void*)vertices.data(), sizeof(VertexCommon) * static_cast<U32>(vertices.size()));
            // Unlock the vertex buffer.
            m_device_context->Unmap(m_vertex_buffer, 0);
        }
    }

    void VertexBufferCommon::Update(const std::vector<NormalVertexCommon>& vertices) const
    {
        if (!vertices.empty() && m_b_dynamic)
        {
            // Lock the vertex buffer so it can be written to.
            D3D11_MAPPED_SUBRESOURCE mapped_resource;
            // mapping
            HRESULT result = m_device_context->Map(m_vertex_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource);
            if (FAILED(result))
            {
                return;
            }
            // Get a pointer to the data in the vertex buffer.
            NormalVertexCommon* vertices_ptr = (NormalVertexCommon*)mapped_resource.pData;
            // Copy the data into the vertex buffer.
            memcpy(vertices_ptr, (void*)vertices.data(), sizeof(NormalVertexCommon) * static_cast<U32>(vertices.size()));
            // Unlock the vertex buffer.
            m_device_context->Unmap(m_vertex_buffer, 0);
        }
    }

    void VertexBufferCommon::Update(const std::vector<Vector3>& vertices) const
    {
        if (!vertices.empty() && m_b_dynamic)
        {
            // Lock the vertex buffer so it can be written to.
            D3D11_MAPPED_SUBRESOURCE mapped_resource;
            // mapping
            HRESULT result = m_device_context->Map(m_vertex_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource);
            if (FAILED(result))
            {
                return;
            }
            // Get a pointer to the data in the vertex buffer.
            Vector3* vertices_ptr = (Vector3*)mapped_resource.pData;
            // Copy the data into the vertex buffer.
            memcpy(vertices_ptr, (void*)vertices.data(), sizeof(Vector3) * static_cast<U32>(vertices.size()));
            // Unlock the vertex buffer.
            m_device_context->Unmap(m_vertex_buffer, 0);
        }
    }

    bool VertexBufferCommon::Init(RendererCommon* renderer, void* data, size_t vertex_size, size_t vertex_count, bool is_dynamic)
    {
        if (m_vertex_buffer != nullptr)
        {
            m_vertex_buffer->Release();
            m_vertex_buffer = nullptr;
        }
        // Set up the description of the static vertex buffer.
        D3D11_BUFFER_DESC vertex_buffer_desc;
        vertex_buffer_desc.Usage               = is_dynamic ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
        vertex_buffer_desc.ByteWidth           = static_cast<U32>(vertex_size * vertex_count);
        vertex_buffer_desc.BindFlags           = D3D11_BIND_VERTEX_BUFFER;
        vertex_buffer_desc.CPUAccessFlags      = is_dynamic ? D3D11_CPU_ACCESS_WRITE : 0;
        vertex_buffer_desc.MiscFlags           = 0;
        vertex_buffer_desc.StructureByteStride = 0;
        // Give the sub resource structure a pointer to the vertex data.
        D3D11_SUBRESOURCE_DATA vertex_data;
        vertex_data.pSysMem          = data;
        vertex_data.SysMemPitch      = 0;
        vertex_data.SysMemSlicePitch = 0;
        // Now create the vertex buffer.
        HRESULT result = renderer->GetDevice()->CreateBuffer(&vertex_buffer_desc, &vertex_data, &m_vertex_buffer);
        if (FAILED(result))
            return false;

        m_device_context = renderer->GetDeviceContext();
        m_b_dynamic      = is_dynamic;
        m_stride         = static_cast<U32>(vertex_size);
        return true;
    }

    void VertexBufferCommon::Update(void* data, size_t vertex_count) const
    {
        if (vertex_count > 0 && m_b_dynamic && data != nullptr)
        {
            // Lock the vertex buffer so it can be written to.
            D3D11_MAPPED_SUBRESOURCE mapped_resource;
            // mapping
            HRESULT result = m_device_context->Map(m_vertex_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource);
            if (FAILED(result))
            {
                return;
            }
            // Copy the data into the vertex buffer.
            memcpy(mapped_resource.pData, data, m_stride * vertex_count);
            // Unlock the vertex buffer.
            m_device_context->Unmap(m_vertex_buffer, 0);
        }
    }

    void VertexBufferCommon::Bind(U32 offset) const
    {
        // Set the vertex buffer to active in the input assembler so it can be rendered.
        m_device_context->IASetVertexBuffers(0, 1, &m_vertex_buffer, &m_stride, &offset);

        // Set the type of primitive that should be rendered from this vertex buffer.
        m_device_context->IASetPrimitiveTopology(m_topology);
    }

    void VertexBufferCommon::Bind(U32 offset, InstanceBufferCommon* instance_buffer) const
    {
        // Set the buffer strides.
        U32 strides[2];
        strides[0] = m_stride;
        strides[1] = instance_buffer->m_stride;

        // Set the buffer offsets.
        U32 offsets[2];
        offsets[0] = offset;
        offsets[1] = 0;

        // Set the array of pointers to the vertex and instance buffers.
        ID3D11Buffer* buffers[2];
        buffers[0] = m_vertex_buffer;
        buffers[1] = instance_buffer->m_instance_buffer;

        // Set the vertex and instance buffer to active in the input assembler so it can be rendered.
        m_device_context->IASetVertexBuffers(0, 2, buffers, strides, offsets);

        // Set the type of primitive that should be rendered from this vertex buffer.
        m_device_context->IASetPrimitiveTopology(m_topology);
    }

    void VertexBufferCommon::Shutdown()
    {
        if (m_vertex_buffer != nullptr)
        {
            m_vertex_buffer->Release();
            m_vertex_buffer = nullptr;
        }
    }

    void VertexBufferCommon::SetPrimitiveTopology(eTopologyType type)
    {
        switch (type)
        {
        case eTopologyType::PointList:
            m_topology = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
            break;
        case eTopologyType::LineList:
            m_topology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
            break;
        case eTopologyType::TriangleList:
            m_topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
            break;
        default:
            m_topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
            break;
        }
    }
}
