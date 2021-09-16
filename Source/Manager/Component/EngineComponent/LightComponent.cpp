#include "LightComponent.hpp"
#include "../../../System/Core/Utility/CoreUtility.hpp"
#include "../../Resource/ResourceType/JsonResource.hpp"
#include "../../../External/JSONCPP/json/json.h"
#include "../../Space/Space.hpp"
#include "../../../System/Graphics/Element/Scene.hpp"
#include "TransformComponent.hpp"
#include "../../../System/Graphics/Element/Light.hpp"
#include "../../Object/Object.hpp"

namespace CS460
{
    LightComponent::~LightComponent()
    {
    }

    void LightComponent::Initialize()
    {
        if (m_light == nullptr)
        {
            m_light = new Light();
            m_light->Initialize();
            m_light->m_component = this;
            Subscribe();
        }

        if (m_transform == nullptr)
        {
            if (m_owner->HasComponent<TransformComponent>())
            {
                m_transform = m_owner->GetComponent<TransformComponent>()->GetTransform();
            }
        }
    }

    void LightComponent::Update(Real dt)
    {
        E5_UNUSED_PARAM(dt);

        if (m_transform != nullptr)
        {
            m_light->light_data.position  = m_transform->position;
        }

        if (m_owner->HasComponent<MeshComponent>())
        {
            auto mesh = m_owner->GetComponent<MeshComponent>();
            mesh->SetDiffuseColor(m_light->light_data.diffuse_color);
        }
    }

    void LightComponent::Shutdown()
    {
        Unsubscribe();
        if (m_light != nullptr)
        {
            m_light->Shutdown();
            delete m_light;
            m_light = nullptr;
        }
    }

    void LightComponent::SetLightType(eLightType type)
    {
        m_light_type = type;
        m_light->SetType(type);
    }

    void LightComponent::SetLightDirection(const Vector3& dir) const
    {
        m_light->light_data.direction = dir;
    }

    Light* LightComponent::GetLight() const
    {
        return m_light;
    }

    bool LightComponent::Load(const Json::Value& data)
    {
        if (JsonResource::HasMember(data, "Type"))
        {
            if (data["Type"].isString())
            {
                std::string light_type = data["Type"].asString();

                if (light_type == "AmbientLight")
                {
                    SetLightType(eLightType::AmbientLight);
                }
                else if (light_type == "DirectionalLight")
                {
                    SetLightType(eLightType::DirectionalLight);
                }
                else if (light_type == "PointLight")
                {
                    SetLightType(eLightType::PointLight);
                }
                else if (light_type == "SpotLight")
                {
                    SetLightType(eLightType::SpotLight);
                }
                else if (light_type == "CapsuleLight")
                {
                    SetLightType(eLightType::CapsuleLight);
                }
            }

            if (data["Type"].isInt())
            {
                int light_type = data["Type"].asInt();
                light_type %= 5;

                SetLightType((eLightType)light_type);
            }
        }
        if (JsonResource::HasMember(data, "Ambient Color") && JsonResource::IsColor(data["Ambient Color"]))
        {
            m_light->light_data.ambient_color = JsonResource::AsColor(data["Ambient Color"]);
        }
        if (JsonResource::HasMember(data, "Ambient Range") && JsonResource::IsColor(data["Ambient Range"]))
        {
            m_light->light_data.ambient_range = JsonResource::AsColor(data["Ambient Range"]);
        }
        if (JsonResource::HasMember(data, "Diffuse Color") && JsonResource::IsColor(data["Diffuse Color"]))
        {
            m_light->light_data.diffuse_color = JsonResource::AsColor(data["Diffuse Color"]);
        }
        if (JsonResource::HasMember(data, "Specular Color") && JsonResource::IsColor(data["Specular Color"]))
        {
            m_light->light_data.specular_color = (JsonResource::AsColor(data["Specular Color"]));
        }
        if (JsonResource::HasMember(data, "Intensity") && data["Intensity"].isDouble())
        {
            m_light->light_data.intensity = (data["Intensity"].asFloat());
        }

        if (JsonResource::HasMember(data, "Direction") && JsonResource::IsVector3(data["Direction"]))
        {
            m_light->light_data.direction = (JsonResource::AsVector3(data["Direction"]));
        }
        if (JsonResource::HasMember(data, "Position") && JsonResource::IsVector3(data["Position"]))
        {
            m_light->light_data.position = (JsonResource::AsVector3(data["Position"]));
        }
        if (JsonResource::HasMember(data, "Attenuation") && JsonResource::IsVector3(data["Attenuation"]))
        {
            m_light->light_data.attenuation = (JsonResource::AsVector3(data["Attenuation"]));
        }
        if (JsonResource::HasMember(data, "Length") && data["Length"].isDouble())
        {
            m_light->light_data.length = (data["Length"].asFloat());
        }
        if (JsonResource::HasMember(data, "Range") && data["Range"].isDouble())
        {
            m_light->light_data.range = (data["Range"].asFloat());
        }
        if (JsonResource::HasMember(data, "Spot") && data["Spot"].isDouble())
        {
            m_light->light_data.spot = (data["Spot"].asFloat());
        }
        return true;
    }

