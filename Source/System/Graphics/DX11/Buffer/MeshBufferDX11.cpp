#include "MeshBufferDX11.hpp"
#include "../../../Core/Utility/CoreDef.hpp"
#include "../../DataType/TopologyDef.hpp"
#include "../../Common/Buffer/MeshBufferCommon.hpp"
#include "../../Common/Renderer/RendererCommon.hpp"

namespace CS460
{
    MeshBufferDX11::MeshBufferDX11()
    {
    }

    MeshBufferDX11::~MeshBufferDX11()
    {
    }

    MeshBufferCommon::MeshBufferCommon()
    {
    }

    MeshBufferCommon::~MeshBufferCommon()
    {
    }

    void MeshBufferCommon::Render(U32 stride, U32 offset, eTopologyType topology) const
    {
        // Set the vertex buffer to active in the input assembler so it can be rendered.
        m_device_context->IASetVertexBuffers(0, 1, &m_vertex_buffer, &stride, &offset);
        // Set the index buffer to active in the input assembler so it can be rendered.
        m_device_context->IASetIndexBuffer(m_index_buffer, DXGI_FORMAT_R32_UINT, 0);
        // Set the type of primitive that should be rendered from this vertex buffer
        switch (topology)
        {
        case CS460::eTopologyType::PointList:
            m_device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
            break;
        case CS460::eTopologyType::LineList:
            m_device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
            break;
        case CS460::eTopologyType::TriangleList:
            m_device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
            break;
        default: //triangle list
            m_device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
            break;
        }
    }

    void MeshBufferCommon::Render(U32 stride, U32 offset, U32 indices_count, eTopologyType topology) const
    {
        // Set the vertex buffer to active in the input assembler so it can be rendered.
        m_device_context->IASetVertexBuffers(0, 1, &m_vertex_buffer, &stride, &offset);
        // Set the index buffer to active in the input assembler so it can be rendered.
        m_device_context->IASetIndexBuffer(m_index_buffer, DXGI_FORMAT_R32_UINT, 0);
        // Set the type of primitive that should be rendered from this vertex buffer
        switch (topology)
        {
        case CS460::eTopologyType::PointList:
            m_device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
            break;
        case CS460::eTopologyType::LineList:
            m_device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
            break;
        case CS460::eTopologyType::TriangleList:
            m_device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
            break;
        default: //triangle list
            m_device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
            break;
        }

        // Render the triangle.
        m_device_context->DrawIndexed(indices_count, 0, 0);
    }

    void MeshBufferCommon::Render(U32 stride, U32 offset) const
    {
        // Set the vertex buffer to active in the input assembler so it can be rendered.
        m_device_context->IASetVertexBuffers(0, 1, &m_vertex_buffer, &stride, &offset);
        // Set the index buffer to active in the input assembler so it can be rendered.
        m_device_context->IASetIndexBuffer(m_index_buffer, DXGI_FORMAT_R32_UINT, 0);
        // Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
        m_device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    }

