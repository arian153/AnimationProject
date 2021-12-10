#pragma once

#include "..//Component.hpp"

namespace CS460
{
    class SpringConstraint;
    class CameraComponent;
    class MouseInput;
    class KeyboardInput;
    class SpacePath;
    class AnimationSpace;
    class Manipulator;
    class Transform;
    class PrimitiveRenderer;

    class SpringConstraintsComponent final : public Component
    {
    public:
        ~SpringConstraintsComponent();
        SpringConstraintsComponent()                                                 = delete;
        SpringConstraintsComponent(const SpringConstraintsComponent& rhs)            = delete;
        SpringConstraintsComponent& operator=(const SpringConstraintsComponent& rhs) = delete;

        void Initialize() override;
        void Update(Real dt) override;
        void Shutdown() override;

    protected:
        bool Load(const Json::Value& data) override;
        void Save(Json::Value& data) const override;
        void Edit(CommandRegistry* command_registry) override;
        void Subscribe() override;
        void Unsubscribe() override;

        void SetUpConstraints() const;
    private:
        friend class SpringConstraintsFactory;
        friend class SpringConstraint;

    private:
        explicit SpringConstraintsComponent(Object* owner);
        void     Clone(SpringConstraintsComponent* origin);

    private:
        SpringConstraint* m_spring_constraint = nullptr;

        Object* m_target_a   = nullptr;
        Object* m_target_b   = nullptr;
        bool    m_has_anchor = false;

        size_t target_a_idx = 0;
        size_t target_b_idx = 7;
    };
}