    void LightComponent::Save(Json::Value& data) const
    {
    }

    void LightComponent::Edit(CommandRegistry* command_registry)
    {
        if (ImGui::CollapsingHeader(m_type.c_str(), &m_b_open))
        {
            ImGui::Text("Light Type");
            const char* light_type[] = {"Ambient Light", "Directional Light", "Point Light", "Spot Light", "Capsule Light"};

            if (ImGui::Combo("##Light Type", (int*)&m_light_type, light_type, 5))
            {
                m_light->SetType(m_light_type);
            }

            ImGui::Text("Ambient Color");
            ImGui::ColorEdit4("##Ambient Color", &m_light->light_data.ambient_color.r);

            if (m_light_type == eLightType::AmbientLight)
            {
                ImGui::Text("Ambient Range");
                ImGui::ColorEdit4("##Ambient Range", &m_light->light_data.ambient_range.r);
            }
            else
            {
                ImGui::Text("Diffuse Color");
                if (ImGui::ColorEdit4("##Diffuse Color", &m_light->light_data.diffuse_color.r))
                {
                }

                ImGui::Text("Specular Color");
                ImGui::ColorEdit4("##Specular Color", &m_light->light_data.specular_color.r);

                ImGui::Text("Intensity");
                ImGui::DragFloat("##Intensity", &m_light->light_data.intensity);

                if (m_light_type == eLightType::DirectionalLight)
                {
                    ImGui::Text("Direction");
                    ImGui::InputFloat3("##Direction", &m_light->light_data.direction.x);
                }
                else if (m_light_type == eLightType::PointLight)
                {
                    Vector3 pos = m_light->light_data.position;
                    ImGui::Text("Position");
                    ImGui::Text("[%.3f, %.3f, %.3f]", pos[0], pos[1], pos[2]);
                    ImGui::Text("Range");
                    ImGui::InputFloat("##Range", &m_light->light_data.range);
                    ImGui::Text("Attenuation");
                    ImGui::InputFloat3("##Attenuation", &m_light->light_data.attenuation.x);
                }
                else if (m_light_type == eLightType::SpotLight)
                {
                    Vector3 pos = m_light->light_data.position;
                    ImGui::Text("Position");
                    ImGui::Text("[%.3f, %.3f, %.3f]", pos[0], pos[1], pos[2]);

                    ImGui::Text("Direction");
                    ImGui::InputFloat3("##Direction", &m_light->light_data.direction.x);

                    ImGui::Text("Spot");
                    ImGui::InputFloat("##Spot", &m_light->light_data.spot);

                    ImGui::Text("Range");
                    ImGui::InputFloat("##Range", &m_light->light_data.range);

                    ImGui::Text("Attenuation");
                    ImGui::InputFloat3("##Attenuation", &m_light->light_data.attenuation.x);
                }
                else if (m_light_type == eLightType::CapsuleLight)
                {
                    Vector3 pos = m_light->light_data.position;
                    ImGui::Text("Position");
                    ImGui::Text("[%.3f, %.3f, %.3f]", pos[0], pos[1], pos[2]);

                    ImGui::Text("Direction");
                    ImGui::InputFloat3("##Direction", &m_light->light_data.direction.x);

                    ImGui::Text("Length");
                    ImGui::InputFloat("##Length", &m_light->light_data.length);

                    ImGui::Text("Range");
                    ImGui::InputFloat("##Range", &m_light->light_data.range);

                    ImGui::Text("Attenuation");
                    ImGui::InputFloat3("##Attenuation", &m_light->light_data.attenuation.x);
                }
            }
        }
    }

    void LightComponent::Subscribe()
    {
        if (m_space != nullptr && m_light != nullptr)
        {
            m_space->GetScene()->AddLight(m_light);
        }
    }

    void LightComponent::Unsubscribe()
    {
        if (m_space != nullptr && m_light != nullptr)
        {
            m_space->GetScene()->RemoveLight(m_light);
        }
    }

    LightComponent::LightComponent(Object* owner)
        : Component(owner)
    {
    }

    void LightComponent::Clone(LightComponent* origin)
    {
    }
}
