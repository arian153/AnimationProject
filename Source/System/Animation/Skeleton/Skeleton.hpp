#pragma once
#include <vector>

#include "../../Graphics/DataType/Color.hpp"
#include "../../Math/Utility/MathDef.hpp"
#include "../AnimationClip/AnimationClip.hpp"

namespace CS460
{
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

        void CreateSample();
        void SetAniMeshResource(AniMeshResource* resource);

    private:
        void  DrawRecursive(PrimitiveRenderer* renderer, Bone* bone) const;
        void  DrawRecursive(PrimitiveRenderer* renderer, Bone* bone, const VQSTransform& parent) const;
        Bone* CreateBone(const VQSTransform& to_bone, const VQSTransform& to_root, const std::string& name, Bone* parent);
        Bone* CreateBone(const VQSTransform& to_bone, const VQSTransform& to_root, const std::string& name, I32 p_idx);
        Bone* CreateBone(const VQSTransform& to_bone, const std::string& name, Bone* parent);
        Bone* CreateBone(const VQSTransform& to_bone, const std::string& name, I32 p_idx);

        void SetUpSiblingRecursive(Bone* bone);

        void AddAnimationClip(const AnimationClip& clip);

    private:
        friend class AniMeshComponent;
        friend class AniMeshResource;

    private:
        AniMeshComponent*          m_component = nullptr;
        std::vector<Bone*>         m_root_bones;
        std::vector<AnimationClip*> m_animation_clips;
        std::vector<Bone*>         m_bones;

        Color m_color;
        I32   m_clip_id = Core::I32_MAX;
        bool  m_b_draw  = true;
    };
}
