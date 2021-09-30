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
    }

    void AniMeshResource::Shutdown()
    {
        m_fbx_loader.Shutdown();
    }
}
