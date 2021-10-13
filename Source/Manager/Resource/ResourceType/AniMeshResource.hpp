#pragma once

#include "../Resource.hpp"
#include "../../../External/BinaryFileParser/BinParserModel.hpp"
#include "../ResourceLoader/FBXLoader.hpp"

namespace CS460
{
    class AniMesh;
    class Skeleton;

    enum class eAniMeshType
    {
        FBX,
        BIN
    };

    class AniMeshResource final : public Resource
    {
    public:
        explicit AniMeshResource(const std::wstring& path);
        ~AniMeshResource();

        void Initialize() override;
        void Shutdown() override;

        void CopyData(Skeleton* skeleton) const;
        void CopyData(AniMesh* ani_mesh);

    private:
        eAniMeshType     m_ani_mesh_type = eAniMeshType::FBX;
        FBXLoader        m_fbx_loader;
        BinParser::Model m_bin_model;
    };
}
