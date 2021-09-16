#include "ShaderProgramDX11.hpp"

#include <d3dcompiler.h>

#include "../../../../Manager/Resource/ResourceType/ShaderResource.hpp"
#include "../../Common/Buffer2/ConstantBufferCommon.hpp"
#include "../../Common/Buffer2/VertexLayoutCommon.hpp"
#include "../../Common/Renderer/RendererCommon.hpp"
#include "../../Common/Shader/ShaderManagerCommon.hpp"
#include "../../Common/Shader/ShaderProgramCommon.hpp"

namespace CS460
{
    ShaderProgramDX11::ShaderProgramDX11()
    {
    }

    ShaderProgramDX11::~ShaderProgramDX11()
    {
    }

    ShaderProgramCommon::ShaderProgramCommon(ShaderManagerCommon* shader_manager)
        : m_shader_manager(shader_manager)
    {
    }

    ShaderProgramCommon::~ShaderProgramCommon()
    {
    }

    bool ShaderProgramCommon::Init(RendererCommon* renderer)
    {
        m_hwnd           = renderer->GetHwnd();
        m_device         = renderer->GetDevice();
        m_device_context = renderer->GetDeviceContext();

        //Initialize vertex shader
        ID3D10Blob*  error_message        = nullptr;
        ID3D10Blob*  vertex_shader_buffer = nullptr;
        std::wstring vertex_shader_path   = m_resource->WFilePath();

        // Compile the vertex shader code.
        HRESULT result = D3DCompileFromFile(vertex_shader_path.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "VertexShaderEntry", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &vertex_shader_buffer, &error_message);
        if (FAILED(result))
        {
            if (error_message)
            {
                m_shader_manager->OutputShaderErrorMessage(error_message, m_hwnd, vertex_shader_path);
            }
            else
            {
                MessageBox(m_hwnd, vertex_shader_path.c_str(), L"Missing Shader File", MB_OK);
            }
            return false;
        }

        // Create the vertex shader from the buffer.
        result = m_device->CreateVertexShader(vertex_shader_buffer->GetBufferPointer(), vertex_shader_buffer->GetBufferSize(), nullptr, &m_vertex_shader);
        if (FAILED(result))
        {
            return false;
        }

        //Initialize pixel shader
        ID3D10Blob*  pixel_shader_buffer = nullptr;
        std::wstring pixel_shader_path   = m_resource->WFilePath();;
        // Compile the pixel shader code.
        result = D3DCompileFromFile(pixel_shader_path.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "PixelShaderEntry", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &pixel_shader_buffer, &error_message);
        if (FAILED(result))
        {
            if (error_message)
            {
                m_shader_manager->OutputShaderErrorMessage(error_message, m_hwnd, pixel_shader_path);
            }
            else
            {
                MessageBox(m_hwnd, pixel_shader_path.c_str(), L"Missing Shader File", MB_OK);
            }
            return false;
        }

        // Create the pixel shader from the buffer.
        result = m_device->CreatePixelShader(pixel_shader_buffer->GetBufferPointer(), pixel_shader_buffer->GetBufferSize(), nullptr, &m_pixel_shader);
        if (FAILED(result))
        {
            return false;
        }

        //Initialize input layout
        if (m_vertex_layout == nullptr)
            return false;

        auto   attributes = m_vertex_layout->GetAttributes();
        size_t size       = attributes.size();

        if (size == 0)
            return false;

        std::vector<D3D11_INPUT_ELEMENT_DESC> vertex_layout;
        vertex_layout.resize(size);
        for (size_t i = 0; i < size; ++i)
        {
            vertex_layout[i].SemanticName         = attributes[i].sem_name.c_str();
            vertex_layout[i].SemanticIndex        = attributes[i].sem_idx;
            vertex_layout[i].InputSlot            = attributes[i].input_slot;
            vertex_layout[i].AlignedByteOffset    = D3D11_APPEND_ALIGNED_ELEMENT;
            vertex_layout[i].InputSlotClass       = (D3D11_INPUT_CLASSIFICATION)attributes[i].slot_type;
            vertex_layout[i].InstanceDataStepRate = attributes[i].step_rate;

            size_t count = attributes[i].format_count;
            auto   type  = attributes[i].format_type;

            int format;
            switch (count)
            {
            case 2:
                format = (int)DXGI_FORMAT_R32G32_TYPELESS;
                break;
            case 3:
                format = (int)DXGI_FORMAT_R32G32B32_TYPELESS;
                break;
            case 4:
                format = (int)DXGI_FORMAT_R32G32B32A32_TYPELESS;
                break;
            default:
                format = (int)DXGI_FORMAT_UNKNOWN;
                break;
            }

            switch (type)
            {
            case eAttributeType::T32:
                break;
            case eAttributeType::R32:
                format += 1;
                break;
            case eAttributeType::I32:
                format += 3;
                break;
            case eAttributeType::U32:
                format += 2;
                break;
            default:
                break;
            }

            vertex_layout[i].Format = (DXGI_FORMAT)format;
        }

        result = m_device->CreateInputLayout(
                                             vertex_layout.data(), (UINT)vertex_layout.size(),
                                             vertex_shader_buffer->GetBufferPointer(),
                                             vertex_shader_buffer->GetBufferSize(), &m_layout);
        if (FAILED(result))
        {
            return false;
        }

        // Release all shader buffers.
        vertex_shader_buffer->Release();
        vertex_shader_buffer = nullptr;

        pixel_shader_buffer->Release();
        pixel_shader_buffer = nullptr;

        // Create a texture sampler state description.
        D3D11_SAMPLER_DESC sampler_desc;
        sampler_desc.Filter         = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        sampler_desc.AddressU       = D3D11_TEXTURE_ADDRESS_WRAP;
        sampler_desc.AddressV       = D3D11_TEXTURE_ADDRESS_WRAP;
        sampler_desc.AddressW       = D3D11_TEXTURE_ADDRESS_WRAP;
        sampler_desc.MipLODBias     = 0.0f;
        sampler_desc.MaxAnisotropy  = 1;
        sampler_desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
        sampler_desc.BorderColor[0] = 0;
        sampler_desc.BorderColor[1] = 0;
        sampler_desc.BorderColor[2] = 0;
        sampler_desc.BorderColor[3] = 0;
        sampler_desc.MinLOD         = 0;
        sampler_desc.MaxLOD         = D3D11_FLOAT32_MAX;
        // Create the texture sampler state.
        result = m_device->CreateSamplerState(&sampler_desc, &m_sampler_state);
        if (FAILED(result))
        {
            return false;
        }

        return true;
    }

