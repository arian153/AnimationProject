#pragma once

#include "..//Component.hpp"
#include "../../../System/Graphics/DataType/MaterialData.hpp"
#include "../../../System/Math/Utility/VectorDef.hpp"

namespace CS460
{
    class Transform;
    class AniMeshResource;

    struct SubMaterial
    {
        MaterialIdentifier material_info;
        MaterialColor      material_color;
    };

    class AniMeshComponent final : public Component
    {
    public:
        ~AniMeshComponent();
        AniMeshComponent()                                       = delete;
        AniMeshComponent(const AniMeshComponent& rhs)            = delete;
        AniMeshComponent& operator=(const AniMeshComponent& rhs) = delete;

        void Initialize() override;
        void Update(Real dt) override;
        void Shutdown() override;

    protected:
        bool Load(const Json::Value& data) override;
        void Save(Json::Value& data) const override;
        void Edit(CommandRegistry* command_registry) override;
        void Subscribe() override;
        void Unsubscribe() override;

    private:
        friend class AniMeshFactory;
        friend class Skeleton;
        friend class SkinnedMesh;

    private:
        explicit AniMeshComponent(Object* owner);
        void     Clone(AniMeshComponent* origin);

    private:
        SkinnedMesh* m_ani_mesh  = nullptr;
        Skeleton*    m_skeleton  = nullptr;
        Transform*   m_transform = nullptr;

        std::string              m_mesh_resource_path;
        AniMeshResource*         m_mesh_resource = nullptr;
        std::vector<SubMaterial> m_sub_materials;
    };
}