    void MeshBufferCommon::Shutdown()
    {
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

    bool MeshBufferCommon::BuildBuffer(RendererCommon* renderer, const std::vector<ColorVertexCommon>& vertices, const std::vector<U32>& indices)
    {
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
        // Set up the description of the static vertex buffer.
        D3D11_BUFFER_DESC vertex_buffer_desc;
        vertex_buffer_desc.Usage               = D3D11_USAGE_DEFAULT;
        vertex_buffer_desc.ByteWidth           = sizeof(ColorVertexCommon) * static_cast<U32>(vertices.size());
        vertex_buffer_desc.BindFlags           = D3D11_BIND_VERTEX_BUFFER;
        vertex_buffer_desc.CPUAccessFlags      = 0;
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
        // Set up the description of the static index buffer.
        D3D11_BUFFER_DESC index_buffer_desc;
        index_buffer_desc.Usage               = D3D11_USAGE_DEFAULT;
        index_buffer_desc.ByteWidth           = sizeof(U32) * static_cast<U32>(indices.size());;
        index_buffer_desc.BindFlags           = D3D11_BIND_INDEX_BUFFER;
        index_buffer_desc.CPUAccessFlags      = 0;
        index_buffer_desc.MiscFlags           = 0;
        index_buffer_desc.StructureByteStride = 0;
        // Give the sub resource structure a pointer to the index data.
        D3D11_SUBRESOURCE_DATA index_data;
        index_data.pSysMem          = indices.data();
        index_data.SysMemPitch      = 0;
        index_data.SysMemSlicePitch = 0;
        // Create the index buffer.
        result = renderer->GetDevice()->CreateBuffer(&index_buffer_desc, &index_data, &m_index_buffer);
        if (FAILED(result))
            return false;
        m_device_context = renderer->GetDeviceContext();
        return true;
    }

    bool MeshBufferCommon::BuildBuffer(RendererCommon* renderer, const std::vector<TextureVertexCommon>& vertices, const std::vector<U32>& indices)
    {
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
        // Set up the description of the static vertex buffer.
        D3D11_BUFFER_DESC vertex_buffer_desc;
        vertex_buffer_desc.Usage               = D3D11_USAGE_DEFAULT;
        vertex_buffer_desc.ByteWidth           = sizeof(TextureVertexCommon) * static_cast<U32>(vertices.size());
        vertex_buffer_desc.BindFlags           = D3D11_BIND_VERTEX_BUFFER;
        vertex_buffer_desc.CPUAccessFlags      = 0;
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
        // Set up the description of the static index buffer.
        D3D11_BUFFER_DESC index_buffer_desc;
        index_buffer_desc.Usage               = D3D11_USAGE_DEFAULT;
        index_buffer_desc.ByteWidth           = sizeof(U32) * static_cast<U32>(indices.size());;
        index_buffer_desc.BindFlags           = D3D11_BIND_INDEX_BUFFER;
        index_buffer_desc.CPUAccessFlags      = 0;
        index_buffer_desc.MiscFlags           = 0;
        index_buffer_desc.StructureByteStride = 0;
        // Give the sub resource structure a pointer to the index data.
        D3D11_SUBRESOURCE_DATA index_data;
        index_data.pSysMem          = indices.data();
        index_data.SysMemPitch      = 0;
        index_data.SysMemSlicePitch = 0;
        // Create the index buffer.
        result = renderer->GetDevice()->CreateBuffer(&index_buffer_desc, &index_data, &m_index_buffer);
        if (FAILED(result))
            return false;
        m_device_context = renderer->GetDeviceContext();
        return true;
    }

    bool MeshBufferCommon::BuildBuffer(RendererCommon* renderer, const std::vector<VertexCommon>& vertices, const std::vector<U32>& indices)
    {
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
        // Set up the description of the static vertex buffer.
        D3D11_BUFFER_DESC vertex_buffer_desc;
        vertex_buffer_desc.Usage               = D3D11_USAGE_DEFAULT;
        vertex_buffer_desc.ByteWidth           = sizeof(VertexCommon) * static_cast<U32>(vertices.size());
        vertex_buffer_desc.BindFlags           = D3D11_BIND_VERTEX_BUFFER;
        vertex_buffer_desc.CPUAccessFlags      = 0;
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
        // Set up the description of the static index buffer.
        D3D11_BUFFER_DESC index_buffer_desc;
        index_buffer_desc.Usage               = D3D11_USAGE_DEFAULT;
        index_buffer_desc.ByteWidth           = sizeof(U32) * static_cast<U32>(indices.size());;
        index_buffer_desc.BindFlags           = D3D11_BIND_INDEX_BUFFER;
        index_buffer_desc.CPUAccessFlags      = 0;
        index_buffer_desc.MiscFlags           = 0;
        index_buffer_desc.StructureByteStride = 0;
        // Give the sub resource structure a pointer to the index data.
        D3D11_SUBRESOURCE_DATA index_data;
        index_data.pSysMem          = indices.data();
        index_data.SysMemPitch      = 0;
        index_data.SysMemSlicePitch = 0;
        // Create the index buffer.
        result = renderer->GetDevice()->CreateBuffer(&index_buffer_desc, &index_data, &m_index_buffer);
        if (FAILED(result))
            return false;
        m_device_context = renderer->GetDeviceContext();
        return true;
    }
}
