#pragma once

#include "../Resource.hpp"
#include "../ResourceLoader/FBXLoader.hpp"

namespace CS460
{
    class AniMeshResource final : public Resource
    {
    public:
        explicit AniMeshResource(const std::wstring& path);
        ~AniMeshResource();

        void Initialize() override;
        void Shutdown() override;

    private:
        FBXLoader m_fbx_loader;
    };
}
