#pragma once

#include "../../Core/Utility/CoreDef.hpp"
#include "../../Math/Structure/VQSTransform.hpp"
#include "../Common/Texture/TextureArrayCommon.hpp"
#include "../Common/Vertex/SkinnedVertexCommon.hpp"
#include "../DataType/MaterialData.hpp"

namespace CS460
{
    class Transform;
    class AniMeshComponent;
    class VertexBufferCommon;
    class IndexBufferCommon;
    class ConstantBufferCommon;
    class RendererCommon;
    class MeshData;

    struct SubSkinnedMesh
    {
        IndexBufferCommon*    index_buffer    = nullptr;
        ConstantBufferCommon* material_buffer = nullptr;
        ConstantBufferCommon* texture_buffer  = nullptr;
        TextureArrayCommon    texture_array;
    };

    class SkinnedMesh
    {
    public:
        SkinnedMesh();
        ~SkinnedMesh();

        void Initialize();
        void Update(Real dt);
        void Shutdown();

        void Bind(size_t idx) const;
        void Draw(size_t idx) const;
        void CreateBuffer();

        void SetRenderer(RendererCommon* renderer);

        std::string GetShaderType() const;

        size_t SubMeshCount() const;

        void SetVertexBuffer(const std::vector<SkinnedVertexCommon>& vertices) const;
        void CreateSubMesh(SubSkinnedMesh& sub_mesh) const;
        void SetUpSubIndexBuffer(SubSkinnedMesh& sub_mesh, const std::vector<U32>& indices) const;
        void ShutdownSubMesh(SubSkinnedMesh& sub_mesh) const;

        void UpdateSkeletonTransforms(const std::vector<VQSTransform>& final_vqs) const;
        void SetSubMeshTexture(TextureCommon* texture, int sub_mesh_idx, int texture_idx);
        void SetMaterialIdentifier(int sub_mesh_idx, const MaterialIdentifier& material_data);
        void SetMaterialColor(int sub_mesh_idx, const MaterialColor& color_data);
        void UpdateMatrixBuffer(const Matrix44& view, const Matrix44& proj) const;

    private:
        friend class AniMeshComponent;
        friend class AniMeshResource;

    private:
        AniMeshComponent* m_component   = nullptr;
        RendererCommon*   m_renderer    = nullptr;
        std::string       m_shader_type = "";
        Transform*        m_transform   = nullptr;

        VertexBufferCommon*         m_vertex_buffer  = nullptr;
        ConstantBufferCommon*       m_matrix_buffer  = nullptr;
        ConstantBufferCommon*       m_skinned_buffer = nullptr;
        std::vector<SubSkinnedMesh> m_sub_meshes;
    };
}
