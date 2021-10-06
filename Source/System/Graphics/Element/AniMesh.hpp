#pragma once

#include "../../Core/Utility/CoreDef.hpp"
#include "../Common/Texture/TextureArrayCommon.hpp"
#include "../DataType/MaterialData.hpp"

namespace CS460
{
    class AniMeshComponent;
    class VertexBufferCommon;
    class IndexBufferCommon;
    class ConstantBufferCommon;
    class RendererCommon;
    class MeshData;

    struct SubAniMesh
    {
        IndexBufferCommon*    index_buffer    = nullptr;
        ConstantBufferCommon* material_buffer = nullptr;
        ConstantBufferCommon* texture_buffer  = nullptr;
    };

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

        void        SetMaterialIdentifier(const MaterialIdentifier& material_data);
        std::string GetShaderType() const;

        I64 SubMeshCount() const;

        void InitSubMesh(SubAniMesh& sub_mesh) const;
        void ShutdownSubMesh(SubAniMesh& sub_mesh) const;

    private:
        friend class AniMeshComponent;

    private:
        AniMeshComponent* m_component = nullptr;
        RendererCommon*   m_renderer  = nullptr;
        MeshData*         m_mesh_data = nullptr;

        TextureArrayCommon m_textures;
        MaterialIdentifier m_material_info;
        MaterialColor      m_material_color;

        VertexBufferCommon*   m_vertex_buffer = nullptr;
        ConstantBufferCommon* m_matrix_buffer = nullptr;

        std::vector<SubAniMesh> m_sub_meshes;
    };
}
