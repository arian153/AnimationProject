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
       /* if (m_file_type_w == L".fbx")
        {
            m_fbx_loader.LoadFbx(m_file_path_w);
            m_b_loaded      = true;
            m_ani_mesh_type = eAniMeshType::FBX;
        }*/

        if (m_file_type_w == L".bin")
        {
            m_ani_mesh_type = eAniMeshType::BIN;
            m_b_loaded      = m_bin_model.LoadFile(m_file_path_m.c_str());

            const BinParser::Mesh& mesh     = m_bin_model.GetMesh();
            auto                   vertices = mesh.GetVertexes();
            U32                    size     = mesh.GetVertexCount();

            m_vertices.resize((size_t)size);
            for (U32 i = 0; i < size; ++i)
            {
                m_vertices[i].position  = ToVector3(vertices[i].Position_);
                m_vertices[i].tex       = ToVector2(vertices[i].UV_);
                m_vertices[i].normal    = ToVector3(vertices[i].Normal_);
                m_vertices[i].indices.x = (I32)vertices[i].BoneIndexes_[0];
                m_vertices[i].indices.y = (I32)vertices[i].BoneIndexes_[1];
                m_vertices[i].indices.z = (I32)vertices[i].BoneIndexes_[2];
                m_vertices[i].indices.w = (I32)vertices[i].BoneIndexes_[3];
                m_vertices[i].weights.x = vertices[i].BoneWeights_[0];
                m_vertices[i].weights.y = vertices[i].BoneWeights_[1];
                m_vertices[i].weights.z = vertices[i].BoneWeights_[2];
                m_vertices[i].weights.w = vertices[i].BoneWeights_[3];
                m_vertices[i].CalculateTangentAndBinormal();
            }
        }
    }

    void AniMeshResource::Shutdown()
    {
        //m_fbx_loader.Shutdown();
        m_vertices.clear();
        m_b_unloaded = true;
    }

    void AniMeshResource::CopyData(Skeleton* skeleton) const
    {
        if (m_ani_mesh_type == eAniMeshType::BIN)
        {
            //copy bone
            auto bin_skeleton = m_bin_model.GetSkeleton();
            auto bones        = bin_skeleton.GetBones();

            for (auto& bone : bones)
            {
                const BinParser::Vqs& to_bone = bone.GetToBoneFromModel();
                const BinParser::Vqs& to_root = bone.GetToModelFromBone();

                skeleton->CreateBone(ToVQS(to_bone), ToVQS(to_root), bone.GetName(), bone.GetParentID());
            }

            //set up siblings
            for (auto& root : skeleton->m_root_bones)
            {
                skeleton->SetUpSiblingRecursive(root);
            }

            //copy animation clip
            auto bin_animations = m_bin_model.GetAnimations();

            size_t size = bin_animations.size();

            for (size_t i = 0; i < size; ++i)
            {
                auto clip      = skeleton->CreateAnimationClip();
                clip->duration = bin_animations[i].GetDuration();
                clip->name     = "Animation Clip " + std::to_string(i);

                if (clip->bone_count == (size_t)bin_animations[i].GetTrackCount())
                {
                    size_t bone_count = clip->bone_count;
                    auto   bin_track  = bin_animations[i].GetTracks();
                    size_t track_size = bin_track.front().GetKeyFrameCount();
                    clip->track.resize(track_size);

                    for (size_t j = 0; j < bone_count; ++j)
                    {
                        for (size_t k = 0; k < track_size; ++k)
                        {
                            const BinParser::KeyFrame& bin_data = bin_track[j].GetKeyFrame((U32)k);
                            clip->track[k].pose.resize(bone_count);
                            clip->track[k].pose[j].to_parent = ToVQS(bin_data.GetToParentFromBone());
                            clip->track[k].pose[j].time      = bin_data.GetTime();
                        }
                    }
                }
            }
        }
 //       else if (m_ani_mesh_type == eAniMeshType::FBX)
 //       {
 //           /* for (auto& bone : m_fbx_loader.m_bones)
 //            {
 //                skeleton->CreateBone(bone->transform, ToString(bone->bone_name), bone->parent_index);
 //            }
 //
 //            skeleton->m_animation_clips;*/
 //       }
    }

    void AniMeshResource::CopyData(AniMesh* ani_mesh)
    {
        if (m_ani_mesh_type == eAniMeshType::BIN)
        {
            const BinParser::Mesh& mesh = m_bin_model.GetMesh();

            ani_mesh->m_sub_meshes.resize(1);
            ani_mesh->SetVertexBuffer(m_vertices);
            ani_mesh->CreateSubMesh(ani_mesh->m_sub_meshes[0]);
            ani_mesh->SetUpSubIndexBuffer(ani_mesh->m_sub_meshes[0], mesh.GetIndexes());
            //no material info from BIN file.
        }
      /*  else if (m_ani_mesh_type == eAniMeshType::FBX)
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
        }*/
    }

    VQSTransform AniMeshResource::ToVQS(const BinParser::Vqs& vqs) const
    {
        return VQSTransform(
                            Vector3(vqs.v.x, vqs.v.y, vqs.v.z),
                            Quaternion(vqs.q.s, vqs.q.x, vqs.q.y, vqs.q.z),
                            vqs.s);
    }

    Vector2 AniMeshResource::ToVector2(const BinParser::Vector2& v2) const
    {
        return Vector2(v2.x, v2.y);
    }

    Vector3 AniMeshResource::ToVector3(const BinParser::Vector3& v3) const
    {
        return Vector3(v3.x, v3.y, v3.z);
    }
}
