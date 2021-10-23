#include "Mesh.hpp"

#include "../../Math/Algebra/Vector3.hpp"
#include "../../Math/Structure/Transform.hpp"
#include "../Common/Buffer2/ConstantBufferCommon.hpp"
#include "../Common/Buffer2/IndexBufferCommon.hpp"
#include "../Common/Buffer2/InstanceBufferCommon.hpp"
#include "../Common/Buffer2/VertexBufferCommon.hpp"
#include "../Common/Renderer/RendererCommon.hpp"
#include "../DataType/Color.hpp"
#include "../DataType/MeshData.hpp"

namespace CS460
{
    Mesh::Mesh()
    {
    }

    Mesh::~Mesh()
    {
    }

    void Mesh::Initialize()
    {
    }

    void Mesh::Shutdown()
    {
        if (m_index_buffer != nullptr)
        {
            m_index_buffer->Shutdown();
            delete m_index_buffer;
            m_index_buffer = nullptr;
        }

        if (m_vertex_buffer != nullptr)
        {
            m_vertex_buffer->Shutdown();
            delete m_vertex_buffer;
            m_vertex_buffer = nullptr;
        }

        if (m_instance_buffer != nullptr)
        {
            m_instance_buffer->Shutdown();
            delete m_instance_buffer;
            m_instance_buffer = nullptr;
        }

        if (m_texture_buffer != nullptr)
        {
            m_texture_buffer->Shutdown();
            delete m_texture_buffer;
            m_texture_buffer = nullptr;
        }

        if (m_mesh_data != nullptr && m_mesh_data->b_resource == false)
        {
            m_mesh_data->indices.clear();
            m_mesh_data->vertices.clear();
            delete m_mesh_data;
            m_mesh_data = nullptr;
        }
        m_textures.Clear();
    }

    void Mesh::Bind() const
    {
        if (m_instance_count > 0)
        {
            m_instance_buffer->Update(m_instances);
            m_vertex_buffer->Bind(0, m_instance_buffer);
            m_index_buffer->Bind(0);
            m_textures.Bind();
            m_texture_buffer->Bind();
        }
    }

    void Mesh::Draw() const
    {
        if (m_instance_count > 0)
        {
            m_index_buffer->Draw(m_instance_count);
        }
    }

    void Mesh::CreateBuffer()
    {
        if (m_index_buffer == nullptr)
        {
            m_index_buffer = new IndexBufferCommon();
        }

        if (m_vertex_buffer == nullptr)
        {
            m_vertex_buffer = new VertexBufferCommon();
        }

        if (m_instance_buffer == nullptr)
        {
            m_instance_buffer = new InstanceBufferCommon();
        }

        if (m_texture_buffer == nullptr)
        {
            m_texture_buffer = new ConstantBufferCommon();
            m_texture_buffer->Init(m_renderer, eBindingStage::PixelShader, sizeof(TextureBufferData), 0);
        }

        if (m_mesh_data != nullptr)
        {
            m_index_buffer->Init(m_renderer, m_mesh_data->indices);

            /*  if (m_vertex_type == eShaderType::Color || m_vertex_type == eShaderType::ColorI)
              {
                  std::vector<ColorVertexCommon> vertices;
                  m_mesh_data->Convert(vertices, Color());
                  m_vertex_buffer->Init(m_renderer, vertices, false);
                  vertices.clear();
              }
              else if (m_vertex_type == eShaderType::Texture || m_vertex_type == eShaderType::TextureI)
              {
                  std::vector<TextureVertexCommon> vertices;
                  m_mesh_data->Convert(vertices);
                  m_vertex_buffer->Init(m_renderer, vertices, false);
                  vertices.clear();
              }
              else if (m_vertex_type == eShaderType::Light || m_vertex_type == eShaderType::LightI)
              {
                  m_vertex_buffer->Init(m_renderer, m_mesh_data->vertices, false);
              }*/

            m_vertex_buffer->Init(m_renderer, m_mesh_data->vertices, false);
        }
    }

    void Mesh::AddInstance(const InstanceBufferData& data)
    {
        if (m_instance_count == m_max_count)
        {
            m_max_count = (m_max_count + 1) * 2;
            ResizeInstanceBuffer(m_max_count);
        }

        m_instances[m_instance_count].model    = data.model;
        m_instances[m_instance_count].ambient  = data.ambient;
        m_instances[m_instance_count].diffuse  = data.diffuse;
        m_instances[m_instance_count].specular = data.specular;
        m_instances[m_instance_count].reflect = data.reflect;

        m_instance_count++;
    }

    void Mesh::AddInstance(Transform* transform)
    {
        if (m_instance_count == m_max_count)
        {
            m_max_count = (m_max_count + 1) * 2;
            ResizeInstanceBuffer(m_max_count);
        }

        m_instances[m_instance_count].model    = transform->LocalToWorldMatrix().Transpose();
        m_instances[m_instance_count].ambient  = Color();
        m_instances[m_instance_count].diffuse  = Color();
        m_instances[m_instance_count].specular = Color();
        m_instances[m_instance_count].reflect = Color();
        m_instance_count++;
    }

    void Mesh::AddInstance(Transform* transform, const MaterialColor& color)
    {
        if (m_instance_count == m_max_count)
        {
            //grow instance count
            m_max_count = (m_max_count + 1) * 2;
            ResizeInstanceBuffer(m_max_count);
        }

        m_instances[m_instance_count].model = transform->LocalToWorldMatrix().Transpose();
        m_instances[m_instance_count].ambient  = color.ambient;
        m_instances[m_instance_count].diffuse  = color.diffuse;
        m_instances[m_instance_count].specular = color.specular;
        m_instances[m_instance_count].reflect  = color.reflect;
        m_instance_count++;
    }

    void Mesh::ClearCount(U32 clear_idx)
    {
        m_instance_count = clear_idx;
    }

    void Mesh::SetModelData(MeshData* data)
    {
        m_mesh_data = data;
        CreateBuffer();
    }

    void Mesh::AddTexture(TextureCommon* texture)
    {
        m_textures.PushBack(texture);
    }

    void Mesh::ClearTexture()
    {
        m_textures.Clear();
    }

    void Mesh::RemoveTexture(TextureCommon* texture)
    {
        m_textures.Erase(texture);
    }

    void Mesh::SetRenderer(RendererCommon* renderer)
    {
        m_renderer = renderer;
    }

    void Mesh::ResizeInstanceBuffer(U32 count)
    {
        m_max_count = count;
        m_instances.resize(count);
        m_instance_buffer->Init(m_renderer, m_instances);
    }

    void Mesh::SetMaterialIdentifier(const MaterialIdentifier& material_data)
    {
        m_diffuse_type  = material_data.diffuse_type;
        m_specular_type = material_data.specular_type;
        m_normal_type   = material_data.normal_type;
        m_shader_type   = material_data.shader_type;

        m_textures.Clear();

        //get actual resource data from resource manager.
        if (m_texture_buffer != nullptr)
        {
            TextureBufferData data;
            data.diff_type = m_diffuse_type;
            data.spec_type = m_specular_type;
            data.norm_type = m_normal_type;
            //E5_TODO : need to update user gamma setting
            data.gamma = 2.2f;

            m_texture_buffer->Update(data);
        }
    }

    void Mesh::SetSceneID(size_t model_id, size_t material_id)
    {
        m_model_id    = model_id;
        m_material_id = material_id;
    }

    std::string Mesh::GetShaderType() const
    {
        return m_shader_type;
    }
}
