#pragma once

#include "../Resource.hpp"
#include "../../../External/BinaryFileParser/BinParserModel.hpp"
#include "../../../System/Graphics/Common/Vertex/SkinnedVertexCommon.hpp"
#include "../../../System/Math/Structure/VQSTransform.hpp"
//#include "../ResourceLoader/FBXLoader.hpp"

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

        VQSTransform ToVQS(const BinParser::Vqs& vqs) const;
        Vector2      ToVector2(const BinParser::Vector2& v2) const;
        Vector3      ToVector3(const BinParser::Vector3& v3) const;

    private:
        eAniMeshType m_ani_mesh_type = eAniMeshType::FBX;
        //FBXLoader        m_fbx_loader;
        BinParser::Model m_bin_model;

        std::vector<SkinnedVertexCommon> m_vertices;
    };
}
