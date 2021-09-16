#pragma once
#include "../../../System/Core/Core.hpp"
#include "../../../System/Math/Math.hpp"
#include "../../../Manager/Component/GameComponent/LogicComponent.hpp"
#include "../../../System/Math/Curve/Curve.hpp"

namespace Game
{
    class ControllerComponent final : public CS460::LogicComponent
    {
    public:
        ~ControllerComponent();
        ControllerComponent()                                          = delete;
        ControllerComponent(const ControllerComponent& rhs)            = delete;
        ControllerComponent& operator=(const ControllerComponent& rhs) = delete;

        void Initialize() override;
        void Update(CS460::Real dt) override;
        void Shutdown() override;
        void Render() override;

    protected:
        bool Load(const Json::Value& data) override;
        void Save(Json::Value& data) const override;
        void Edit(CS460::CommandRegistry* command_registry) override;
        void Subscribe() override;
        void Unsubscribe() override;

    private:
        friend class ControllerFactory;

    private:
        explicit ControllerComponent(CS460::Object* owner);
        void     Clone(ControllerComponent* origin);

    private:
        CS460::Real    m_theta  = -CS460::Math::HALF_PI;
        CS460::Real    m_phi    = CS460::Math::HALF_PI;
        CS460::Real    m_radius = 60.0f;
        CS460::Vector3 target_pos;
        CS460::Vector3 eye_pos;
    };
}
