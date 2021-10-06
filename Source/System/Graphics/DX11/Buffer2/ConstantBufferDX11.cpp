#include "ConstantBufferDX11.hpp"

#include "../../Common/Buffer2/ConstantBufferCommon.hpp"
#include "../../Common/Buffer2/ConstantBufferData.hpp"
#include "../../Common/Renderer/RendererCommon.hpp"

namespace CS460
{
    ConstantBufferDX11::ConstantBufferDX11()
    {
    }

    ConstantBufferDX11::~ConstantBufferDX11()
    {
    }

    ConstantBufferCommon::ConstantBufferCommon()
    {
    }

    ConstantBufferCommon::~ConstantBufferCommon()
    {
    }

    bool ConstantBufferCommon::Init(RendererCommon* renderer, eBindingStage type, size_t buffer_size, U32 slot)
    {
        if (m_constant_buffer != nullptr)
        {
            m_constant_buffer->Release();
            m_constant_buffer = nullptr;
        }

        m_stage        = type;
        m_slot         = slot;
        U32 byte_width = (buffer_size + 255) & ~255;

        D3D11_BUFFER_DESC buffer_desc;
        // Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
        buffer_desc.Usage               = D3D11_USAGE_DYNAMIC;
        buffer_desc.ByteWidth           = byte_width;
        buffer_desc.BindFlags           = D3D11_BIND_CONSTANT_BUFFER;
        buffer_desc.CPUAccessFlags      = D3D11_CPU_ACCESS_WRITE;
        buffer_desc.MiscFlags           = 0;
        buffer_desc.StructureByteStride = 0;
        // Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
        HRESULT result = renderer->GetDevice()->CreateBuffer(&buffer_desc, nullptr, &m_constant_buffer);
        if (FAILED(result))
            return false;
        m_device_context = renderer->GetDeviceContext();
        return true;
    }

    void ConstantBufferCommon::Update(void* data) const
    {
        D3D11_MAPPED_SUBRESOURCE mapped_resource;
        HRESULT                  result = m_device_context->Map(m_constant_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource);

        if (FAILED(result))
            return;

        mapped_resource.pData = data;
        m_device_context->Unmap(m_constant_buffer, 0);
    }

    void ConstantBufferCommon::Update(const ColorBufferData& data) const
    {
        D3D11_MAPPED_SUBRESOURCE mapped_resource;
        HRESULT                  result = m_device_context->Map(m_constant_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource);

        if (FAILED(result))
            return;

        ColorBufferData* data_ptr = (ColorBufferData*)mapped_resource.pData;
        data_ptr->color           = data.color;
        m_device_context->Unmap(m_constant_buffer, 0);
    }

    void ConstantBufferCommon::Update(const CameraBufferData& data) const
    {
        D3D11_MAPPED_SUBRESOURCE mapped_resource;
        HRESULT                  result = m_device_context->Map(m_constant_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource);

        if (FAILED(result))
            return;

        CameraBufferData* data_ptr = (CameraBufferData*)mapped_resource.pData;
        data_ptr->camera_position  = data.camera_position;

        m_device_context->Unmap(m_constant_buffer, 0);
    }

    void ConstantBufferCommon::Update(const GammaBufferData& data) const
    {
        D3D11_MAPPED_SUBRESOURCE mapped_resource;
        HRESULT                  result = m_device_context->Map(m_constant_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource);

        if (FAILED(result))
            return;

        GammaBufferData* data_ptr  = (GammaBufferData*)mapped_resource.pData;
        data_ptr->gamma_correction = data.gamma_correction;

        m_device_context->Unmap(m_constant_buffer, 0);
    }

    void ConstantBufferCommon::Update(const LightBufferData& data) const
    {
        D3D11_MAPPED_SUBRESOURCE mapped_resource;
        HRESULT                  result = m_device_context->Map(m_constant_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource);

        if (FAILED(result))
            return;

        LightBufferData* data_ptr = (LightBufferData*)mapped_resource.pData;
        data_ptr->ambient_color   = data.ambient_color;
        data_ptr->diffuse_color   = data.diffuse_color;
        data_ptr->specular_color  = data.specular_color;
        //data_ptr->light_direction = data.light_direction;
        data_ptr->intensity = data.intensity;

        m_device_context->Unmap(m_constant_buffer, 0);
    }

