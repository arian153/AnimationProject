#include "AniMeshResource.hpp"

#include "../../../System/Animation/Skeleton/Skeleton.hpp"

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
        skeleton->m_bones;
        skeleton->m_animation_clips;
    }
}
