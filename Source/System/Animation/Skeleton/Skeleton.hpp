#pragma once
#include <vector>

#include "../../Graphics/DataType/Color.hpp"
#include "../../Math/Utility/MathDef.hpp"
#include "../AnimationClip/AnimationClip.hpp"

namespace CS460
{
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

    private:
        Bone* CreateBone(const VQSTransform& to_bone, const VQSTransform& to_root, const std::string& name, Bone* parent);
        Bone* CreateBone(const VQSTransform& to_bone, const VQSTransform& to_root, const std::string& name, I32 p_idx);
        Bone* CreateBone(const VQSTransform& to_bone, const std::string& name, Bone* parent);
        Bone* CreateBone(const VQSTransform& to_bone, const std::string& name, I32 p_idx);
        void  SetUpSiblingRecursive(Bone* bone);
        void SetUpBind();

        AnimationClip* CreateAnimationClip();

    private:
        friend class AniMeshComponent;
        friend class AniMeshResource;

    private:
        AniMeshComponent* m_component = nullptr;
        Transform*        m_transform = nullptr;

        I32   m_clip_id = 0;
        Color m_color;
        bool  m_b_draw  = true;
        bool  m_b_pause = false;

        int m_render_mode = 0;

        std::vector<Bone*>          m_root_bones;
        std::vector<Bone*>          m_bones;
        std::vector<AnimationClip*> m_animation_clips;
        std::vector<VQSTransform>   m_final_vqs;
        std::vector<VQSTransform>   m_to_roots;
        std::vector<std::string>    m_clip_names;

        std::vector<Matrix44>   m_final_mats;
        std::vector<Matrix44>   m_to_root_mats;
        std::vector<Matrix44>   m_bind_mats;
    };
}
