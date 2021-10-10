#include "FBXLoader.hpp"

#include <cassert>
#include <filesystem>

#include "../../../System/Core/Utility/CoreUtility.hpp"

#pragma warning(disable : 26451)
#pragma warning(disable : 26495)
#pragma warning(disable : 26812)

namespace CS460
{
    void BoneWeight::AddWeights(U32 index, double weight)
    {
        if (weight <= 0.f)
            return;

        auto findIt = std::find_if(
                                   bone_weights.begin(), bone_weights.end(),
                                   [=](const Pair& p)
                                   {
                                       return p.second < weight;
                                   });

        if (findIt != bone_weights.end())
            bone_weights.insert(findIt, Pair(index, weight));
        else
            bone_weights.push_back(Pair(index, weight));

        // set max weight as 4
        if (bone_weights.size() > 4)
            bone_weights.pop_back();
    }

    void BoneWeight::Normalize()
    {
        double sum = 0.0;
        for (auto& [idx, weight] : bone_weights)
        {
            sum += weight;
        }

        for (auto& [idx, weight] : bone_weights)
        {
            weight = weight / sum;
        }
    }

    FBXLoader::FBXLoader()
    {
    }

    FBXLoader::~FBXLoader()
    {
    }

    void FBXLoader::LoadFbx(const std::wstring& path)
    {
        Import(path);

        LoadBones(m_scene->GetRootNode());
        LoadAnimationInfo();

        ParseNode(m_scene->GetRootNode());
    }

    void FBXLoader::Shutdown()
    {
        if (m_scene != nullptr)
        {
            m_scene->Destroy();
            m_scene = nullptr;
        }

        if (m_manager != nullptr)
        {
            m_manager->Destroy();
            m_manager = nullptr;
        }

        const I32 anim_count = m_anim_names.GetCount();
        for (I32 i = 0; i < anim_count; i++)
        {
            m_anim_names[i]->Clear();
            delete m_anim_names[i];
        }
        m_anim_names.Clear();

        for (auto& bone : m_bones)
        {
            if (bone != nullptr)
            {
                delete bone;
            }
        }
        m_bones.clear();

        for (auto& clips : m_anim_clips)
        {
            if (clips != nullptr)
            {
                clips->key_frames.clear();
                delete clips;
            }
        }
        m_anim_clips.clear();
    }

    I32 FBXLoader::GetMeshCount() const
    {
        return static_cast<I32>(m_meshes.size());
    }

    const FbxMeshInfo& FBXLoader::GetMesh(I32 idx)
    {
        return m_meshes[idx];
    }

    std::vector<FbxBoneInfo*>& FBXLoader::GetBones()
    {
        return m_bones;
    }

    std::vector<FbxAnimClipInfo*>& FBXLoader::GetAnimClip()
    {
        return m_anim_clips;
    }

    void FBXLoader::Import(const std::wstring& path)
    {
        m_manager               = FbxManager::Create();
        FbxIOSettings* settings = FbxIOSettings::Create(m_manager, IOSROOT);
        m_manager->SetIOSettings(settings);
        m_scene    = FbxScene::Create(m_manager, "");
        m_importer = FbxImporter::Create(m_manager, "");

        std::string str_path = ToString(path);
        m_importer->Initialize(str_path.c_str(), -1, m_manager->GetIOSettings());
        m_importer->Import(m_scene);
        m_scene->GetGlobalSettings().SetAxisSystem(FbxAxisSystem::DirectX);

        FbxGeometryConverter fbx_geometry_converter(m_manager);
        fbx_geometry_converter.Triangulate(m_scene, true);
        m_importer->Destroy();
    }

    void FBXLoader::ParseNode(FbxNode* node)
    {
        FbxNodeAttribute* attribute = node->GetNodeAttribute();

        if (attribute)
        {
            switch (attribute->GetAttributeType())
            {
            case FbxNodeAttribute::eMesh:
                LoadMesh(node->GetMesh());
                break;
            default:
                break;
            }
        }

        const U32 material_count = node->GetMaterialCount();
        for (U32 i = 0; i < material_count; ++i)
        {
            FbxSurfaceMaterial* fbx_surface_material = node->GetMaterial(i);
            LoadMaterial(fbx_surface_material);
        }

        const I32 child_count = node->GetChildCount();
        for (I32 i = 0; i < child_count; ++i)
            ParseNode(node->GetChild(i));
    }

