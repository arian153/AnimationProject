#include "AniMeshResource.hpp"

#include "../../../System/Animation/Skeleton/Skeleton.hpp"
#include "../../../System/Core/Utility/CoreUtility.hpp"
#include "../../../System/Graphics/Element/AniMesh.hpp"

namespace CS460
{
    AniMeshResource::AniMeshResource(const std::wstring& path)
        : Resource(path)
    {
        m_type = eResourceType::AniMesh;
    }

    AniMeshResource::~AniMeshResource()
    {
    }

    void AniMeshResource::Initialize()
    {
        if (m_file_type_w == L".fbx")
        {
            m_fbx_loader.LoadFbx(m_file_path_w);
            m_b_loaded      = true;
            m_ani_mesh_type = eAniMeshType::FBX;
        }

        if (m_file_type_w == L".bin")
        {
            m_ani_mesh_type = eAniMeshType::BIN;
            m_b_loaded      = m_bin_model.LoadFile(m_file_path_m.c_str());
        }
    }

    void AniMeshResource::Shutdown()
    {
        m_fbx_loader.Shutdown();
        m_b_unloaded = true;
    }

    void AniMeshResource::CopyData(Skeleton* skeleton) const
    {
        if (m_ani_mesh_type == eAniMeshType::BIN)
        {
            //copy bone
            auto bin_skeleton = m_bin_model.GetSkeleton();
            auto bones        = bin_skeleton.GetBones();

            VQSTransform to_bone;
            VQSTransform to_root;

            for (auto& bone : bones)
            {
                const BinParser::Vqs& bfm = bone.GetToBoneFromModel();
                const BinParser::Vqs& mfb = bone.GetToModelFromBone();

                to_bone.Set(
                            Vector3(bfm.v.x, bfm.v.y, bfm.v.z),
                            Quaternion(bfm.q.s, bfm.q.x, bfm.q.y, bfm.q.z),
                            bfm.s);

                to_root.Set(
                            Vector3(mfb.v.x, mfb.v.y, mfb.v.z),
                            Quaternion(mfb.q.s, mfb.q.x, mfb.q.y, mfb.q.z),
                            mfb.s);

                skeleton->CreateBone(to_bone, to_root, bone.GetName(), bone.GetParentID());
            }

            //set up siblings
            for (auto& root : skeleton->m_root_bones)
            {
                skeleton->SetUpSiblingRecursive(root);
            }

            //copy animation clip
            auto bin_animations = m_bin_model.GetAnimations();



        }
        else if (m_ani_mesh_type == eAniMeshType::FBX)
        {
            /* for (auto& bone : m_fbx_loader.m_bones)
             {
                 skeleton->CreateBone(bone->transform, ToString(bone->bone_name), bone->parent_index);
             }
 
             skeleton->m_animation_clips;*/
        }
    }

    void AniMeshResource::CopyData(AniMesh* ani_mesh)
    {
        if (m_fbx_loader.m_meshes.empty())
            return;

        if (m_fbx_loader.m_meshes.size() == 1)
        {
            auto&  mesh_info = m_fbx_loader.m_meshes.front();
            size_t size      = mesh_info.indices.size();
            ani_mesh->m_sub_meshes.resize(size);
            ani_mesh->SetVertexBuffer(mesh_info.vertices);

            for (size_t i = 0; i < size; ++i)
            {
                ani_mesh->CreateSubMesh(ani_mesh->m_sub_meshes[i]);
                ani_mesh->SetUpSubIndexBuffer(ani_mesh->m_sub_meshes[i], mesh_info.indices[i]);
            }
        }
    }
}
