#include "SkinnedMesh.hpp"

#include "../../../Manager/Component/EngineComponent/AniMeshComponent.hpp"
#include "../../Math/Structure/Transform.hpp"
#include "../Common/Buffer2/ConstantBufferCommon.hpp"
#include "../Common/Buffer2/ConstantBufferData.hpp"
#include "../Common/Buffer2/IndexBufferCommon.hpp"
#include "../Common/Buffer2/VertexBufferCommon.hpp"
#include "../DataType/BufferBindType.hpp"

namespace CS460
{
    SkinnedMesh::SkinnedMesh()
    {
    }

    SkinnedMesh::~SkinnedMesh()
    {
    }

    void SkinnedMesh::Initialize()
    {
    }

    void SkinnedMesh::Update(Real dt)
    {
        m_component->Update(dt);
    }

    void SkinnedMesh::Shutdown()
    {
        if (m_component != nullptr)
        {
            m_component->m_ani_mesh = nullptr;
        }

        if (m_vertex_buffer != nullptr)
        {
            m_vertex_buffer->Shutdown();
            delete m_vertex_buffer;
            m_vertex_buffer = nullptr;
        }

        if (m_matrix_buffer != nullptr)
        {
            m_matrix_buffer->Shutdown();
            delete m_matrix_buffer;
            m_matrix_buffer = nullptr;
        }

        if (m_skinned_buffer != nullptr)
        {
            m_skinned_buffer->Shutdown();
            delete m_skinned_buffer;
            m_skinned_buffer = nullptr;
        }

        for (auto& sub_mesh : m_sub_meshes)
        {
            ShutdownSubMesh(sub_mesh);
        }
    }

   
    void SkinnedMesh::Bind(size_t idx) const
    {
        m_vertex_buffer->Bind(0);
        m_sub_meshes[idx].index_buffer->Bind(0);
        m_sub_meshes[idx].texture_array.Bind();
        m_matrix_buffer->Bind();
        m_skinned_buffer->Bind();
        m_sub_meshes[idx].texture_buffer->Bind();
        m_sub_meshes[idx].material_buffer->Bind();
    }

    void SkinnedMesh::Draw(size_t idx) const
    {
        m_sub_meshes[idx].index_buffer->Draw();
    }

    void SkinnedMesh::CreateBuffer()
    {
        if (m_vertex_buffer == nullptr)
        {
            m_vertex_buffer = new VertexBufferCommon();
        }

        if (m_matrix_buffer == nullptr)
        {
            m_matrix_buffer = new ConstantBufferCommon();
            m_matrix_buffer->Init(m_renderer, eBindingStage::VertexShader, sizeof(MatrixBufferData), 0);
        }

        if (m_skinned_buffer == nullptr)
        {
            m_skinned_buffer = new ConstantBufferCommon();
            m_skinned_buffer->Init(m_renderer, eBindingStage::VertexShader, sizeof(SkinnedBufferData), 1);
        }
    }

    void SkinnedMesh::SetRenderer(RendererCommon* renderer)
    {
        m_renderer = renderer;
    }

    std::string SkinnedMesh::GetShaderType() const
    {
        return m_shader_type;
    }

    size_t SkinnedMesh::SubMeshCount() const
    {
        return m_sub_meshes.size();
    }

    void SkinnedMesh::CreateSubMesh(SubSkinnedMesh& sub_mesh) const
    {
        if (sub_mesh.index_buffer == nullptr)
        {
            sub_mesh.index_buffer = new IndexBufferCommon();
        }

        if (sub_mesh.material_buffer == nullptr)
        {
            sub_mesh.material_buffer = new ConstantBufferCommon();
            sub_mesh.material_buffer->Init(m_renderer, eBindingStage::PixelShader, sizeof(MaterialBufferData), 3);
        }

        if (sub_mesh.texture_buffer == nullptr)
        {
            sub_mesh.texture_buffer = new ConstantBufferCommon();
            sub_mesh.texture_buffer->Init(m_renderer, eBindingStage::PixelShader, sizeof(TextureBufferData), 0);
        }
    }