    void FBXLoader::LoadMesh(FbxMesh* mesh)
    {
        m_meshes.push_back(FbxMeshInfo());
        FbxMeshInfo& fbx_mesh_info = m_meshes.back();

        fbx_mesh_info.name = ToWString(mesh->GetName());

        const I32 vertex_count = mesh->GetControlPointsCount();
        fbx_mesh_info.vertices.resize(vertex_count);
        fbx_mesh_info.bone_weights.resize(vertex_count);

        // Position
        FbxVector4* control_points = mesh->GetControlPoints();
        for (I32 i = 0; i < vertex_count; ++i)
        {
            fbx_mesh_info.vertices[i].pos.x = static_cast<float>(control_points[i].mData[0]);
            fbx_mesh_info.vertices[i].pos.y = static_cast<float>(control_points[i].mData[2]);
            fbx_mesh_info.vertices[i].pos.z = static_cast<float>(control_points[i].mData[1]);
        }

        const I32 material_count = mesh->GetNode()->GetMaterialCount();
        fbx_mesh_info.indices.resize(material_count);

        FbxGeometryElementMaterial* geometry_element_material = mesh->GetElementMaterial();

        const I32 polygon_size = mesh->GetPolygonSize(0);
        assert(polygon_size == 3);

        U32 arr_idx[3];
        U32 vertex_counter = 0;

        const I32 face_count = mesh->GetPolygonCount();
        for (I32 i = 0; i < face_count; i++)
        {
            for (I32 j = 0; j < 3; j++)
            {
                I32 control_point_index = mesh->GetPolygonVertex(i, j);
                arr_idx[j]              = control_point_index;

                GetNormal(mesh, &fbx_mesh_info, control_point_index, vertex_counter);
                GetTangent(mesh, &fbx_mesh_info, control_point_index, vertex_counter);
                GetUV(mesh, &fbx_mesh_info, control_point_index, mesh->GetTextureUVIndex(i, j));

                vertex_counter++;
            }

            const U32 subset_idx = geometry_element_material->GetIndexArray().GetAt(i);
            fbx_mesh_info.indices[subset_idx].push_back(arr_idx[0]);
            fbx_mesh_info.indices[subset_idx].push_back(arr_idx[2]);
            fbx_mesh_info.indices[subset_idx].push_back(arr_idx[1]);
        }

        // Animation
        LoadAnimationData(mesh, &fbx_mesh_info);
    }

    void FBXLoader::LoadMaterial(FbxSurfaceMaterial* surface_material)
    {
        FbxMaterialInfo material;
        material.name = ToWString(surface_material->GetName());

        material.diffuse  = GetMaterialData(surface_material, FbxSurfaceMaterial::sDiffuse, FbxSurfaceMaterial::sDiffuseFactor);
        material.ambient  = GetMaterialData(surface_material, FbxSurfaceMaterial::sAmbient, FbxSurfaceMaterial::sAmbientFactor);
        material.specular = GetMaterialData(surface_material, FbxSurfaceMaterial::sSpecular, FbxSurfaceMaterial::sSpecularFactor);

        material.diffuse_texture  = GetTextureRelativeName(surface_material, FbxSurfaceMaterial::sDiffuse);
        material.normal_texture   = GetTextureRelativeName(surface_material, FbxSurfaceMaterial::sNormalMap);
        material.specular_texture = GetTextureRelativeName(surface_material, FbxSurfaceMaterial::sSpecular);

        m_meshes.back().materials.push_back(material);
    }

