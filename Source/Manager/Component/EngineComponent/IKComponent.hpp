#pragma once

#include "..//Component.hpp"
#include "../../../System/Math/Utility/VectorDef.hpp"

namespace CS460
{
    class Manipulator;
    class Transform;

    class IKComponent final : public Component
    {
    public:
        ~IKComponent();
        IKComponent()                                  = delete;
        IKComponent(const IKComponent& rhs)            = delete;
        IKComponent& operator=(const IKComponent& rhs) = delete;

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
        friend class IKFactory;

    private:
        explicit IKComponent(Object* owner);
        void     Clone(IKComponent* origin);

    private:
        Transform*   m_transform   = nullptr;
        Manipulator* m_manipulator = nullptr;
    };
}