    void ShaderProgramCommon::Shutdown()
    {
        m_constant_buffers.clear();

        // Release the sampler state.
        if (m_sampler_state != nullptr)
        {
            m_sampler_state->Release();
            m_sampler_state = nullptr;
        }
        // Release the layout.
        m_vertex_layout = nullptr;
        if (m_layout != nullptr)
        {
            m_layout->Release();
            m_layout = nullptr;
        }
        // Release the pixel shader.
        if (m_pixel_shader != nullptr)
        {
            m_pixel_shader->Release();
            m_pixel_shader = nullptr;
        }
        // Release the vertex shader.
        if (m_vertex_shader != nullptr)
        {
            m_vertex_shader->Release();
            m_vertex_shader = nullptr;
        }
    }

    void ShaderProgramCommon::Bind() const
    {
        // Set the vertex input layout.
        m_device_context->IASetInputLayout(m_layout);
        // Set the vertex shader and pixel shader.
        m_device_context->VSSetShader(m_vertex_shader, nullptr, 0);
        m_device_context->PSSetShader(m_pixel_shader, nullptr, 0);
        // Set the sampler state in the pixel shader.
        m_device_context->PSSetSamplers(0, 1, &m_sampler_state);
    }

    void ShaderProgramCommon::SetVertexLayout(VertexLayoutCommon* layout)
    {
        m_vertex_layout = layout;
    }

    void ShaderProgramCommon::SetShaderResource(ShaderResource* resource)
    {
        m_resource = resource;
    }
}
