#pragma once

#include "../../Core/Utility/CoreDef.hpp"
#include "../Common/Texture/TextureArrayCommon.hpp"
#include "../DataType/MaterialData.hpp"

namespace CS460
{
    class VertexBufferCommon;
    class IndexBufferCommon;
    class ConstantBufferCommon;
    class RendererCommon;
    class MeshData;

    class AniMesh
    {
    public:
        AniMesh();
        ~AniMesh();

        void Initialize();
        void Shutdown();

        void Bind() const;
        void Draw() const;
        void CreateBuffer();

        void SetModelData(MeshData* data);
        void AddTexture(TextureCommon* texture);
        void ClearTexture();
        void RemoveTexture(TextureCommon* texture);
        void SetRenderer(RendererCommon* renderer);

        void SetMaterialIdentifier(const MaterialIdentifier& material_data);
        void SetSceneID(size_t model_id, size_t material_id);

        std::string GetShaderType() const;
    private:
        RendererCommon*    m_renderer  = nullptr;
        MeshData*          m_mesh_data = nullptr;
        TextureArrayCommon m_textures;

        size_t      m_material_id   = (size_t)-1;
        size_t      m_model_id      = (size_t)-1;
        int         m_diffuse_type  = -1;
        int         m_specular_type = -1;
        int         m_normal_type   = -1;
        std::string m_shader_type   = "";

        VertexBufferCommon*             m_vertex_buffer  = nullptr;
        ConstantBufferCommon*           m_texture_buffer = nullptr;
        ConstantBufferCommon*           m_matrix_buffer  = nullptr;
        std::vector<IndexBufferCommon*> m_index_buffers;
    };
}
