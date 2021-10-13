#pragma once

#include <algorithm>
#include <vector>
#include <FBX/fbxsdk.h>

#include "../../../System/Graphics/Common/Vertex/SkinnedVertexCommon.hpp"
#include "../../../System/Graphics/DataType/Color.hpp"
#include "../../../System/Math/Algebra/Quaternion.hpp"
#include "../../../System/Math/Algebra/Vector2.hpp"
#include "../../../System/Math/Algebra/Vector3.hpp"
#include "../../../System/Math/Algebra/Vector4.hpp"
#include "../../../System/Math/Structure/VQSTransform.hpp"

namespace CS460
{
    struct FbxMaterialInfo
    {
        Color        diffuse;
        Color        ambient;
        Color        specular;
        std::wstring name;
        std::wstring diffuse_texture;
        std::wstring normal_texture;
        std::wstring specular_texture;
    };

    struct BoneWeight
    {
    public:
        void AddWeights(U32 index, double weight);
        void Normalize();

    public:
        using Pair = std::pair<I32, double>;
        std::vector<Pair> bone_weights;
    };

    struct FbxMeshInfo
    {
        std::wstring                     name;
        std::vector<SkinnedVertexCommon> vertices;
        std::vector<std::vector<U32>>    indices;
        std::vector<FbxMaterialInfo>     materials;
        std::vector<BoneWeight>          bone_weights;
        bool                             has_animation;
        bool                             has_tangent;
    };

    struct FbxKeyFrameInfo
    {
        FbxAMatrix transform_mat;
        double     time;
    };

    struct FbxBoneInfo
    {
        std::wstring bone_name;
        I32          parent_index;
        FbxAMatrix   offset;
        VQSTransform transform;
    };

    struct FbxAnimClipInfo
    {
        std::wstring                              name;
        FbxTime                                   start_time;
        FbxTime                                   end_time;
        FbxTime::EMode                            mode;
        std::vector<std::vector<FbxKeyFrameInfo>> key_frames;
    };

    class FBXLoader
    {
    public:
        FBXLoader();
        ~FBXLoader();

    public:
        void LoadFbx(const std::wstring& path);
        void Shutdown();

    public:
        I32                            GetMeshCount() const;
        const FbxMeshInfo&             GetMesh(I32 idx);
        std::vector<FbxBoneInfo*>&     GetBones();
        std::vector<FbxAnimClipInfo*>& GetAnimClip();

    private:
        void Import(const std::wstring& path);

        void ParseNode(FbxNode* root);
        void LoadMesh(FbxMesh* mesh);
        void LoadMaterial(FbxSurfaceMaterial* surface_material);

        void         GetNormal(FbxMesh* mesh, FbxMeshInfo* container, I32 idx, I32 vertex_idx);
        void         GetTangent(FbxMesh* mesh, FbxMeshInfo* container, I32 idx, I32 vertex_idx);
        void         GetUV(FbxMesh* mesh, FbxMeshInfo* container, I32 idx, I32 vertex_idx);
        Color        GetMaterialData(FbxSurfaceMaterial* surface, const char* material_name, const char* factor_name);
        std::wstring GetTextureRelativeName(FbxSurfaceMaterial* surface, const char* material_property);

        // Animation
        void LoadBones(FbxNode* node);
        void LoadBones(FbxNode* node, I32 idx, I32 parent_idx);
        void LoadAnimationInfo();

        void LoadAnimationData(FbxMesh* mesh, FbxMeshInfo* mesh_info);
        void LoadBoneWeight(FbxCluster* cluster, I32 bone_idx, FbxMeshInfo* mesh_info);
        void LoadOffsetMatrix(FbxCluster* cluster, const FbxAMatrix& node_transform, I32 bone_idx, FbxMeshInfo* mesh_info);
        void LoadKeyframe(I32 anim_index, FbxNode* node, FbxCluster* cluster, const FbxAMatrix& node_transform, I32 bone_idx, FbxMeshInfo* container);

        I32        FindBoneIndex(std::string name);
        FbxAMatrix GetTransform(FbxNode* node);

        void FillBoneWeight(FbxMesh* mesh, FbxMeshInfo* mesh_info);

        Vector3      GetTranslation(const FbxAMatrix& transform);
        Quaternion   GetRotation(const FbxAMatrix& transform);
        Real         GetScale(const FbxAMatrix& transform);
        VQSTransform GetVQSTransform(const FbxAMatrix& transform);

    private:
        friend class AniMeshResource;

    private:
        FbxManager*  m_manager  = nullptr;
        FbxScene*    m_scene    = nullptr;
        FbxImporter* m_importer = nullptr;
        std::wstring m_resource_directory;

        std::vector<FbxMeshInfo>      m_meshes;
        std::vector<FbxBoneInfo*>     m_bones;
        std::vector<FbxAnimClipInfo*> m_anim_clips;
        FbxArray<FbxString*>          m_anim_names;
    };
}