    void FBXLoader::GetNormal(FbxMesh* mesh, FbxMeshInfo* container, I32 idx, I32 vertex_idx)
    {
        if (mesh->GetElementNormalCount() == 0)
            return;

        FbxGeometryElementNormal* normal     = mesh->GetElementNormal();
        U32                       normal_idx = 0;

        if (normal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
        {
            if (normal->GetReferenceMode() == FbxGeometryElement::eDirect)
                normal_idx = vertex_idx;
            else
                normal_idx = normal->GetIndexArray().GetAt(vertex_idx);
        }
        else if (normal->GetMappingMode() == FbxGeometryElement::eByControlPoint)
        {
            if (normal->GetReferenceMode() == FbxGeometryElement::eDirect)
                normal_idx = idx;
            else
                normal_idx = normal->GetIndexArray().GetAt(idx);
        }

        FbxVector4 vec                    = normal->GetDirectArray().GetAt(normal_idx);
        container->vertices[idx].normal.x = static_cast<float>(vec.mData[0]);
        container->vertices[idx].normal.y = static_cast<float>(vec.mData[2]);
        container->vertices[idx].normal.z = static_cast<float>(vec.mData[1]);
    }

    void FBXLoader::GetTangent(FbxMesh* mesh, FbxMeshInfo* mesh_info, I32 idx, I32 vertex_idx)
    {
        if (mesh->GetElementTangentCount() == 0)
        {
            mesh_info->vertices[idx].tangent.x = 1.f;
            mesh_info->vertices[idx].tangent.y = 0.f;
            mesh_info->vertices[idx].tangent.z = 0.f;
            return;
        }

        FbxGeometryElementTangent* tangent     = mesh->GetElementTangent();
        U32                        tangent_idx = 0;

        if (tangent->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
        {
            if (tangent->GetReferenceMode() == FbxGeometryElement::eDirect)
                tangent_idx = vertex_idx;
            else
                tangent_idx = tangent->GetIndexArray().GetAt(vertex_idx);
        }
        else if (tangent->GetMappingMode() == FbxGeometryElement::eByControlPoint)
        {
            if (tangent->GetReferenceMode() == FbxGeometryElement::eDirect)
                tangent_idx = idx;
            else
                tangent_idx = tangent->GetIndexArray().GetAt(idx);
        }

        FbxVector4 vec                     = tangent->GetDirectArray().GetAt(tangent_idx);
        mesh_info->vertices[idx].tangent.x = static_cast<float>(vec.mData[0]);
        mesh_info->vertices[idx].tangent.y = static_cast<float>(vec.mData[2]);
        mesh_info->vertices[idx].tangent.z = static_cast<float>(vec.mData[1]);
    }

    void FBXLoader::GetUV(FbxMesh* mesh, FbxMeshInfo* mesh_info, I32 idx, I32 uv_index)
    {
        FbxVector2 uv                 = mesh->GetElementUV()->GetDirectArray().GetAt(uv_index);
        mesh_info->vertices[idx].uv.x = static_cast<float>(uv.mData[0]);
        mesh_info->vertices[idx].uv.y = 1.f - static_cast<float>(uv.mData[1]);
    }

    Color FBXLoader::GetMaterialData(FbxSurfaceMaterial* surface, const char* material_name, const char* factor_name)
    {
        FbxDouble3 material;
        FbxDouble  factor = 0.f;

        FbxProperty material_property = surface->FindProperty(material_name);
        FbxProperty factor_property   = surface->FindProperty(factor_name);

        if (material_property.IsValid() && factor_property.IsValid())
        {
            material = material_property.Get<FbxDouble3>();
            factor   = factor_property.Get<FbxDouble>();
        }

        Color ret = Color(
                          static_cast<float>(material.mData[0] * factor),
                          static_cast<float>(material.mData[1] * factor),
                          static_cast<float>(material.mData[2] * factor),
                          static_cast<float>(factor));

        return ret;
    }

    std::wstring FBXLoader::GetTextureRelativeName(FbxSurfaceMaterial* surface, const char* material_property)
    {
        std::string name;

        FbxProperty texture_property = surface->FindProperty(material_property);
        if (texture_property.IsValid())
        {
            U32 count = texture_property.GetSrcObjectCount();

            if (1 <= count)
            {
                FbxFileTexture* texture = texture_property.GetSrcObject<FbxFileTexture>(0);
                if (texture)
                    name = texture->GetRelativeFileName();
            }
        }

        return ToWString(name);
    }

    void FBXLoader::LoadBones(FbxNode* node)
    {
        LoadBones(node, 0, -1);
    }

    void FBXLoader::LoadBones(FbxNode* node, I32 idx, I32 parent_idx)
    {
        FbxNodeAttribute* attribute = node->GetNodeAttribute();

        if (attribute && attribute->GetAttributeType() == FbxNodeAttribute::eSkeleton)
        {
            FbxBoneInfo* bone  = new FbxBoneInfo();
            bone->bone_name    = ToWString(node->GetName());
            bone->parent_index = parent_idx;
            m_bones.push_back(bone);
        }

        I32 child_count = node->GetChildCount();
        for (I32 i = 0; i < child_count; i++)
        {
            LoadBones(node->GetChild(i), static_cast<I32>(m_bones.size()), idx);
        }
    }

    void FBXLoader::LoadAnimationInfo()
    {
        m_scene->FillAnimStackNameArray(OUT m_anim_names);

        const I32 anim_count = m_anim_names.GetCount();
        for (I32 i = 0; i < anim_count; i++)
        {
            FbxAnimStack* fbx_anim_stack = m_scene->FindMember<FbxAnimStack>(m_anim_names[i]->Buffer());
            if (fbx_anim_stack == nullptr)
                continue;

            FbxAnimClipInfo* anim_clip = new FbxAnimClipInfo();
            anim_clip->name            = ToWString(fbx_anim_stack->GetName());
            anim_clip->key_frames.resize(m_bones.size());

            FbxTakeInfo* fbx_take_info = m_scene->GetTakeInfo(fbx_anim_stack->GetName());
            anim_clip->start_time      = fbx_take_info->mLocalTimeSpan.GetStart();
            anim_clip->end_time        = fbx_take_info->mLocalTimeSpan.GetStop();
            anim_clip->mode            = m_scene->GetGlobalSettings().GetTimeMode();

            m_anim_clips.push_back(anim_clip);
        }
    }

    void FBXLoader::LoadAnimationData(FbxMesh* mesh, FbxMeshInfo* mesh_info)
    {
        const I32 skin_count = mesh->GetDeformerCount(FbxDeformer::eSkin);
        if (skin_count <= 0 || m_anim_clips.empty())
            return;

        mesh_info->has_animation = true;

        for (I32 i = 0; i < skin_count; i++)
        {
            FbxSkin* fbx_skin = static_cast<FbxSkin*>(mesh->GetDeformer(i, FbxDeformer::eSkin));

            if (fbx_skin)
            {
                FbxSkin::EType type = fbx_skin->GetSkinningType();
                if (FbxSkin::eRigid == type || FbxSkin::eLinear)
                {
                    I32 cluster_count = fbx_skin->GetClusterCount();
                    for (I32 j = 0; j < cluster_count; j++)
                    {
                        FbxCluster* cluster = fbx_skin->GetCluster(j);
                        if (cluster->GetLink() == nullptr)
                            continue;

                        I32 bone_idx = FindBoneIndex(cluster->GetLink()->GetName());
                        assert(bone_idx >= 0);

                        FbxAMatrix node_transform = GetTransform(mesh->GetNode());
                        LoadBoneWeight(cluster, bone_idx, mesh_info);
                        LoadOffsetMatrix(cluster, node_transform, bone_idx, mesh_info);

                        I32 anim_count = m_anim_names.Size();
                        for (I32 k = 0; k < anim_count; k++)
                        {
                            LoadKeyframe(k, mesh->GetNode(), cluster, node_transform, bone_idx, mesh_info);
                        }
                    }
                }
            }
        }

        FillBoneWeight(mesh, mesh_info);
    }

    void FBXLoader::FillBoneWeight(FbxMesh* mesh, FbxMeshInfo* mesh_info)
    {
        const I32 size = static_cast<I32>(mesh_info->bone_weights.size());
        for (I32 v = 0; v < size; v++)
        {
            BoneWeight& bone_weight = mesh_info->bone_weights[v];
            bone_weight.Normalize();

            float anim_bone_index[4]  = {};
            float anim_bone_weight[4] = {};

            const I32 weightCount = static_cast<I32>(bone_weight.bone_weights.size());
            for (I32 w = 0; w < weightCount; w++)
            {
                anim_bone_index[w]  = static_cast<float>(bone_weight.bone_weights[w].first);
                anim_bone_weight[w] = static_cast<float>(bone_weight.bone_weights[w].second);
            }

            memcpy(&mesh_info->vertices[v].indices, anim_bone_index, sizeof(Color));
            memcpy(&mesh_info->vertices[v].weights, anim_bone_weight, sizeof(Color));
        }
    }

    Vector3 FBXLoader::GetTranslation(const FbxAMatrix& transform)
    {
        FbxVector4 translation = transform.GetT();
        return Vector3((Real)translation.mData[0], (Real)translation.mData[1], (Real)translation.mData[2]);
    }

    Quaternion FBXLoader::GetRotation(const FbxAMatrix& transform)
    {
        FbxQuaternion rotation = transform.GetQ();
        return Quaternion((Real)rotation.mData[3], (Real)rotation.mData[0], (Real)rotation.mData[1], (Real)rotation.mData[2]);
    }

    Real FBXLoader::GetScale(const FbxAMatrix& transform)
    {
        FbxVector4 scale = transform.GetT();
        return (Real)scale.mData[0];
    }

    VQSTransform FBXLoader::GetVQSTransform(const FbxAMatrix& transform)
    {
        FbxVector4    translation = transform.GetT();
        FbxQuaternion rotation    = transform.GetQ();
        FbxVector4    scale       = transform.GetT();

        Vector3    v = Vector3((Real)translation.mData[0], (Real)translation.mData[1], (Real)translation.mData[2]);
        Quaternion q((Real)rotation.mData[3], (Real)rotation.mData[0], (Real)rotation.mData[1], (Real)rotation.mData[2]);
        return VQSTransform(v, q, (Real)scale.mData[0]);
    }

    void FBXLoader::LoadBoneWeight(FbxCluster* cluster, I32 bone_idx, FbxMeshInfo* mesh_info)
    {
        const I32 indices_count = cluster->GetControlPointIndicesCount();
        for (I32 i = 0; i < indices_count; i++)
        {
            double weight  = cluster->GetControlPointWeights()[i];
            I32    vtx_idx = cluster->GetControlPointIndices()[i];
            mesh_info->bone_weights[vtx_idx].AddWeights(bone_idx, weight);
        }
    }

    void FBXLoader::LoadOffsetMatrix(FbxCluster* cluster, const FbxAMatrix& node_transform, I32 bone_idx, FbxMeshInfo* meshInfo)
    {
        FbxAMatrix cluster_transform;
        FbxAMatrix cluster_link_transform;
        // The transformation of the mesh at binding time 
        cluster->GetTransformMatrix(cluster_transform);
        // The transformation of the cluster(joint) at binding time from joint space to world space 
        cluster->GetTransformLinkMatrix(cluster_link_transform);

        FbxVector4 V0 = {1, 0, 0, 0};
        FbxVector4 V1 = {0, 0, 1, 0};
        FbxVector4 V2 = {0, 1, 0, 0};
        FbxVector4 V3 = {0, 0, 0, 1};

        FbxAMatrix reflect;
        reflect[0] = V0;
        reflect[1] = V1;
        reflect[2] = V2;
        reflect[3] = V3;

        FbxAMatrix offset = cluster_link_transform.Inverse() * cluster_transform;
        offset            = reflect * offset * reflect;

        m_bones[bone_idx]->offset = offset.Transpose();
    }

    void FBXLoader::LoadKeyframe(I32 anim_index, FbxNode* node, FbxCluster* cluster, const FbxAMatrix& node_transform, I32 bone_idx, FbxMeshInfo* meshInfo)
    {
        if (m_anim_clips.empty())
            return;

        FbxVector4 v1 = {1, 0, 0, 0};
        FbxVector4 v2 = {0, 0, 1, 0};
        FbxVector4 v3 = {0, 1, 0, 0};
        FbxVector4 v4 = {0, 0, 0, 1};
        FbxAMatrix reflect;
        reflect.mData[0] = v1;
        reflect.mData[1] = v2;
        reflect.mData[2] = v3;
        reflect.mData[3] = v4;

        FbxTime::EMode time_mode = m_scene->GetGlobalSettings().GetTimeMode();

        FbxAnimStack* anim_stack = m_scene->FindMember<FbxAnimStack>(m_anim_names[anim_index]->Buffer());
        m_scene->SetCurrentAnimationStack(OUT anim_stack);

        FbxLongLong start_frame = m_anim_clips[anim_index]->start_time.GetFrameCount(time_mode);
        FbxLongLong end_frame   = m_anim_clips[anim_index]->end_time.GetFrameCount(time_mode);

        for (FbxLongLong frame = start_frame; frame < end_frame; frame++)
        {
            FbxKeyFrameInfo key_frame_info = {};
            FbxTime         time           = 0;

            time.SetFrame(frame, time_mode);

            FbxAMatrix mat_from_node = node->EvaluateGlobalTransform(time);
            FbxAMatrix mat_transform = mat_from_node.Inverse() * cluster->GetLink()->EvaluateGlobalTransform(time);
            mat_transform            = reflect * mat_transform * reflect;

            key_frame_info.time          = time.GetSecondDouble();
            key_frame_info.transform_mat = mat_transform;

            m_anim_clips[anim_index]->key_frames[bone_idx].push_back(key_frame_info);
        }
    }

    I32 FBXLoader::FindBoneIndex(std::string name)
    {
        std::wstring bone_name = std::wstring(name.begin(), name.end());

        for (U32 i = 0; i < m_bones.size(); ++i)
        {
            if (m_bones[i]->bone_name == bone_name)
                return i;
        }

        return -1;
    }

    FbxAMatrix FBXLoader::GetTransform(FbxNode* node)
    {
        const FbxVector4 translation = node->GetGeometricTranslation(FbxNode::eSourcePivot);
        const FbxVector4 rotation    = node->GetGeometricRotation(FbxNode::eSourcePivot);
        const FbxVector4 scaling     = node->GetGeometricScaling(FbxNode::eSourcePivot);
        return FbxAMatrix(translation, rotation, scaling);
    }
}
