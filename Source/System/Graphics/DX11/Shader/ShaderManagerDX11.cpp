#include <fstream>

#include "ShaderManagerDX11.hpp"
#include "../../Common/Shader/ShaderManagerCommon.hpp"
#include "../../Common/Buffer2/VertexLayoutCommon.hpp"
#include "../../Common/Renderer/RendererCommon.hpp"
#include "../../Common/Shader/ShaderProgramCommon.hpp"
#include "../../../../Manager/Resource/ResourceManager.hpp"
#include "../../../../Manager/Resource/ResourceType/ShaderResource.hpp"

namespace CS460
{
    ShaderManagerDX11::ShaderManagerDX11()
    {
    }

    ShaderManagerDX11::~ShaderManagerDX11()
    {
    }

    void ShaderManagerDX11::SetHWnd(HWND hwnd)
    {
        m_hwnd = hwnd;
    }

    void ShaderManagerDX11::SetDevice(ID3D11Device* device)
    {
        m_device = device;
    }

    void ShaderManagerDX11::SetDeviceContext(ID3D11DeviceContext* device_context)
    {
        m_device_context = device_context;
    }

    void ShaderManagerDX11::OutputShaderErrorMessage(ID3D10Blob* error, HWND hwnd, const std::wstring& shader_name)
    {
        std::ofstream file_out;
        // Get a pointer to the error message text buffer.
        char* compile_errors = (char*)(error->GetBufferPointer());
        // Get the length of the message.
        unsigned long long buffer_size = error->GetBufferSize();
        // Open a file to write the error message to.
        file_out.open("shader-error.txt");
        // Write out the error message.
        for (unsigned long long i = 0; i < buffer_size; i++)
        {
            file_out << compile_errors[i];
        }
        // Close the file.
        file_out.close();
        // Release the error message.
        error->Release();
        error = nullptr;
        // Pop a message up on the screen to notify the user to check the text file for compile errors.
        MessageBox(hwnd, L"Error compiling shader.  Check shader-error.txt for message.", shader_name.c_str(), MB_OK);
    }

    ShaderManagerCommon::ShaderManagerCommon()
    {
    }

    ShaderManagerCommon::~ShaderManagerCommon()
    {
    }

