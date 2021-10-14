#pragma once

#include "../../Core/Utility/CoreDef.hpp"
#include "../Common/Texture/TextureArrayCommon.hpp"
#include "../Common/Vertex/SkinnedVertexCommon.hpp"
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
        TextureArrayCommon    texture_array;
        MaterialIdentifier    material_info;
        MaterialColor         material_color;
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

        void SetRenderer(RendererCommon* renderer);

        std::string GetShaderType() const;

        I64 SubMeshCount() const;

        void SetVertexBuffer(const std::vector<SkinnedVertexCommon>& vertices) const;
        void CreateSubMesh(SubAniMesh& sub_mesh) const;
        void SetUpSubIndexBuffer(SubAniMesh& sub_mesh, const std::vector<U32>& indices) const;

        void ShutdownSubMesh(SubAniMesh& sub_mesh) const;

    private:
        friend class AniMeshComponent;
        friend class AniMeshResource;

    private:
        AniMeshComponent* m_component   = nullptr;
        RendererCommon*   m_renderer    = nullptr;
        std::string       m_shader_type = "";

        VertexBufferCommon*     m_vertex_buffer = nullptr;
        ConstantBufferCommon*   m_matrix_buffer = nullptr;
        std::vector<SubAniMesh> m_sub_meshes;
    };
}
