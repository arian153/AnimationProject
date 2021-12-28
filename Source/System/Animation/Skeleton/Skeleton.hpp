#pragma once
#include <vector>

#include "OrientationController.hpp"
#include "SpeedController.hpp"
#include "../../Graphics/DataType/Color.hpp"
#include "../../Math/Primitive/ConvexHull3D/Sphere.hpp"
#include "../../Math/Utility/MathDef.hpp"
#include "../AnimationClip/AnimationClip.hpp"
#include "../InverseKinematics/Manipulator.hpp"

namespace CS460
{
    class AnimationSpace;
    class Transform;
    class VQSTransform;
    class AniMeshResource;
    class PrimitiveRenderer;
    class Bone;

    class Skeleton
    {
    public:
        Skeleton();
        ~Skeleton();

        void Initialize();
        void Update(Real dt);
        void Shutdown();
        void Draw(PrimitiveRenderer* renderer) const;
        void UpdateKeyFrame(std::vector<KeyFrame>& key_frames);
        void AddPath(int idx_of_path);
        void SetSpace(AnimationSpace* space);
        Real GetSParam() const;
        void SetSParam(Real s);
        void ResetStatus();
        void SetCOI(const Vector3& coi);

    private:
        Bone* CreateBone(const VQSTransform& to_bone, const VQSTransform& to_root, const std::string& name, Bone* parent);
        Bone* CreateBone(const VQSTransform& to_bone, const VQSTransform& to_root, const std::string& name, I32 p_idx);
        Bone* CreateBone(const VQSTransform& to_bone, const std::string& name, Bone* parent);
        Bone* CreateBone(const VQSTransform& to_bone, const std::string& name, I32 p_idx);
        void  SetUpSiblingRecursive(Bone* bone);
        void  SetUpBind();
        void  SetUpEndEffectors();
        void  SetUpManipulators();

        AnimationClip* CreateAnimationClip();

    private:
        friend class AniMeshComponent;
        friend class AniMeshResource;

    private:
        AniMeshComponent* m_component = nullptr;
        Transform*        m_transform = nullptr;
        AnimationSpace*   m_ani_space = nullptr;

        Quaternion m_default_orientation;

        Sphere m_skeleton_sphere;

        I32   m_clip_id = 0;
        Color m_color;
        bool  m_b_draw  = true;
        bool  m_b_pause = false;

        int  m_render_mode   = 0;
        int  m_current_path  = -1;
        Real m_path_duration = 1.0f;
        Real m_path_length   = 1.0f;
        Real m_speed_factor  = 1.0f;

        std::vector<Bone*>          m_root_bones;
        std::vector<Bone*>          m_bones;
        std::vector<Bone*>          m_end_effectors;
        std::vector<Manipulator>    m_manipulators;
        std::vector<AnimationClip*> m_animation_clips;
        std::vector<VQSTransform>   m_final_vqs;
        std::vector<VQSTransform>   m_to_root_vqs;
        std::vector<std::string>    m_clip_names;

        std::vector<Matrix44> m_final_mats;
        std::vector<Matrix44> m_to_root_mats;
        std::vector<Matrix44> m_bind_mats;

        std::vector<int>         m_path_ids;
        std::vector<std::string> m_path_names;
        SpeedController          m_speed_control;
        OrientationController    m_orientation_control;
    };
}
