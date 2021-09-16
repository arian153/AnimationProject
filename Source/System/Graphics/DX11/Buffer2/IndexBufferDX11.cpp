#include "IndexBufferDX11.hpp"

#include "../../Common/Buffer2/IndexBufferCommon.hpp"
#include "../../Common/Renderer/RendererCommon.hpp"

namespace CS460
{
    IndexBufferDX11::IndexBufferDX11()
    {
    }

    IndexBufferDX11::~IndexBufferDX11()
    {
    }

    IndexBufferCommon::IndexBufferCommon()
    {
    }

    IndexBufferCommon::~IndexBufferCommon()
    {
    }

    bool IndexBufferCommon::Init(RendererCommon* renderer, const std::vector<U32>& indices)
    {
        if (m_index_buffer != nullptr)
        {
            m_index_buffer->Release();
            m_index_buffer  = nullptr;
            m_indices_count = 0;
        }

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
        m_indices_count             = static_cast<U32>(indices.size());
        // Create the index buffer.
        HRESULT result = renderer->GetDevice()->CreateBuffer(&index_buffer_desc, &index_data, &m_index_buffer);
        if (FAILED(result))
            return false;
        m_device_context = renderer->GetDeviceContext();
        return true;
    }

    void IndexBufferCommon::Bind(U32 offset) const
    {
        // Set the index buffer to active in the input assembler so it can be rendered.
        m_device_context->IASetIndexBuffer(m_index_buffer, DXGI_FORMAT_R32_UINT, offset);
    }

    void IndexBufferCommon::Draw() const
    {
        // Render the triangle.
        m_device_context->DrawIndexed(m_indices_count, 0, 0);
    }

    void IndexBufferCommon::Draw(U32 instance_count) const
    {
        // Render the triangle
        m_device_context->DrawIndexedInstanced(m_indices_count, instance_count, 0, 0, 0);
    }

    void IndexBufferCommon::Shutdown()
    {
        if (m_index_buffer != nullptr)
        {
            m_index_buffer->Release();
            m_index_buffer = nullptr;
        }
    }
}