    void ShaderManagerCommon::Initialize(RendererCommon* renderer, ResourceManager* resource_manager)
    {
        SetHWnd(renderer->GetHwnd());
        SetDevice(renderer->GetDevice());
        SetDeviceContext(renderer->GetDeviceContext());
        m_resource_manager = resource_manager;
        m_renderer         = renderer;

        //Point Vertex
        {
            m_point_vertex_layout = new VertexLayoutCommon();
            m_point_vertex_layout->PushDX11(eAttributeType::R32, 3, "POSITION", 0, eInputSlotType::VERTEX_DATA, 0, 0);
        }

        //Color Vertex
        {
            m_color_vertex_layout = new VertexLayoutCommon();
            m_color_vertex_layout->PushDX11(eAttributeType::R32, 3, "POSITION", 0, eInputSlotType::VERTEX_DATA, 0, 0);
            m_color_vertex_layout->PushDX11(eAttributeType::R32, 4, "COLOR", 0, eInputSlotType::VERTEX_DATA, 0, 0);

            m_instancing_color_vertex_layout = new VertexLayoutCommon();
            m_instancing_color_vertex_layout->PushDX11(eAttributeType::R32, 3, "POSITION", 0, eInputSlotType::VERTEX_DATA, 0, 0);
            m_instancing_color_vertex_layout->PushDX11(eAttributeType::R32, 4, "COLOR", 0, eInputSlotType::VERTEX_DATA, 0, 0);
            m_instancing_color_vertex_layout->PushDX11(eAttributeType::R32, 4, "WORLD", 0, eInputSlotType::INSTANCE_DATA, 1, 1);
            m_instancing_color_vertex_layout->PushDX11(eAttributeType::R32, 4, "WORLD", 1, eInputSlotType::INSTANCE_DATA, 1, 1);
            m_instancing_color_vertex_layout->PushDX11(eAttributeType::R32, 4, "WORLD", 2, eInputSlotType::INSTANCE_DATA, 1, 1);
            m_instancing_color_vertex_layout->PushDX11(eAttributeType::R32, 4, "WORLD", 3, eInputSlotType::INSTANCE_DATA, 1, 1);
            m_instancing_color_vertex_layout->PushDX11(eAttributeType::R32, 4, "COLOR", 1, eInputSlotType::INSTANCE_DATA, 1, 1);
            m_instancing_color_vertex_layout->PushDX11(eAttributeType::R32, 4, "COLOR", 2, eInputSlotType::INSTANCE_DATA, 1, 1);
            m_instancing_color_vertex_layout->PushDX11(eAttributeType::R32, 4, "COLOR", 3, eInputSlotType::INSTANCE_DATA, 1, 1);
            m_instancing_color_vertex_layout->PushDX11(eAttributeType::R32, 4, "COLOR", 4, eInputSlotType::INSTANCE_DATA, 1, 1);
        }

        //Texture Vertex
        {
            m_texture_vertex_layout = new VertexLayoutCommon();
            m_texture_vertex_layout->PushDX11(eAttributeType::R32, 3, "POSITION", 0, eInputSlotType::VERTEX_DATA, 0, 0);
            m_texture_vertex_layout->PushDX11(eAttributeType::R32, 2, "TEXCOORD", 0, eInputSlotType::VERTEX_DATA, 0, 0);

            m_instancing_texture_vertex_layout = new VertexLayoutCommon();
            m_instancing_texture_vertex_layout->PushDX11(eAttributeType::R32, 3, "POSITION", 0, eInputSlotType::VERTEX_DATA, 0, 0);
            m_instancing_texture_vertex_layout->PushDX11(eAttributeType::R32, 2, "TEXCOORD", 0, eInputSlotType::VERTEX_DATA, 0, 0);
            m_instancing_texture_vertex_layout->PushDX11(eAttributeType::R32, 4, "WORLD", 0, eInputSlotType::INSTANCE_DATA, 1, 1);
            m_instancing_texture_vertex_layout->PushDX11(eAttributeType::R32, 4, "WORLD", 1, eInputSlotType::INSTANCE_DATA, 1, 1);
            m_instancing_texture_vertex_layout->PushDX11(eAttributeType::R32, 4, "WORLD", 2, eInputSlotType::INSTANCE_DATA, 1, 1);
            m_instancing_texture_vertex_layout->PushDX11(eAttributeType::R32, 4, "WORLD", 3, eInputSlotType::INSTANCE_DATA, 1, 1);
            m_instancing_texture_vertex_layout->PushDX11(eAttributeType::R32, 4, "COLOR", 0, eInputSlotType::INSTANCE_DATA, 1, 1);
            m_instancing_texture_vertex_layout->PushDX11(eAttributeType::R32, 4, "COLOR", 1, eInputSlotType::INSTANCE_DATA, 1, 1);
            m_instancing_texture_vertex_layout->PushDX11(eAttributeType::R32, 4, "COLOR", 2, eInputSlotType::INSTANCE_DATA, 1, 1);
            m_instancing_texture_vertex_layout->PushDX11(eAttributeType::R32, 4, "COLOR", 3, eInputSlotType::INSTANCE_DATA, 1, 1);
        }

        //Normal Vertex
        {
            m_normal_vertex_layout = new VertexLayoutCommon();
            m_normal_vertex_layout->PushDX11(eAttributeType::R32, 3, "POSITION", 0, eInputSlotType::VERTEX_DATA, 0, 0);
            m_normal_vertex_layout->PushDX11(eAttributeType::R32, 3, "NORMAL", 0, eInputSlotType::VERTEX_DATA, 0, 0);
            m_normal_vertex_layout->PushDX11(eAttributeType::R32, 3, "TANGENT", 0, eInputSlotType::VERTEX_DATA, 0, 0);
            m_normal_vertex_layout->PushDX11(eAttributeType::R32, 3, "BINORMAL", 0, eInputSlotType::VERTEX_DATA, 0, 0);

            m_instancing_normal_vertex_layout = new VertexLayoutCommon();
            m_instancing_normal_vertex_layout->PushDX11(eAttributeType::R32, 3, "POSITION", 0, eInputSlotType::VERTEX_DATA, 0, 0);
            m_instancing_normal_vertex_layout->PushDX11(eAttributeType::R32, 3, "NORMAL", 0, eInputSlotType::VERTEX_DATA, 0, 0);
            m_instancing_normal_vertex_layout->PushDX11(eAttributeType::R32, 3, "TANGENT", 0, eInputSlotType::VERTEX_DATA, 0, 0);
            m_instancing_normal_vertex_layout->PushDX11(eAttributeType::R32, 3, "BINORMAL", 0, eInputSlotType::VERTEX_DATA, 0, 0);

            m_instancing_normal_vertex_layout->PushDX11(eAttributeType::R32, 4, "WORLD", 0, eInputSlotType::INSTANCE_DATA, 1, 1);
            m_instancing_normal_vertex_layout->PushDX11(eAttributeType::R32, 4, "WORLD", 1, eInputSlotType::INSTANCE_DATA, 1, 1);
            m_instancing_normal_vertex_layout->PushDX11(eAttributeType::R32, 4, "WORLD", 2, eInputSlotType::INSTANCE_DATA, 1, 1);
            m_instancing_normal_vertex_layout->PushDX11(eAttributeType::R32, 4, "WORLD", 3, eInputSlotType::INSTANCE_DATA, 1, 1);
            m_instancing_normal_vertex_layout->PushDX11(eAttributeType::R32, 4, "COLOR", 0, eInputSlotType::INSTANCE_DATA, 1, 1);
            m_instancing_normal_vertex_layout->PushDX11(eAttributeType::R32, 4, "COLOR", 1, eInputSlotType::INSTANCE_DATA, 1, 1);
            m_instancing_normal_vertex_layout->PushDX11(eAttributeType::R32, 4, "COLOR", 2, eInputSlotType::INSTANCE_DATA, 1, 1);
        }

        //Vertex - General
        {
            m_vertex_layout = new VertexLayoutCommon();
            m_vertex_layout->PushDX11(eAttributeType::R32, 3, "POSITION", 0, eInputSlotType::VERTEX_DATA, 0, 0);
            m_vertex_layout->PushDX11(eAttributeType::R32, 2, "TEXCOORD", 0, eInputSlotType::VERTEX_DATA, 0, 0);
            m_vertex_layout->PushDX11(eAttributeType::R32, 3, "NORMAL", 0, eInputSlotType::VERTEX_DATA, 0, 0);
            m_vertex_layout->PushDX11(eAttributeType::R32, 3, "TANGENT", 0, eInputSlotType::VERTEX_DATA, 0, 0);
            m_vertex_layout->PushDX11(eAttributeType::R32, 3, "BINORMAL", 0, eInputSlotType::VERTEX_DATA, 0, 0);

            m_instancing_vertex_layout = new VertexLayoutCommon();
            m_instancing_vertex_layout->PushDX11(eAttributeType::R32, 3, "POSITION", 0, eInputSlotType::VERTEX_DATA, 0, 0);
            m_instancing_vertex_layout->PushDX11(eAttributeType::R32, 2, "TEXCOORD", 0, eInputSlotType::VERTEX_DATA, 0, 0);
            m_instancing_vertex_layout->PushDX11(eAttributeType::R32, 3, "NORMAL", 0, eInputSlotType::VERTEX_DATA, 0, 0);
            m_instancing_vertex_layout->PushDX11(eAttributeType::R32, 3, "TANGENT", 0, eInputSlotType::VERTEX_DATA, 0, 0);
            m_instancing_vertex_layout->PushDX11(eAttributeType::R32, 3, "BINORMAL", 0, eInputSlotType::VERTEX_DATA, 0, 0);

            m_instancing_vertex_layout->PushDX11(eAttributeType::R32, 4, "WORLD", 0, eInputSlotType::INSTANCE_DATA, 1, 1);
            m_instancing_vertex_layout->PushDX11(eAttributeType::R32, 4, "WORLD", 1, eInputSlotType::INSTANCE_DATA, 1, 1);
            m_instancing_vertex_layout->PushDX11(eAttributeType::R32, 4, "WORLD", 2, eInputSlotType::INSTANCE_DATA, 1, 1);
            m_instancing_vertex_layout->PushDX11(eAttributeType::R32, 4, "WORLD", 3, eInputSlotType::INSTANCE_DATA, 1, 1);

            m_instancing_vertex_layout->PushDX11(eAttributeType::R32, 4, "COLOR", 0, eInputSlotType::INSTANCE_DATA, 1, 1);
            m_instancing_vertex_layout->PushDX11(eAttributeType::R32, 4, "COLOR", 1, eInputSlotType::INSTANCE_DATA, 1, 1);
            m_instancing_vertex_layout->PushDX11(eAttributeType::R32, 4, "COLOR", 2, eInputSlotType::INSTANCE_DATA, 1, 1);
            m_instancing_vertex_layout->PushDX11(eAttributeType::R32, 4, "COLOR", 3, eInputSlotType::INSTANCE_DATA, 1, 1);
        }

        {
            m_skinned_vertex_layout = new VertexLayoutCommon();
            m_skinned_vertex_layout->PushDX11(eAttributeType::R32, 3, "POSITION", 0, eInputSlotType::VERTEX_DATA, 0, 0);
            m_skinned_vertex_layout->PushDX11(eAttributeType::R32, 2, "TEXCOORD", 0, eInputSlotType::VERTEX_DATA, 0, 0);
            m_skinned_vertex_layout->PushDX11(eAttributeType::R32, 3, "NORMAL", 0, eInputSlotType::VERTEX_DATA, 0, 0);
            m_skinned_vertex_layout->PushDX11(eAttributeType::R32, 3, "TANGENT", 0, eInputSlotType::VERTEX_DATA, 0, 0);
            m_skinned_vertex_layout->PushDX11(eAttributeType::R32, 3, "BINORMAL", 0, eInputSlotType::VERTEX_DATA, 0, 0);
            m_skinned_vertex_layout->PushDX11(eAttributeType::R32, 4, "WEIGHTS", 0, eInputSlotType::VERTEX_DATA, 0, 0);
            m_skinned_vertex_layout->PushDX11(eAttributeType::I32, 4, "BONEINDICES", 0, eInputSlotType::VERTEX_DATA, 0, 0);
        }

        AddShader(L"Color.hlsl", m_color_vertex_layout);
        AddShader(L"ColorInstancing.hlsl", m_instancing_color_vertex_layout);
        AddShader(L"Texture.hlsl", m_texture_vertex_layout);
        AddShader(L"TextureInstancing.hlsl", m_instancing_texture_vertex_layout);

        AddShader(L"PhongInstancing.hlsl", m_instancing_vertex_layout);
        AddShader(L"SkyBox.hlsl", m_point_vertex_layout);
        AddShader(L"SkinnedPhong.hlsl", m_skinned_vertex_layout);
    }

