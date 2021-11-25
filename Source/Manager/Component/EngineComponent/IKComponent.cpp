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
        m_speed_control.SetT1T2(0.001f, 0.999f);
    }

    void IKComponent::Update(Real dt)
    {
        if (m_manipulator.IsReachable(m_target_pos))
        {
            m_manipulator.InverseKinematics(dt);
        }
        else
        {
            //calculate elapsed t for update.
            m_speed_control.elapsed_t += dt;
            Real normalize_t = m_speed_control.elapsed_t / m_path_duration;

            m_speed_control.elapsed_s = m_speed_control.DistanceTime(normalize_t);
            /* if (m_speed_control.elapsed_t >= m_path_duration)
             {
                 m_speed_control.elapsed_t = 0.0f;
                 m_speed_control.elapsed_s = 0.0f;
             }*/

            //Get position using space curve.
            Vector3 pos = m_ani_space->GetPathPoint(m_speed_control.elapsed_s);
            m_manipulator.SetRootPosition(pos);
        }
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
        SpacePath* path  = nullptr;
        m_target_pos     = target_point;
        m_target_pos.y   = 0.0f;

        if (m_input->IsDown(eKeyCodeKeyboard::Shift_Left))
        {
            path        = m_ani_space->GetPath(m_path_id);
            Real curr_s = m_speed_control.elapsed_s * path->MaxLength();
            path->AddControlPoint(target_point);

            Real new_length           = path->MaxLength();
            curr_s                    = Math::IsZero(new_length) ? 0.0f : curr_s / new_length;
            m_path_duration           = new_length * 0.08f;
            m_speed_control.elapsed_s = curr_s;
            m_speed_control.elapsed_t = m_path_duration * m_speed_control.InvDT(curr_s);
        }
        else
        {
            m_path_id = m_ani_space->CreateNewPath("");
            path      = m_ani_space->GetPath(m_path_id);
            path->AddControlPoint(root_pos);
            path->AddControlPoint(target_point);

            Real new_length = path->MaxLength();
            m_path_duration = new_length * 0.08f;

            m_speed_control.elapsed_t = 0.0f;
            m_speed_control.elapsed_s = 0.0f;
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