    void SkinnedMesh::SetUpSubIndexBuffer(SubSkinnedMesh& sub_mesh, const std::vector<U32>& indices) const
    {
        if (sub_mesh.index_buffer != nullptr)
        {
            sub_mesh.index_buffer->Init(m_renderer, indices);
        }
    }

    void SkinnedMesh::SetVertexBuffer(const std::vector<SkinnedVertexCommon>& vertices) const
    {
        if (m_vertex_buffer != nullptr)
        {
            m_vertex_buffer->Init(m_renderer, vertices, false);
        }
    }

    void SkinnedMesh::ShutdownSubMesh(SubSkinnedMesh& sub_mesh) const
    {
        sub_mesh.texture_array.Clear();

        if (sub_mesh.index_buffer != nullptr)
        {
            sub_mesh.index_buffer->Shutdown();
            delete sub_mesh.index_buffer;
        }

        if (sub_mesh.material_buffer != nullptr)
        {
            sub_mesh.material_buffer->Shutdown();
            delete sub_mesh.material_buffer;
        }

        if (sub_mesh.texture_buffer != nullptr)
        {
            sub_mesh.texture_buffer->Shutdown();
            delete sub_mesh.texture_buffer;
        }
    }

    void SkinnedMesh::UpdateSkeletonTransforms(const std::vector<VQSTransform>& final_vqs) const
    {
        m_skinned_buffer->Update(final_vqs);
    }

    void SkinnedMesh::SetSubMeshTexture(TextureCommon* texture, int sub_mesh_idx, int texture_idx)
    {
        if (sub_mesh_idx > -1 && texture_idx > -1)
        {
            if (m_sub_meshes.size() > (size_t)sub_mesh_idx)
            {
                if (m_sub_meshes[sub_mesh_idx].texture_array.Size() > (size_t)texture_idx)
                {
                    m_sub_meshes[sub_mesh_idx].texture_array.Replace(texture, (size_t)texture_idx);
                }
                else
                {
                    m_sub_meshes[sub_mesh_idx].texture_array.PushBack(texture);
                }
            }
        }
    }

    void SkinnedMesh::SetMaterialIdentifier(int sub_mesh_idx, const MaterialIdentifier& material_data)
    {
        if (sub_mesh_idx > -1 && m_sub_meshes.size() > (size_t)sub_mesh_idx)
        {
            m_sub_meshes[sub_mesh_idx].texture_array.Clear();

            //get actual resource data from resource manager.
            if (m_sub_meshes[sub_mesh_idx].texture_buffer != nullptr)
            {
                TextureBufferData data;
                data.diff_type = material_data.diffuse_type;
                data.spec_type = material_data.specular_type;
                data.norm_type = material_data.normal_type;
                //E5_TODO : need to update user gamma setting
                data.gamma = 2.2f;

                m_sub_meshes[sub_mesh_idx].texture_buffer->Update(data);
            }
        }
    }

    void SkinnedMesh::SetMaterialColor(int sub_mesh_idx, const MaterialColor& color_data)
    {
        if (sub_mesh_idx > -1 && m_sub_meshes.size() > (size_t)sub_mesh_idx)
        {
            if (m_sub_meshes[sub_mesh_idx].material_buffer != nullptr)
            {
                MaterialBufferData data;
                data.ambient  = color_data.ambient;
                data.diffuse  = color_data.diffuse;
                data.specular = color_data.specular;
                data.reflect  = color_data.reflect;

                m_sub_meshes[sub_mesh_idx].material_buffer->Update(data);
            }
        }
    }

    void SkinnedMesh::UpdateMatrixBuffer(const Matrix44& view, const Matrix44& proj) const
    {
        MatrixBufferData mvp_buffer;
        mvp_buffer.world = m_transform == nullptr ? Matrix44::Identity() : m_transform->LocalToWorldMatrix();
        mvp_buffer.view  = view;
        mvp_buffer.proj  = proj;
        m_matrix_buffer->Update(mvp_buffer);
    }
}
