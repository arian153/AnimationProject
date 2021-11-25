#include "IKComponent.hpp"

#include "TransformComponent.hpp"
#include "../../../System/Animation/AnimationSubsystem.hpp"
#include "../../../System/Animation/Space/AnimationSpace.hpp"
#include "../../../System/Core/Input/InputCommon.hpp"
#include "../../../System/Core/Input/KeyboardInput.hpp"
#include "../../../System/Logic/LogicSubsystem.hpp"
#include "../../Object/Object.hpp"
#include "../../Space/Space.hpp"

namespace CS460
{
    IKComponent::~IKComponent()
    {
    }

    void IKComponent::Initialize()
    {
        Subscribe();
        if (m_transform == nullptr)
        {
            if (m_owner->HasComponent<TransformComponent>())
            {
                m_transform = m_owner->GetComponent<TransformComponent>()->GetTransform();
            }
        }

        m_input = m_space->GetLogicSubsystem()->GetInput()->GetKeyboardInput();

        m_manipulator.PushChild(Vector3(0, 0, 0));
        m_manipulator.PushChild(Vector3(2, 0, 0));
        m_manipulator.PushChild(Vector3(4, 0, 0));
        m_manipulator.PushChild(Vector3(6, 0, 0));
        m_manipulator.PushChild(Vector3(8, 0, 0));
        m_manipulator.PushChild(Vector3(10, 0, 0));
        m_manipulator.PushChild(Vector3(12, 0, 0));
        m_manipulator.SetUpData();
    }

    void IKComponent::Update(Real dt)
    {
        m_manipulator.InverseKinematics(dt);
    }

    void IKComponent::Shutdown()
    {
        m_manipulator.Shutdown();
        Unsubscribe();
    }

    void IKComponent::SetAnimationSpace(AnimationSpace* ani_space)
    {
        m_ani_space = ani_space;
        m_ani_space->SetIKCompo(this);
        m_path_id = ani_space->CreateNewPath("");
    }

    void IKComponent::UpdatePath(const Vector3& target_point)
    {
        Vector3 root_pos = m_manipulator.RootOrigin();
        root_pos.y       = target_point.y;
        SpacePath* path = nullptr;

        if (m_input->IsDown(eKeyCodeKeyboard::Shift_Left))
        {
            path = m_ani_space->GetPath(m_path_id);
            path->AddControlPoint(target_point);
        }
        else
        {
            m_path_id = m_ani_space->CreateNewPath("");
            path = m_ani_space->GetPath(m_path_id);
            path->AddControlPoint(root_pos);
            path->AddControlPoint(target_point);
        }
    }

    bool IKComponent::Load(const Json::Value& data)
    {
        return true;
    }

    void IKComponent::Save(Json::Value& data) const
    {
    }

    void IKComponent::Edit(CommandRegistry* command_registry)
    {
        if (ImGui::CollapsingHeader(m_type.c_str(), &m_b_open))
        {
        }
    }

    void IKComponent::Subscribe()
    {
        if (m_space != nullptr)
        {
            m_space->GetAnimationSubsystem()->AddIKComponent(this);
        }
    }

    void IKComponent::Unsubscribe()
    {
        if (m_space != nullptr)
        {
            m_space->GetAnimationSubsystem()->RemoveIKComponent(this);
        }
    }

    IKComponent::IKComponent(Object* owner)
        : Component(owner)
    {
    }

    void IKComponent::Clone(IKComponent* origin)
    {
        if (origin != nullptr && origin != this)
        {
            //copy data
        }
    }
}
