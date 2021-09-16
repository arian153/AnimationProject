#pragma once
#include "../../../System/Core/Core.hpp"
#include "../../../System/Math/Math.hpp"
#include "../../../Manager/Component/GameComponent/LogicComponent.hpp"

namespace Game
{
    class RotatingComponent final : public CS460::LogicComponent
    {
    public:
        ~RotatingComponent();
        RotatingComponent()                                        = delete;
        RotatingComponent(const RotatingComponent& rhs)            = delete;
        RotatingComponent& operator=(const RotatingComponent& rhs) = delete;

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

        void UpdateElapsedAngle() const;

    private:
        friend class RotatingFactory;

    private:
        explicit RotatingComponent(CS460::Object* owner);
        void     Clone(RotatingComponent* origin);

    private:
        float m_elapsed_angle = 0.0f;

        inline static bool             s_b_pause = false;
        inline static CS460::Vector3 s_normal  = CS460::Vector3(0, 1, 0);
        inline static CS460::Basis   s_basis;

        inline static float            s_ball_speed   = 1.0f;
        inline static float            s_orbit_radius = 3.0f;
        inline static CS460::Vector3 s_orbit_center = CS460::Vector3();
        inline static int              s_count        = 0;
    };
}
