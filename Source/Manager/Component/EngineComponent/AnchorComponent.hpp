#pragma once

#include "..//Component.hpp"

namespace CS460
{
    class KeyboardInput;
    class SpacePath;
    class AnimationSpace;
    class Manipulator;
    class Transform;
    class PrimitiveRenderer;

    class AnchorComponent final : public Component
    {
    public:
        ~AnchorComponent();
        AnchorComponent()                                      = delete;
        AnchorComponent(const AnchorComponent& rhs)            = delete;
        AnchorComponent& operator=(const AnchorComponent& rhs) = delete;

        void Initialize() override;
        void Update(Real dt) override;
        void Shutdown() override;
        void Draw(PrimitiveRenderer* renderer);

    protected:
        bool Load(const Json::Value& data) override;
        void Save(Json::Value& data) const override;
        void Edit(CommandRegistry* command_registry) override;
        void Subscribe() override;
        void Unsubscribe() override;

    private:
        friend class AnchorFactory;

    private:
        explicit AnchorComponent(Object* owner);
        void     Clone(AnchorComponent* origin);

    private:
        Transform*     m_transform = nullptr;
        KeyboardInput* m_input     = nullptr;
    };
}
