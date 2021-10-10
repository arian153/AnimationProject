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
        CreateSample();
    }

    void Skeleton::Update(Real dt)
    {
        //update by keyframe
        if (m_clip_id != Core::I64_MAX)
        {
            auto& clip = m_animation_clips[m_clip_id];
            clip.Update(m_root_bone, dt);
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
    }

    void Skeleton::Draw(PrimitiveRenderer* renderer) const
    {
        if (m_root_bone != nullptr && m_b_draw)
        {
            DrawRecursive(renderer, m_root_bone);
        }
    }

    void Skeleton::CreateSample()
    {
        if (m_root_bone == nullptr)
        {
            m_root_bone = CreateBone(VQSTransform(Vector3(0, 2, 0), Quaternion(EulerAngle(Math::PI_DIV_4, Math::PI_DIV_4, Math::PI_DIV_6))), "root", -1);

            Bone* part_a = CreateBone(VQSTransform(Vector3(1, 0, 0), Quaternion(EulerAngle(0.0f, 0.0f, Math::PI_DIV_6))), "Part-A", m_root_bone);
            Bone* part_b = CreateBone(VQSTransform(Vector3(0, 1, 0), Quaternion()), "Part-B", m_root_bone);
            Bone* part_c = CreateBone(VQSTransform(Vector3(0, 0, 1), Quaternion()), "Part-C", m_root_bone);
            Bone* part_d = CreateBone(VQSTransform(Vector3(-1, 0, 0), Quaternion()), "Part-D", m_root_bone);
            Bone* part_e = CreateBone(VQSTransform(Vector3(0, -1, 0), Quaternion()), "Part-E", m_root_bone);
            Bone* part_f = CreateBone(VQSTransform(Vector3(0, 0, -1), Quaternion()), "Part-F", m_root_bone);

            Bone* part_a_a = CreateBone(VQSTransform(Vector3(0.5f, 0.5f, 0), Quaternion()), "Sub-A", part_a);
            Bone* part_a_b = CreateBone(VQSTransform(Vector3(0, 0.5f, 0.5f), Quaternion()), "Sub-B", part_a);
            Bone* part_a_c = CreateBone(VQSTransform(Vector3(0.5f, 0, 0.5f), Quaternion()), "Sub-C", part_a);
            Bone* part_a_d = CreateBone(VQSTransform(Vector3(-0.5f, -0.5f, 0), Quaternion()), "Sub-D", part_a);
            Bone* part_a_e = CreateBone(VQSTransform(Vector3(0, -0.5f, -0.5f), Quaternion()), "Sub-E", part_a);
            Bone* part_a_f = CreateBone(VQSTransform(Vector3(-0.5f, 0, -0.5f), Quaternion()), "Sub-F", part_a);

            Bone* part_b_a = CreateBone(VQSTransform(Vector3(0.5f, 0, 0), Quaternion()), "Sub-A", part_b);
            Bone* part_b_b = CreateBone(VQSTransform(Vector3(-0.5f, 0, 0), Quaternion()), "Sub-B", part_b);
            Bone* part_b_c = CreateBone(VQSTransform(Vector3(0, 0, 0.5f), Quaternion()), "Sub-C", part_b);
            Bone* part_b_d = CreateBone(VQSTransform(Vector3(0, 0, -0.5f), Quaternion()), "Sub-D", part_b);

            Bone* part_c_a = CreateBone(VQSTransform(Vector3(0.5f, 0, 0), Quaternion()), "Sub-A", part_c);
            Bone* part_c_b = CreateBone(VQSTransform(Vector3(-0.5f, 0, 0), Quaternion()), "Sub-B", part_c);
            Bone* part_c_c = CreateBone(VQSTransform(Vector3(0, 0, 0.5f), Quaternion()), "Sub-C", part_c);

            Bone* part_c_a1 = CreateBone(VQSTransform(Vector3(0, 0, 0.5f), Quaternion(EulerAngle(Math::PI_DIV_6))), "Sub-A", part_c_c);
            Bone* part_c_a2 = CreateBone(VQSTransform(Vector3(0, 0, 0.5f), Quaternion(EulerAngle(Math::PI_DIV_6))), "Sub-A", part_c_a1);
            Bone* part_c_a3 = CreateBone(VQSTransform(Vector3(0, 0, 0.5f), Quaternion(EulerAngle(Math::PI_DIV_6))), "Sub-A", part_c_a2);
            Bone* part_c_a4 = CreateBone(VQSTransform(Vector3(0, 0, 0.5f), Quaternion(EulerAngle(Math::PI_DIV_6))), "Sub-A", part_c_a3);
        }
    }

    void Skeleton::SetAniMeshResource(AniMeshResource* resource)
    {
        resource->CopyData(this);
    }

    void Skeleton::DrawRecursive(PrimitiveRenderer* renderer, Bone* bone) const
    {
        //only call root bone
        bone->Draw(renderer, m_color);
        Vector3 bone_pos = bone->m_offset.position;
        for (auto& child : bone->m_children)
        {
            Vector3 child_pos = Multiply(bone->m_offset, child->m_offset).position;
            renderer->DrawSegment(bone_pos, child_pos, m_color);
            DrawRecursive(renderer, child, bone->m_offset);
        }
    }

    void Skeleton::DrawRecursive(PrimitiveRenderer* renderer, Bone* bone, const VQSTransform& parent) const
    {
        bone->Draw(renderer, m_color, parent);
        VQSTransform offset   = Multiply(parent, bone->m_offset);
        Vector3      bone_pos = offset.position;

        for (auto& child : bone->m_children)
        {
            Vector3 child_pos = Multiply(offset, child->m_offset).position;
            renderer->DrawSegment(bone_pos, child_pos, m_color);
            DrawRecursive(renderer, child, offset);
        }
    }

    Bone* Skeleton::CreateBone(const VQSTransform& vqs, const std::string& name, Bone* parent)
    {
        Bone* created       = new Bone();
        created->m_skeleton = this;
        created->m_offset   = vqs;
        created->m_name     = name;

        created->m_parent     = parent;
        created->m_parent_idx = parent == nullptr ? -1 : parent->m_own_idx;
        created->m_own_idx    = (I64)m_bones.size();

        m_bones.push_back(created);

        if (parent != nullptr)
        {
            parent->Push(created);
        }

        return created;
    }

    Bone* Skeleton::CreateBone(const VQSTransform& vqs, const std::string& name, I64 p_idx)
    {
        Bone* created       = new Bone();
        created->m_skeleton = this;
        created->m_offset   = vqs;
        created->m_name     = name;

        created->m_parent     = p_idx < 0 ? nullptr : m_bones[p_idx];
        created->m_parent_idx = p_idx;
        created->m_own_idx    = (I64)m_bones.size();

        m_bones.push_back(created);

        if (created->m_parent != nullptr)
        {
            created->m_parent->Push(created);
        }

        return created;
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
}
