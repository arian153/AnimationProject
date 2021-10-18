#include "Skeleton.hpp"

#include "Bone.hpp"
#include "../../../Manager/Component/EngineComponent/AniMeshComponent.hpp"
#include "../../../Manager/Resource/ResourceType/AniMeshResource.hpp"
#include "../../Graphics/Utility/PrimitiveRenderer.hpp"

namespace CS460
{
    Skeleton::Skeleton()
    {
    }

    Skeleton::~Skeleton()
    {
    }

    void Skeleton::Initialize()
    {
        //CreateSample();
    }

    void Skeleton::Update(Real dt)
    {
        //update by keyframe
        if (m_clip_id != Core::I64_MAX)
        {
            auto& clip = m_animation_clips[m_clip_id];
            clip->Update(dt);
        }
    }

    void Skeleton::Shutdown()
    {
        if (m_component != nullptr)
        {
            m_component->m_skeleton = nullptr;
        }

        for (auto& bone : m_bones)
        {
            bone->Shutdown();
            delete bone;
        }
        m_bones.clear();

        for (auto& clip : m_animation_clips)
        {
            clip->Shutdown();
            delete clip;
        }
        m_animation_clips.clear();
    }

    void Skeleton::Draw(PrimitiveRenderer* renderer) const
    {
        if (!m_root_bones.empty() && m_b_draw)
        {
            DrawFinalVQS(renderer);
        }
    }

    void Skeleton::SetAniMeshResource(AniMeshResource* resource)
    {
        resource->CopyData(this);
    }

    void Skeleton::UpdateKeyFrame(std::vector<KeyFrame>& key_frames)
    {
        I32 size = (I32)key_frames.size();

        if (size != m_bones.size())
        {
            return;
        }

        std::vector<VQSTransform> to_roots;
        to_roots.resize(size);
        m_final_vqs.resize(size);

        for (I32 i = 0; i < size; ++i)
        {
            I32          parent_idx     = m_bones[i]->m_parent_idx;
            VQSTransform parent_to_root = parent_idx >= 0 ? key_frames[parent_idx].to_parent : VQSTransform();
            to_roots[i]                 = Concatenate(key_frames[i].to_parent, parent_to_root);
            to_roots[i].rotation.SetNormalize();
        }

        for (I32 i = 0; i < size; ++i)
        {
            m_final_vqs[i] = Concatenate(m_bones[i]->m_to_root, to_roots[i]);
            m_final_vqs[i].rotation.SetNormalize();
        }
    }

    void Skeleton::DrawRecursive(PrimitiveRenderer* renderer, Bone* bone) const
    {
        //only call root bone
        bone->Draw(renderer, m_color);
        Vector3 bone_pos = bone->m_to_bone.position;
        for (auto& child : bone->m_children)
        {
            Vector3 child_pos = Concatenate(bone->m_to_bone, child->m_to_bone).position;
            renderer->DrawSegment(bone_pos, child_pos, m_color);
            DrawRecursive(renderer, child, bone->m_to_bone);
        }
    }

    void Skeleton::DrawRecursive(PrimitiveRenderer* renderer, Bone* bone, const VQSTransform& parent) const
    {
        bone->Draw(renderer, m_color, parent);
        VQSTransform offset   = Concatenate(parent, bone->m_to_bone);
        Vector3      bone_pos = offset.position;

        for (auto& child : bone->m_children)
        {
            Vector3 child_pos = Concatenate(offset, child->m_to_bone).position;
            renderer->DrawSegment(bone_pos, child_pos, m_color);
            DrawRecursive(renderer, child, offset);
        }
    }