    void ShaderManagerCommon::Shutdown()
    {
        if (m_skinned_vertex_layout != nullptr)
        {
            m_skinned_vertex_layout->Clear();
            delete m_skinned_vertex_layout;
            m_skinned_vertex_layout = nullptr;
        }

        if (m_point_vertex_layout != nullptr)
        {
            m_point_vertex_layout->Clear();
            delete m_point_vertex_layout;
            m_point_vertex_layout = nullptr;
        }

        if (m_texture_vertex_layout != nullptr)
        {
            m_texture_vertex_layout->Clear();
            delete m_texture_vertex_layout;
            m_texture_vertex_layout = nullptr;
        }

        if (m_instancing_texture_vertex_layout != nullptr)
        {
            m_instancing_texture_vertex_layout->Clear();
            delete m_instancing_texture_vertex_layout;
            m_instancing_texture_vertex_layout = nullptr;
        }

        if (m_color_vertex_layout != nullptr)
        {
            m_color_vertex_layout->Clear();
            delete m_color_vertex_layout;
            m_color_vertex_layout = nullptr;
        }

        if (m_instancing_color_vertex_layout != nullptr)
        {
            m_instancing_color_vertex_layout->Clear();
            delete m_instancing_color_vertex_layout;
            m_instancing_color_vertex_layout = nullptr;
        }

        if (m_normal_vertex_layout != nullptr)
        {
            m_normal_vertex_layout->Clear();
            delete m_normal_vertex_layout;
            m_normal_vertex_layout = nullptr;
        }

        if (m_instancing_normal_vertex_layout != nullptr)
        {
            m_instancing_normal_vertex_layout->Clear();
            delete m_instancing_normal_vertex_layout;
            m_instancing_normal_vertex_layout = nullptr;
        }

        if (m_vertex_layout != nullptr)
        {
            m_vertex_layout->Clear();
            delete m_vertex_layout;
            m_vertex_layout = nullptr;
        }

        if (m_instancing_vertex_layout != nullptr)
        {
            m_instancing_vertex_layout->Clear();
            delete m_instancing_vertex_layout;
            m_instancing_vertex_layout = nullptr;
        }

        for (auto& [key, shader] : m_shader_table)
        {
            shader->Shutdown();
            delete shader;
            shader = nullptr;
        }
        m_shader_table.clear();
    }

    void ShaderManagerCommon::AddShader(const std::wstring& name, VertexLayoutCommon* layout)
    {
        ShaderResource* resource  = m_resource_manager->GetShaderResourceFileName(name);
        std::string     file_name = resource->FileName();

        auto found = m_shader_table.find(file_name);
        if (found == m_shader_table.end())
        {
            ShaderProgramCommon* new_shader = new ShaderProgramCommon(this);
            new_shader->SetShaderResource(resource);
            new_shader->SetVertexLayout(layout);
            new_shader->Init(m_renderer);
            m_shader_table.emplace(file_name, new_shader);
        }
    }

    void ShaderManagerCommon::Bind(const std::string& name)
    {
        auto found = m_shader_table.find(name);
        if (found != m_shader_table.end())
        {
            found->second->Bind();
        }
    }

    ShaderProgramCommon* ShaderManagerCommon::GetShader(const std::string& name) const
    {
        ShaderProgramCommon* shader = nullptr;
        auto                 found  = m_shader_table.find(name);
        if (found != m_shader_table.end())
        {
            shader = found->second;
        }

        return shader;
    }
}
