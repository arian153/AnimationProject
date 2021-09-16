#include "InstanceBufferDX11.hpp"
#include "../../Common/Buffer2/InstanceBufferCommon.hpp"
#include "../../Common/Renderer/RendererCommon.hpp"

namespace CS460
{
    InstanceBufferDX11::InstanceBufferDX11()
    {
    }

    InstanceBufferDX11::~InstanceBufferDX11()
    {
    }

    InstanceBufferCommon::InstanceBufferCommon()
    {
    }

    InstanceBufferCommon::~InstanceBufferCommon()
    {
    }

    bool InstanceBufferCommon::Init(RendererCommon* renderer, const std::vector<InstanceBufferData>& instances)
    {
        m_device         = renderer->GetDevice();
        m_device_context = renderer->GetDeviceContext();

        if (instances.empty() == false)
        {
            if (m_instance_buffer != nullptr)
            {
                m_instance_buffer->Release();
                m_instance_buffer = nullptr;
            }
            // Set up the description of the instance buffer.
            D3D11_BUFFER_DESC instance_buffer_desc;
            instance_buffer_desc.Usage               = D3D11_USAGE_DYNAMIC;
            instance_buffer_desc.ByteWidth           = sizeof(InstanceBufferData) * static_cast<U32>(instances.size());
            instance_buffer_desc.BindFlags           = D3D11_BIND_VERTEX_BUFFER;
            instance_buffer_desc.CPUAccessFlags      = D3D11_CPU_ACCESS_WRITE;
            instance_buffer_desc.MiscFlags           = 0;
            instance_buffer_desc.StructureByteStride = 0;
            // Give the sub resource structure a pointer to the instance data.
            D3D11_SUBRESOURCE_DATA instance_data;
            instance_data.pSysMem          = instances.data();
            instance_data.SysMemPitch      = 0;
            instance_data.SysMemSlicePitch = 0;
            // Create the instance buffer.
            HRESULT result = m_device->CreateBuffer(&instance_buffer_desc, &instance_data, &m_instance_buffer);
            if (FAILED(result))
                return false;

            m_stride = static_cast<U32>(sizeof(InstanceBufferData));
            return true;
        }

        return false;
    }

    void InstanceBufferCommon::Update(const std::vector<InstanceBufferData>& instances) const
    {
        if (instances.empty() == false)
        {
            // Lock the instance buffer so it can be written to.
            D3D11_MAPPED_SUBRESOURCE mapped_resource;
            // mapping
            HRESULT result = m_device_context->Map(m_instance_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource);
            if (FAILED(result))
            {
                return;
            }
            // Get a pointer to the data in the instance buffer.
            InstanceDataCommon* instances_ptr = (InstanceDataCommon*)mapped_resource.pData;
            // Copy the data into the vertex buffer.
            memcpy(instances_ptr, (void*)instances.data(), sizeof(InstanceBufferData) * static_cast<U32>(instances.size()));
            // Unlock the instance buffer.
            m_device_context->Unmap(m_instance_buffer, 0);
        }
    }

    void InstanceBufferCommon::Shutdown()
    {
        if (m_instance_buffer != nullptr)
        {
            m_instance_buffer->Release();
            m_instance_buffer = nullptr;
        }
    }
}
