#pragma once

#include "..//Component.hpp"
#include "../../../System/Math/Utility/VectorDef.hpp"

namespace CS460
{
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
        friend class AniMesh;

    private:
        explicit AniMeshComponent(Object* owner);
        void     Clone(AniMeshComponent* origin);

    private:
        AniMesh*  m_ani_mesh = nullptr;
        Skeleton* m_skeleton = nullptr;
    };
}
