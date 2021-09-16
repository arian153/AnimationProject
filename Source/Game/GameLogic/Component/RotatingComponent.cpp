#include "RotatingComponent.h"

#include "../../../External/JSONCPP/json/json.h"
#include "../../../Manager/Component/EngineComponent/LightComponent.hpp"
#include "../../../Manager/Object/Object.hpp"
#include "../../../Manager/Component/EngineComponent/TransformComponent.hpp"
#include "../../../Manager/Resource/ResourceType/JsonResource.hpp"
#include "../../../Manager/Space/Space.hpp"
#include "../../../System/Logic/LogicSubsystem.hpp"
#include "../../../System/Graphics/Element/Scene.hpp"

namespace Game
{
    using namespace CS460;

    RotatingComponent::~RotatingComponent()
    {
    }

    void RotatingComponent::Initialize()
    {
        if (m_space != nullptr)
        {
            m_space->GetLogicSubsystem()->InitializeLogic(this);
        }
        Subscribe();

        s_basis.CalculateBasisQuaternion(s_normal);
        s_count++;
        UpdateElapsedAngle();
    }

    void RotatingComponent::Update(CS460::Real dt)
    {
        if (m_owner->HasComponent<CS460::TransformComponent>() && s_b_pause == false)
        {
            m_elapsed_angle += s_ball_speed * dt;
            Vector3 position  = s_orbit_radius * (cosf(m_elapsed_angle) * s_basis.j + sinf(m_elapsed_angle) * s_basis.k);
            auto    transform = m_owner->GetComponent<CS460::TransformComponent>();
            transform->SetPosition(s_orbit_center + position);

            if (m_owner->HasComponent<CS460::LightComponent>())
            {
                auto light = m_owner->GetComponent<CS460::LightComponent>();
                light->SetLightDirection((-position).Unit());
            }
        }
    }

    void RotatingComponent::Shutdown()
    {
        s_count--;
        Unsubscribe();
        UpdateElapsedAngle();
    }

    void RotatingComponent::Render()
    {
    }

    bool RotatingComponent::Load(const Json::Value& data)
    {
        return true;
    }

    void RotatingComponent::Save(Json::Value& data) const
    {
    }

    void RotatingComponent::Edit(CS460::CommandRegistry* command_registry)
    {
        if (ImGui::CollapsingHeader(m_type.c_str(), &m_b_open))
        {
            std::string state = s_b_pause ? "Resume" : "Pause";

            ImGui::Checkbox(state.c_str(), &s_b_pause);
            ImGui::Text("Ball Speed");
            ImGui::SameLine();
            if (ImGui::Button("Zero##Ball Speed"))
            {
                s_ball_speed = 0.0f;
            }
            ImGui::SliderFloat("##Ball Speed", &s_ball_speed, -2.0f, 2.0f);
            ImGui::Text("Orbit Radius");
            ImGui::SliderFloat("##Orbit Radius", &s_orbit_radius, 0.0f, 20.0f);
            ImGui::Text("Orbit Center");
            ImGui::InputFloat3("##Orbit Center", &s_orbit_center.x);
            ImGui::Text("Orbit Normal");
            if (ImGui::InputFloat3("##Orbit Normal", &s_normal.x))
            {
                s_normal.SetNormalize();
                s_basis.CalculateBasisQuaternion(s_normal);
            }
        }
    }

    void RotatingComponent::Subscribe()
    {
        if (m_space != nullptr)
        {
            m_space->GetLogicSubsystem()->AddLogic(this);
        }
    }

    void RotatingComponent::Unsubscribe()
    {
        if (m_space != nullptr)
        {
            m_space->GetLogicSubsystem()->RemoveLogic(this);
        }
    }

    void RotatingComponent::UpdateElapsedAngle() const
    {
        std::vector<LogicComponent*> container;
        m_space->GetLogicSubsystem()->GetLogicComponents(m_type, container);
        s_count = (int)container.size();

        float angle = Math::TWO_PI / (float)s_count;

        for (int i = 0; i < s_count; ++i)
        {
            RotatingComponent* compo = ((RotatingComponent*)container[i]);
            compo->m_elapsed_angle   = (float)i * angle;
        }
    }

    RotatingComponent::RotatingComponent(CS460::Object* owner)
        : LogicComponent(owner)
    {
    }

    void RotatingComponent::Clone(RotatingComponent* origin)
    {
    }
}
