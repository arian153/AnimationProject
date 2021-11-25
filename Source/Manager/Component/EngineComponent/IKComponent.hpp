#pragma once

#include "..//Component.hpp"
#include "../../../System/Animation/InverseKinematics/Manipulator.hpp"
#include "../../../System/Math/Utility/VectorDef.hpp"

namespace CS460
{
    class KeyboardInput;
    class SpacePath;
    class AnimationSpace;
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

        void SetAnimationSpace(AnimationSpace* ani_space);
        void UpdatePath(const Vector3& target_point);

    protected:
        bool Load(const Json::Value& data) override;
        void Save(Json::Value& data) const override;
        void Edit(CommandRegistry* command_registry) override;
        void Subscribe() override;
        void Unsubscribe() override;

    private:
        friend class IKFactory;
        friend class AnimationSubsystem;

    private:
        explicit IKComponent(Object* owner);
        void     Clone(IKComponent* origin);

    private:
        Transform*      m_transform = nullptr;
        AnimationSpace* m_ani_space = nullptr;
        Manipulator     m_manipulator;
        int             m_path_id = -1;
        KeyboardInput*  m_input   = nullptr;
    };
}
