#include "AniMeshResource.hpp"

#include "../../../System/Animation/Skeleton/Skeleton.hpp"
#include "../../../System/Core/Utility/CoreUtility.hpp"

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
        }

        m_b_loaded = true;
    }

    void AniMeshResource::Shutdown()
    {
        m_fbx_loader.Shutdown();
        m_b_unloaded = true;
    }

    void AniMeshResource::CopyData(Skeleton* skeleton)
    {
        for (auto& bone : m_fbx_loader.m_bones)
        {
            skeleton->CreateBone(bone->transform, ToString(bone->bone_name), bone->parent_index);
        }

        skeleton->m_animation_clips;
    }
}
