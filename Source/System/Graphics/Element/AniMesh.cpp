#include "AniMesh.hpp"

#include "../../../Manager/Component/EngineComponent/AniMeshComponent.hpp"
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

    void AniMesh::SetRenderer(RendererCommon* renderer)
    {
        m_renderer = renderer;
    }

    std::string AniMesh::GetShaderType() const
    {
        return m_shader_type;
    }

    I64 AniMesh::SubMeshCount() const
    {
        return (I64)m_sub_meshes.size();
    }

    void AniMesh::CreateSubMesh(SubAniMesh& sub_mesh) const
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

    void AniMesh::SetUpSubIndexBuffer(SubAniMesh& sub_mesh, const std::vector<U32>& indices) const
    {
        if (sub_mesh.index_buffer != nullptr)
        {
            sub_mesh.index_buffer->Init(m_renderer, indices);
        }
    }

    void AniMesh::SetVertexBuffer(const std::vector<SkinnedVertexCommon>& vertices) const
    {
        if (m_vertex_buffer != nullptr)
        {
            m_vertex_buffer->Init(m_renderer, vertices, false);
        }
    }

    void AniMesh::ShutdownSubMesh(SubAniMesh& sub_mesh) const
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
}