    void ConstantBufferCommon::Update(const MatrixBufferData& data) const
    {
        D3D11_MAPPED_SUBRESOURCE mapped_resource;
        HRESULT                  result = m_device_context->Map(m_constant_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource);

        if (FAILED(result))
            return;

        MatrixBufferData* data_ptr = (MatrixBufferData*)mapped_resource.pData;
        data_ptr->world            = data.world.Transpose();
        data_ptr->view             = data.view.Transpose();
        data_ptr->proj             = data.proj.Transpose();

        //mapped_resource.pData = data;
        m_device_context->Unmap(m_constant_buffer, 0);
    }

    void ConstantBufferCommon::Update(const MatrixBufferDataInstancing& data) const
    {
        D3D11_MAPPED_SUBRESOURCE mapped_resource;
        HRESULT                  result = m_device_context->Map(m_constant_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource);

        if (FAILED(result))
            return;

        MatrixBufferDataInstancing* data_ptr = (MatrixBufferDataInstancing*)mapped_resource.pData;
        data_ptr->view                       = data.view.Transpose();
        data_ptr->proj                       = data.proj.Transpose();

        //mapped_resource.pData = data;
        m_device_context->Unmap(m_constant_buffer, 0);
    }

    void ConstantBufferCommon::Update(const TextureBufferData& data) const
    {
        D3D11_MAPPED_SUBRESOURCE mapped_resource;
        HRESULT                  result = m_device_context->Map(m_constant_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource);

        if (FAILED(result))
            return;

        TextureBufferData* data_ptr = (TextureBufferData*)mapped_resource.pData;
        data_ptr->diff_type         = data.diff_type;
        data_ptr->spec_type         = data.spec_type;
        data_ptr->norm_type         = data.norm_type;
        data_ptr->gamma             = data.gamma;

        m_device_context->Unmap(m_constant_buffer, 0);
    }

    void ConstantBufferCommon::Update(const MultipleLightsBufferData& data) const
    {
        D3D11_MAPPED_SUBRESOURCE mapped_resource;
        HRESULT                  result = m_device_context->Map(m_constant_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource);

        if (FAILED(result))
            return;

        MultipleLightsBufferData* data_ptr = (MultipleLightsBufferData*)mapped_resource.pData;
        int                       count    = data.light_count;
        data_ptr->light_count              = data.light_count;
        for (int i = 0; i < count; ++i)
        {
            data_ptr->data[i].ambient_color  = data.data[i].ambient_color;
            data_ptr->data[i].diffuse_color  = data.data[i].diffuse_color;
            data_ptr->data[i].specular_color = data.data[i].specular_color;
            data_ptr->data[i].ambient_range  = data.data[i].ambient_range;
            data_ptr->data[i].direction      = data.data[i].direction;
            data_ptr->data[i].range          = data.data[i].range;
            data_ptr->data[i].position       = data.data[i].position;
            data_ptr->data[i].length         = data.data[i].length;
            data_ptr->data[i].attenuation    = data.data[i].attenuation;
            data_ptr->data[i].spot           = data.data[i].spot;
            data_ptr->data[i].type           = data.data[i].type;
            data_ptr->data[i].intensity      = data.data[i].intensity;
        }
        m_device_context->Unmap(m_constant_buffer, 0);
    }

    void ConstantBufferCommon::Update(const MaterialBufferData& data) const
    {
        D3D11_MAPPED_SUBRESOURCE mapped_resource;
        HRESULT                  result = m_device_context->Map(m_constant_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource);

        if (FAILED(result))
            return;

        MaterialBufferData* data_ptr = (MaterialBufferData*)mapped_resource.pData;
        data_ptr->ambient            = data.ambient;
        data_ptr->diffuse            = data.diffuse;
        data_ptr->specular           = data.specular;
        data_ptr->reflect            = data.reflect;

        m_device_context->Unmap(m_constant_buffer, 0);
    }

    void* ConstantBufferCommon::Map() const
    {
        D3D11_MAPPED_SUBRESOURCE mapped_resource;
        HRESULT                  result = m_device_context->Map(m_constant_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource);

        if (FAILED(result))
            return nullptr;

        return mapped_resource.pData;
    }

    void ConstantBufferCommon::UnMap() const
    {
        m_device_context->Unmap(m_constant_buffer, 0);
    }

    void ConstantBufferCommon::Bind() const
    {
        if (m_stage == eBindingStage::VertexShader)
            m_device_context->VSSetConstantBuffers(m_slot, 1, &m_constant_buffer);

        if (m_stage == eBindingStage::PixelShader)
            m_device_context->PSSetConstantBuffers(m_slot, 1, &m_constant_buffer);
    }

    void ConstantBufferCommon::Shutdown()
    {
        if (m_constant_buffer != nullptr)
        {
            m_constant_buffer->Release();
            m_constant_buffer = nullptr;
        }
    }
}
