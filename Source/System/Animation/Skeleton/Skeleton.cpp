#include "Skeleton.hpp"

#include "Bone.hpp"
#include "../../../Manager/Component/EngineComponent/AniMeshComponent.hpp"
#include "../../../Manager/Resource/ResourceType/AniMeshResource.hpp"
#include "../../Graphics/Utility/PrimitiveRenderer.hpp"
#include "../Space/AnimationSpace.hpp"

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
        m_skeleton_sphere.position.SetZero();
        m_skeleton_sphere.orientation.SetIdentity();
        m_skeleton_sphere.radius = 0.05f;
        m_speed_control.SetT1T2(0.001f, 0.999f);
    }

    void Skeleton::Update(Real dt)
    {
        //calculate elapsed t for update.
        m_speed_control.elapsed_t += dt;
        Real normalize_t = m_speed_control.elapsed_t / m_path_duration;
        //update by keyframe
        if (m_clip_id != -1 && !m_b_pause)
        {
            auto& clip  = m_animation_clips[m_clip_id];
            clip->speed = m_speed_control.VT(normalize_t);
            clip->Update(dt);
        }

        //get a elapsed s from distance-time function. it mapped [0:1]
        m_speed_control.elapsed_s = m_speed_control.DistanceTime(normalize_t);
        if (m_speed_control.elapsed_t >= m_path_duration)
        {
            m_speed_control.elapsed_t = 0.0f;
        }

        //check is there a path
        if (m_current_path > -1)
        {
            //Get position using space curve.
            Vector3 pos = m_ani_space->GetPathPoint(m_speed_control.elapsed_s);
            //Get rotation using orientation control
            m_orientation_control.SetPath(m_ani_space->GetCurrentPath());
            Quaternion rotation = m_orientation_control.GetOrientation(m_speed_control.elapsed_s);

            if (m_transform != nullptr)
            {
                //set transform of object
                m_transform->position    = pos;
                m_transform->orientation = rotation * m_default_orientation;
                //default orientation is a orientation from transform component.
                //some meshes are oriented different way, so default value from user.
            }
        }
    }

    void Skeleton::Shutdown()
    {
        //release data.
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

        for (auto& manipulator : m_manipulators)
        {
            manipulator.Shutdown();
        }
        m_manipulators.clear();
    }

    void Skeleton::Draw(PrimitiveRenderer* renderer) const
    {
        //Render skeleton structure
        if (!m_root_bones.empty() && m_b_draw)
        {
            size_t size = m_bones.size();

            if (size != m_final_vqs.size())
            {
                return;
            }

            Matrix44 world = Matrix44::Identity();
            if (m_transform != nullptr)
            {
                world = m_transform->LocalToWorldMatrix();
            }

            if (m_render_mode == 0)
            {
                for (size_t i = 0; i < size; ++i)
                {
                    Bone*   bone       = m_bones[i];
                    size_t  child_size = bone->m_children.size();
                    Vector3 parent_pos = world.TransformPoint(m_final_vqs[i].position);
                    renderer->DrawPrimitiveInstancing(m_skeleton_sphere, m_skeleton_sphere.orientation, parent_pos, eRenderingMode::Face, m_color);
                    for (size_t j = 0; j < child_size; ++j)
                    {
                        //has a child draw line segment.
                        Vector3 child_pos = world.TransformPoint(m_final_vqs[bone->m_children[j]->m_own_idx].position);
                        renderer->DrawSegment(parent_pos, child_pos, m_color);
                    }
                }
            }
            else if (m_render_mode == 1)
            {
                //render matrix version of animation bone
                for (size_t i = 0; i < size; ++i)
                {
                    Bone*   bone       = m_bones[i];
                    size_t  child_size = bone->m_children.size();
                    Vector3 parent_pos = world.TransformPoint(m_final_mats[i].GetPosition());
                    for (size_t j = 0; j < child_size; ++j)
                    {
                        //has a child draw line segment.
                        Vector3 child_pos = world.TransformPoint(m_final_mats[bone->m_children[j]->m_own_idx].GetPosition());
                        renderer->DrawSegment(parent_pos, child_pos, Color(1));
                    }
                }
            }
            else if (m_render_mode == 2)
            {
                //render matrix version of bind bone
                for (size_t i = 0; i < size; ++i)
                {
                    Bone*   bone       = m_bones[i];
                    size_t  child_size = bone->m_children.size();
                    Vector3 parent_pos = world.TransformPoint(m_bind_mats[i].GetPosition());
                    for (size_t j = 0; j < child_size; ++j)
                    {
                        //has a child draw line segment.
                        Vector3 child_pos = world.TransformPoint(m_bind_mats[bone->m_children[j]->m_own_idx].GetPosition());
                        renderer->DrawSegment(parent_pos, child_pos, Color(0, 0, 1));
                    }
                }
            }
        }
    }

    void Skeleton::UpdateKeyFrame(std::vector<KeyFrame>& key_frames)
    {
        //apply key frame transformation data to bone structure.
        I32 size = (I32)key_frames.size();
        if (size != m_bones.size())
        {
            return;
        }

        m_to_roots.resize(size);
        m_final_vqs.resize(size);

        m_final_mats.resize(size);
        m_to_root_mats.resize(size);
        //m_bind_mats;

        Matrix44 identity = Matrix44::Identity();
        for (I32 i = 0; i < size; ++i)
        {
            I32 parent_idx = m_bones[i]->m_parent_idx;
            Matrix44 parent_tf = parent_idx >= 0 ? m_to_root_mats[parent_idx] : identity;
            Matrix44 local_tf  = key_frames[i].to_parent.ToMatrix();
            m_to_root_mats[i]  = local_tf * parent_tf;
        }

        //for (uint boneIndex = 0; boneIndex < bones.size(); ++boneIndex)
        //{
        //    Bone& bone = bones[boneIndex];
        //    AnimTransform atx;
        //    //anim.CalculateTransform(time, boneIndex, atx, trackData[boneIndex]);
        //    Matrix4 parentTransform = bone.ParentBoneIndex != -1 ? buffer[bone.ParentBoneIndex] : identity;
        //    Matrix4 localTransform = atx.GetMatrix();
        //    Matrix4 modelTransform = localTransform * parentTransform;
        //    buffer[boneIndex] = modelTransform;
        //}

        for (I32 i = 0; i < size; ++i)
        {
            m_final_vqs[i] = Concatenate(m_bones[i]->m_to_root, m_to_roots[i]);
            m_final_vqs[i].rotation.SetNormalize();
            m_final_mats[i] = m_bind_mats[i] * m_to_root_mats[i];
        }

        for (I32 i = 0; i < size; ++i)
        {
            I32          parent_idx       = m_bones[i]->m_parent_idx;
            VQSTransform parent_transform = parent_idx > -1 ? m_final_vqs[parent_idx] : VQSTransform();
            VQSTransform local_transform  = Concatenate(key_frames[i].to_parent, m_bones[i]->m_to_root);
            m_final_vqs[i]                = Concatenate(local_transform, parent_transform);
        }
    }

    void Skeleton::AddPath(int idx_of_path)
    {
        //idx in animation space.
        auto found = std::find(m_path_ids.begin(), m_path_ids.end(), idx_of_path);
        if (found == m_path_ids.end())
        {
            m_path_ids.push_back(idx_of_path);
            m_current_path = idx_of_path;
            m_path_names.push_back(m_ani_space->GetPathName(idx_of_path));
        }
        else
        {
            m_current_path = idx_of_path;
        }
    }

    void Skeleton::SetSpace(AnimationSpace* space)
    {
        m_ani_space = space;
    }

    Real Skeleton::GetSParam() const
    {
        return m_speed_control.elapsed_s;
    }

    void Skeleton::SetSParam(Real s)
    {
        m_speed_control.elapsed_s = s;
        m_speed_control.elapsed_t = m_path_duration * m_speed_control.InvDT(s);
    }

    void Skeleton::ResetStatus()
    {
        m_speed_control.elapsed_s = 0.0f;
        m_speed_control.elapsed_t = 0.0f;

        m_transform->orientation = m_default_orientation;
    }

    void Skeleton::SetCOI(const Vector3& coi)
    {
        m_orientation_control.SetCOI(coi);
    }

    Bone* Skeleton::CreateBone(const VQSTransform& to_bone, const VQSTransform& to_root, const std::string& name, Bone* parent)
    {
        //create bone data
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
        //create bone data
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
        //Set up sibling relation ship
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

    void Skeleton::SetUpBind()
    {
        size_t size = m_bones.size();
        m_bind_mats.resize(size);
        for (size_t i = 0; i < size; ++i)
        {
            m_bind_mats[i] = m_bones[i]->m_to_root.ToMatrix();
        }
    }

    void Skeleton::SetUpEndEffectors()
    {
        m_end_effectors.clear();
        for (auto& bone : m_bones)
        {
            if (bone->IsEndEffector())
            {
                m_end_effectors.push_back(bone);
            }
        }
    }

    void Skeleton::SetUpManipulators()
    {
        size_t size = m_end_effectors.size();
        m_manipulators.clear();
        m_manipulators.resize(size);

        for (size_t i = 0; i < size; ++i)
        {
            m_manipulators[i].SetUpLinks(m_end_effectors[i]);
        }
    }

    AnimationClip* Skeleton::CreateAnimationClip()
    {
        //Create Animation clip data.
        AnimationClip* created = new AnimationClip();
        created->skeleton      = this;
        created->bone_count    = m_bones.size();
        m_animation_clips.push_back(created);
        return created;
    }
}
