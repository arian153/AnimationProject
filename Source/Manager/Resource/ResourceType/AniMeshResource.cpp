#include "AniMeshResource.hpp"

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
        m_fbx_loader.LoadFbx(m_file_path_w);
        m_b_loaded = true;
    }

    void AniMeshResource::Shutdown()
    {
        m_fbx_loader.Shutdown();
        m_b_unloaded = true;
    }
}