    Bone* Skeleton::CreateBone(const VQSTransform& to_bone, const VQSTransform& to_root, const std::string& name, Bone* parent)
    {
        Bone* created       = new Bone();
        created->m_skeleton = this;
        created->m_to_bone  = to_bone;
        created->m_to_root  = to_root;
        created->m_name     = name;

        if (m_bones.empty())
        {
            created->m_parent     = nullptr;
            created->m_parent_idx = -1;
        }
        else
        {
            created->m_parent     = parent;
            created->m_parent_idx = parent == nullptr ? -1 : parent->m_own_idx;
        }

        if (created->m_parent == nullptr)
        {
            m_root_bones.push_back(created);
        }

        created->m_own_idx = (I32)m_bones.size();

        m_bones.push_back(created);

        if (parent != nullptr)
        {
            parent->Push(created);
        }

        return created;
    }

    Bone* Skeleton::CreateBone(const VQSTransform& to_bone, const VQSTransform& to_root, const std::string& name, I32 p_idx)
    {
        Bone* created       = new Bone();
        created->m_skeleton = this;
        created->m_to_bone  = to_bone;
        created->m_to_root  = to_root;
        created->m_name     = name;

        if (m_bones.empty())
        {
            created->m_parent     = nullptr;
            created->m_parent_idx = -1;
        }
        else
        {
            created->m_parent     = p_idx < 0 ? nullptr : m_bones[p_idx];
            created->m_parent_idx = p_idx;
        }
        created->m_own_idx = (I32)m_bones.size();

        if (created->m_parent == nullptr)
        {
            m_root_bones.push_back(created);
        }

        m_bones.push_back(created);

        if (created->m_parent != nullptr)
        {
            created->m_parent->Push(created);
        }

        return created;
    }

    Bone* Skeleton::CreateBone(const VQSTransform& to_bone, const std::string& name, Bone* parent)
    {
        return CreateBone(to_bone, to_bone.Inverse(), name, parent);
    }

    Bone* Skeleton::CreateBone(const VQSTransform& to_bone, const std::string& name, I32 p_idx)
    {
        return CreateBone(to_bone, to_bone.Inverse(), name, p_idx);
    }

    void Skeleton::SetUpSiblingRecursive(Bone* bone)
    {
        if (bone->m_children.empty())
            return;

        size_t size = bone->m_children.size();

        for (size_t i = 0; i < size; ++i)
        {
            SetUpSiblingRecursive(bone->m_children[i]);

            bone->m_children[i]->m_sibling.clear();
            bone->m_children[i]->m_sibling.reserve(size);
            bone->m_children[i]->m_sib_idx.clear();
            bone->m_children[i]->m_sib_idx.reserve(size);
            for (size_t j = 0; j < size; ++j)
            {
                if (i != j)
                {
                    bone->m_children[i]->m_sibling.push_back(bone->m_children[j]);
                    bone->m_children[i]->m_sib_idx.push_back(bone->m_children[j]->m_own_idx);
                }
            }
        }
    }

    void Skeleton::SetUpFinalBones()
    {
    }

    AnimationClip* Skeleton::CreateAnimationClip()
    {
        AnimationClip* created = new AnimationClip();
        created->skeleton      = this;
        created->bone_count    = m_bones.size();
        m_animation_clips.push_back(created);
        return created;
    }

    void Skeleton::DrawFinalVQS(PrimitiveRenderer* renderer) const
    {
        size_t size = m_bones.size();

        if (size != m_final_vqs.size())
        {
            return;
        }

       
        for (size_t i = 0; i < size; ++i)
        {
            Bone*  bone       = m_bones[i];
            size_t child_size = bone->m_children.size();
            for (size_t j = 0; j < child_size; ++j)
            {
                Vector3 parent_pos = m_final_vqs[bone->m_own_idx].position;
                Vector3 child_pos  = m_final_vqs[bone->m_children[j]->m_own_idx].position;
                renderer->DrawSegment(parent_pos, child_pos, m_color);
            }

           /* if (m_bones[i]->m_parent_idx >= 0)
            {
                renderer->DrawPrimitive(Sphere(m_final_vqs[i].position, m_final_vqs[i].rotation, 0.05f), eRenderingMode::Face, m_color);

               
            }*/
        }
    }
}
