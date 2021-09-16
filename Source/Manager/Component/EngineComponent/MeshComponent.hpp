#pragma once
#include "..//Component.hpp"
#include "../../../System/Graphics/DataType/MaterialData.hpp"

namespace CS460
{
    class MeshResource;
    class Mesh;
    class Quaternion;
    class Vector3;
    class Transform;

    class MeshComponent final : public Component
    {
    public:
        ~MeshComponent();
        MeshComponent()                                    = delete;
        MeshComponent(const MeshComponent& rhs)            = delete;
        MeshComponent& operator=(const MeshComponent& rhs) = delete;

        void Initialize() override;
        void Update(Real dt) override;
        void Shutdown() override;
        void SetDiffuseColor(const Color& color);

    protected:
        bool Load(const Json::Value& data) override;
        void Save(Json::Value& data) const override;
        void Edit(CommandRegistry* command_registry) override;
        void Subscribe() override;
        void Unsubscribe() override;
        void AddMesh();

    private:
        friend class MeshFactory;
        friend class Mesh;

    private:
        explicit MeshComponent(Object* owner);
        void     Clone(MeshComponent* origin);

    private:
        Mesh*         m_mesh           = nullptr;
        Transform*    m_transform      = nullptr;
        MeshResource* m_model_resource = nullptr;
        size_t        m_model_id       = 0;

        std::string        m_model_resource_path = "";
        std::string        m_shader_type         = "";
        MaterialIdentifier m_material_identifier;
        MaterialColor      m_material_color;
    };
}
