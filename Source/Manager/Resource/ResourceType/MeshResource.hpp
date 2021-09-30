#pragma once
#include "../Resource.hpp"
#include "../../../System/Graphics/DataType/MeshData.hpp"

namespace CS460
{
    enum class eMeshType : size_t
    {
        Invalid,
        CustomTXT,
        WaveFrontOBJ
    };

    enum class eOBJFaceType
    {
        Point,
        PointTexture,
        PointNormal,
        PointTextureNormal
    };

    class MeshResource final : public Resource
    {
    public:
        explicit MeshResource(const std::wstring& path);
        ~MeshResource();

        void Initialize() override;
        void Shutdown() override;

        void LoadWaveFrontOBJ(std::ifstream& file);
        void LoadGeneralOBJ(std::ifstream& file);
        void LoadOBJOnlyPos(std::ifstream& file);
        void LoadCustomTXT(std::ifstream& file);
        void CheckMeshType();

        MeshData* GetMeshData();

    private:
        eMeshType m_mesh_type = eMeshType::Invalid;
        MeshData  m_mesh_data;
    };
}
