#pragma once

#include "../Resource.hpp"
#include "../ResourceLoader/FBXLoader.hpp"

namespace CS460
{
    class Skeleton;

    class AniMeshResource final : public Resource
    {
    public:
        explicit AniMeshResource(const std::wstring& path);
        ~AniMeshResource();

        void Initialize() override;
        void Shutdown() override;

        void CopyData(Skeleton* skeleton);

    private:
        FBXLoader m_fbx_loader;
    };
}
