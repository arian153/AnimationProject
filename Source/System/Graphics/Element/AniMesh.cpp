#include "AniMesh.hpp"

#include "../Common/Buffer2/ConstantBufferCommon.hpp"
#include "../Common/Buffer2/ConstantBufferData.hpp"
#include "../Common/Buffer2/IndexBufferCommon.hpp"
#include "../Common/Buffer2/VertexBufferCommon.hpp"
#include "../DataType/BufferBindType.hpp"

namespace CS460
{
    AniMesh::AniMesh()
    {
    }

    AniMesh::~AniMesh()
    {
    }

    void AniMesh::Initialize()
    {
    }

    void AniMesh::Shutdown()
    {
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

        for (auto& sub_mesh : m_sub_meshes)
        {
            ShutdownSubMesh(sub_mesh);
        }
    }

    void AniMesh::Bind() const
    {
    }

    void AniMesh::Draw() const
    {
    }

    void AniMesh::CreateBuffer()
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
    }

    void AniMesh::SetModelData(MeshData* data)
    {
    }

    void AniMesh::AddTexture(TextureCommon* texture)
    {
    }

    void AniMesh::ClearTexture()
    {
    }

    void AniMesh::RemoveTexture(TextureCommon* texture)
    {
    }

    void AniMesh::SetRenderer(RendererCommon* renderer)
    {
        m_renderer = renderer;
    }

    void AniMesh::SetMaterialIdentifier(const MaterialIdentifier& material_data)
    {
    }

    std::string AniMesh::GetShaderType() const
    {
        return m_material_info.shader_type;
    }

    I64 AniMesh::SubMeshCount() const
    {
        return (I64)m_sub_meshes.size();
    }

    void AniMesh::InitSubMesh(SubAniMesh& sub_mesh) const
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

    void AniMesh::ShutdownSubMesh(SubAniMesh& sub_mesh) const
    {
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
}
