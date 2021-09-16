#include "ParticleEmitterComponent.hpp"
#include "../../../System/Graphics/Element/ParticleEmitter.hpp"
#include "TransformComponent.hpp"
#include "../../Object/Object.hpp"
#include "../../Space/Space.hpp"
#include "../../../System/Graphics/Element/Scene.hpp"
#include "../../../System/Core/Utility/CoreUtility.hpp"
#include "../../Resource/ResourceType/JsonResource.hpp"
#include "../../../External/JSONCPP/json/json.h"
#include "../../Resource/ResourceManager.hpp"
#include "../../Resource/ResourceType/TextureResource.hpp"
#include "../../../System/GUI/Editor/Command/EditorCommand.hpp"
#include "../../../System/GUI/Editor/Command/CommandRegistry.hpp"

namespace CS460
{
    ParticleEmitterComponent::~ParticleEmitterComponent()
    {
    }

    void ParticleEmitterComponent::Initialize()
    {
        if (m_emitter == nullptr)
        {
            m_emitter = new ParticleEmitter();
            m_space->GetScene()->InitializeParticleEmitter(m_emitter);
            m_emitter->m_component = this;
            Subscribe();
        }
        if (m_transform == nullptr)
        {
            if (m_owner->HasComponent<TransformComponent>())
            {
                m_transform = m_owner->GetComponent<TransformComponent>()->GetTransform();
                m_emitter->SetTransform(m_transform);
            }
        }
    }

    void ParticleEmitterComponent::Update(Real dt)
    {
        E5_UNUSED_PARAM(dt);
    }

    void ParticleEmitterComponent::Shutdown()
    {
        Unsubscribe();
        if (m_emitter != nullptr)
        {
            m_emitter->Shutdown();
            delete m_emitter;
            m_emitter = nullptr;
        }
    }

    bool ParticleEmitterComponent::Load(const Json::Value& data)
    {
        if (JsonResource::HasMember(data, "Texture") && data["Texture"].isString())
        {
            std::string texture          = data["Texture"].asString();
            auto        resource         = m_space->GetResourceManager()->GetTextureResource(ToWString(texture));
            auto        default_resource = m_space->GetResourceManager()->GetTextureResource(L"DefaultTexture");
            m_emitter->SetTexture(resource->GetTexture(), default_resource->GetTexture());
        }
        if (JsonResource::HasMember(data, "Particle Amount") && data["Particle Amount"].isInt())
        {
            size_t amount = data["Particle Amount"].asLargestUInt();
            m_emitter->SetParticleAmount(amount);
        }
        if (JsonResource::HasMember(data, "Emission Amount") && data["Emission Amount"].isInt())
        {
            size_t amount = data["Emission Amount"].asLargestUInt();
            m_emitter->SetEmissionAmount(amount);
        }
        if (JsonResource::HasMember(data, "Emission Rate") && data["Emission Rate"].isDouble())
        {
            Real rate = data["Emission Rate"].asFloat();
            m_emitter->SetEmissionRate(rate);
        }
        if (JsonResource::HasMember(data, "Life Decay Rate") && data["Life Decay Rate"].isDouble())
        {
            Real rate = data["Life Decay Rate"].asFloat();
            m_emitter->SetLifeDecayRate(rate);
        }
        if (JsonResource::HasMember(data, "Scale Decay Rate") && data["Scale Decay Rate"].isDouble())
        {
            Real rate = data["Scale Decay Rate"].asFloat();
            m_emitter->SetScaleDecayRate(rate);
        }
        if (JsonResource::HasMember(data, "Base Position") && JsonResource::IsVector3(data["Base Position"]))
        {
            auto value = JsonResource::AsVector3(data["Base Position"]);
            m_emitter->SetBasePosition(value);
        }
        if (JsonResource::HasMember(data, "Base Velocity") && JsonResource::IsVector3(data["Base Velocity"]))
        {
            auto value = JsonResource::AsVector3(data["Base Velocity"]);
            m_emitter->SetBaseVelocity(value);
        }
        if (JsonResource::HasMember(data, "Base Color") && JsonResource::IsColor(data["Base Color"]))
        {
            auto value = JsonResource::AsColor(data["Base Color"]);
            m_emitter->SetBaseColor(value);
        }
        if (JsonResource::HasMember(data, "Base Life") && data["Base Life"].isDouble())
        {
            Real value = data["Base Life"].asFloat();
            m_emitter->SetBaseLife(value);
        }
        if (JsonResource::HasMember(data, "Base Scale") && data["Base Scale"].isDouble())
        {
            Real value = data["Base Scale"].asFloat();
            m_emitter->SetBaseScale(value);
        }
        if (JsonResource::HasMember(data, "Position Variance") && JsonResource::IsVector3(data["Position Variance"]))
        {
            auto variance = JsonResource::AsVector3(data["Position Variance"]);
            m_emitter->SetPositionVariance(variance);
        }
        if (JsonResource::HasMember(data, "Velocity Variance") && JsonResource::IsVector3(data["Velocity Variance"]))
        {
            auto variance = JsonResource::AsVector3(data["Velocity Variance"]);
            m_emitter->SetVelocityVariance(variance);
        }
        if (JsonResource::HasMember(data, "Color Variance") && JsonResource::IsColor(data["Color Variance"]))
        {
            auto variance = JsonResource::AsColor(data["Color Variance"]);
            m_emitter->SetColorVariance(variance);
        }
        if (JsonResource::HasMember(data, "Life Variance") && data["Life Variance"].isDouble())
        {
            Real variance = data["Life Variance"].asFloat();
            m_emitter->SetLifeVariance(variance);
        }
        if (JsonResource::HasMember(data, "Scale Variance") && data["Scale Variance"].isDouble())
        {
            Real variance = data["Scale Variance"].asFloat();
            m_emitter->SetScaleVariance(variance);
        }
        return true;
    }

    void ParticleEmitterComponent::Save(Json::Value& data) const
    {
    }

    void ParticleEmitterComponent::Edit(CommandRegistry* command_registry)
    {
        if (ImGui::CollapsingHeader(m_type.c_str(), &m_b_open))
        {
            ImGui::Separator();
            ImGui::Text("Texture");
            ImGui::Separator();
            ImGui::Text("Particle Amount");
            int particle_amount = (int)m_emitter->m_max_amount;
            int emission_amount = (int)m_emitter->m_emission_amount;
            ImGui::SliderInt("##ParticleEdit1", &particle_amount, emission_amount, 100000);
            if (ImGui::IsItemEdited())
            {
                m_edit_slider_particle_amount = particle_amount;
            }
            if (ImGui::IsItemDeactivatedAfterEdit())
            {
                command_registry->PushCommand(
                                              new EditFunction<
                                                  size_t,
                                                  ParticleEmitter,
                                                  &ParticleEmitter::SetParticleAmount>(
                                                                                       m_emitter,
                                                                                       m_emitter->m_max_amount,
                                                                                       m_edit_slider_particle_amount));
            }
            ImGui::InputInt("##ParticleEdit2", &particle_amount, 1, 100);
            particle_amount = Math::Clamp(particle_amount, emission_amount, 100000);
            if (ImGui::IsItemEdited())
            {
                command_registry->PushCommand(
                                              new EditFunction<
                                                  size_t,
                                                  ParticleEmitter,
                                                  &ParticleEmitter::SetParticleAmount>(m_emitter, m_emitter->m_max_amount, (size_t)particle_amount));
            }
            ImGui::Separator();
            ImGui::Text("Emission Amount");
            ImGui::SliderInt("##ParticleEdit3", &emission_amount, 0, 10000);
            if (ImGui::IsItemEdited())
            {
                m_edit_slider_emission_amount = emission_amount;
            }
            if (ImGui::IsItemDeactivatedAfterEdit())
            {
                command_registry->PushCommand(
                                              new EditFunction<
                                                  size_t,
                                                  ParticleEmitter,
                                                  &ParticleEmitter::SetEmissionAmount>(
                                                                                       m_emitter,
                                                                                       m_emitter->m_emission_amount,
                                                                                       m_edit_slider_emission_amount));
            }
            ImGui::InputInt("##ParticleEdit4", &emission_amount, 1, 100);
            emission_amount = Math::Clamp(emission_amount, 0, 10000);
            if (ImGui::IsItemEdited())
            {
                command_registry->PushCommand(
                                              new EditFunction<
                                                  size_t,
                                                  ParticleEmitter,
                                                  &ParticleEmitter::SetEmissionAmount>(m_emitter, m_emitter->m_emission_amount, (size_t)emission_amount));
            }
            ImGui::Separator();
            ImGui::Text("Emission Rate");
            Real emission_rate = m_emitter->m_emission_rate;
            ImGui::InputFloat("##ParticleEdit5", &emission_rate, 0.01f, 1.0f);
            emission_rate = Math::Max(emission_rate, 0.0f);
            if (ImGui::IsItemEdited())
            {
                command_registry->PushCommand(
                                              new EditFunction<
                                                  Real,
                                                  ParticleEmitter,
                                                  &ParticleEmitter::SetEmissionRate>(m_emitter, m_emitter->m_emission_rate, emission_rate));
            }
            ImGui::Separator();
            ImGui::Text("Life Decay Rate");
            Real life_decay_rate = m_emitter->m_life_decay_rate;
            ImGui::InputFloat("##ParticleEdit6", &life_decay_rate, 0.01f, 1.0f);
            life_decay_rate = Math::Max(life_decay_rate, 0.0f);
            if (ImGui::IsItemEdited())
            {
                command_registry->PushCommand(
                                              new EditFunction<
                                                  Real,
                                                  ParticleEmitter,
                                                  &ParticleEmitter::SetLifeDecayRate>(m_emitter, m_emitter->m_life_decay_rate, life_decay_rate));
            }
            ImGui::Separator();
            ImGui::Text("Scale Decay Rate");
            Real scale_decay_rate = m_emitter->m_scale_decay_rate;
            ImGui::InputFloat("##ParticleEdit7", &scale_decay_rate, 0.01f, 1.0f);
            if (ImGui::IsItemEdited())
            {
                command_registry->PushCommand(
                                              new EditFunction<
                                                  Real,
                                                  ParticleEmitter,
                                                  &ParticleEmitter::SetScaleDecayRate>(m_emitter, m_emitter->m_scale_decay_rate, scale_decay_rate));
            }
            ImGui::Separator();
            Particle base = m_emitter->m_base_particle;
            ImGui::Text("Base Position");
            float base_position[3] = {base.position.x, base.position.y, base.position.z};
            ImGui::InputFloat3("##ParticleEdit8", base_position, 3);
            if (ImGui::IsItemEdited())
            {
                command_registry->PushCommand(
                                              new EditFunction<
                                                  Vector3,
                                                  ParticleEmitter,
                                                  &ParticleEmitter::SetBasePosition>(m_emitter, base.position, Vector3(base_position)));
            }
            ImGui::Separator();
            ImGui::Text("Base Velocity");
            float base_velocity[3] = {base.velocity.x, base.velocity.y, base.velocity.z};
            ImGui::InputFloat3("##ParticleEdit9", base_velocity, 3);
            if (ImGui::IsItemEdited())
            {
                command_registry->PushCommand(
                                              new EditFunction<
                                                  Vector3,
                                                  ParticleEmitter,
                                                  &ParticleEmitter::SetBaseVelocity>(m_emitter, base.velocity, Vector3(base_velocity)));
            }
            ImGui::Separator();
            ImGui::Text("Base Color");
            float base_color[4] = {base.color.r, base.color.g, base.color.b, base.color.a};
            ImGui::ColorEdit4("##ParticleEdit10", base_color);
            if (ImGui::IsItemEdited())
            {
                m_edit_color_base = Color(base_color[0], base_color[1], base_color[2], base_color[3]);
            }
            if (ImGui::IsItemDeactivatedAfterEdit())
            {
                command_registry->PushCommand(
                                              new EditFunction<
                                                  Color,
                                                  ParticleEmitter,
                                                  &ParticleEmitter::SetBaseColor>(
                                                                                  m_emitter, base.color,
                                                                                  m_edit_color_base));
            }
            ImGui::Separator();
            ImGui::Text("Base Life");
            Real base_life = base.life;
            ImGui::InputFloat("##ParticleEdit11", &base_life, 0.01f, 1.0f);
            base_life = Math::Max(0.0f, base_life);
            if (ImGui::IsItemEdited())
            {
                command_registry->PushCommand(
                                              new EditFunction<
                                                  Real,
                                                  ParticleEmitter,
                                                  &ParticleEmitter::SetBaseLife>(m_emitter, base.life, base_life));
            }
            ImGui::Separator();
            ImGui::Text("Base Scale");
            Real base_scale = base.scale;
            ImGui::InputFloat("##ParticleEdit12", &base_scale, 0.01f, 1.0f);
            base_scale = Math::Max(0.0f, base_scale);
            if (ImGui::IsItemEdited())
            {
                command_registry->PushCommand(
                                              new EditFunction<
                                                  Real,
                                                  ParticleEmitter,
                                                  &ParticleEmitter::SetBaseScale>(m_emitter, base.scale, base_scale));
            }
            ImGui::Separator();
            ImGui::Text("Position Variance");
            float position_variance[3] = {
                m_emitter->m_position_variance.x, m_emitter->m_position_variance.y, m_emitter->m_position_variance.z
            };
            ImGui::InputFloat3("##ParticleEdit13", position_variance, 3);
            if (ImGui::IsItemEdited())
            {
                command_registry->PushCommand(
                                              new EditFunction<
                                                  Vector3,
                                                  ParticleEmitter,
                                                  &ParticleEmitter::SetPositionVariance>(m_emitter, m_emitter->m_position_variance, Vector3(position_variance)));
            }
            ImGui::Separator();
            ImGui::Text("Velocity Variance");
            float velocity_variance[3] = {
                m_emitter->m_velocity_variance.x, m_emitter->m_velocity_variance.y, m_emitter->m_velocity_variance.z
            };
            ImGui::InputFloat3("##ParticleEdit14", velocity_variance, 3);
            if (ImGui::IsItemEdited())
            {
                command_registry->PushCommand(
                                              new EditFunction<
                                                  Vector3,
                                                  ParticleEmitter,
                                                  &ParticleEmitter::SetVelocityVariance>(
                                                                                         m_emitter,
                                                                                         m_emitter->m_velocity_variance,
                                                                                         Vector3(velocity_variance)
                                                                                        )
                                             );
            }
            ImGui::Separator();
            ImGui::Text("Color Variance");
            float color_variance[4] = {
                m_emitter->m_color_variance.r, m_emitter->m_color_variance.g, m_emitter->m_color_variance.b, m_emitter->m_color_variance.a
            };
            ImGui::ColorEdit4("##ParticleEdit15", color_variance);
            if (ImGui::IsItemEdited())
            {
                m_edit_color_variance = Color(color_variance[0], color_variance[1], color_variance[2], color_variance[3]);
            }
            if (ImGui::IsItemDeactivatedAfterEdit())
            {
                command_registry->PushCommand(
                                              new EditFunction<
                                                  Color,
                                                  ParticleEmitter,
                                                  &ParticleEmitter::SetColorVariance>(
                                                                                      m_emitter,
                                                                                      m_emitter->m_color_variance,
                                                                                      m_edit_color_variance
                                                                                     )
                                             );
            }
            ImGui::Separator();
            ImGui::Text("Life Variance");
            Real life_variance = m_emitter->m_life_variance;
            ImGui::InputFloat("##ParticleEdit16", &life_variance, 0.01f, 1.0f);
            life_variance = Math::Max(0.0f, life_variance);
            if (ImGui::IsItemEdited())
            {
                command_registry->PushCommand(
                                              new EditFunction<
                                                  Real,
                                                  ParticleEmitter,
                                                  &ParticleEmitter::SetLifeVariance>(
                                                                                     m_emitter,
                                                                                     m_emitter->m_life_variance,
                                                                                     life_variance
                                                                                    )
                                             );
            }
            ImGui::Separator();
            ImGui::Text("Scale Variance");
            Real scale_variance = m_emitter->m_scale_variance;
            ImGui::InputFloat("##ParticleEdit17", &scale_variance, 0.01f, 1.0f);
            scale_variance = Math::Max(0.0f, scale_variance);
            if (ImGui::IsItemEdited())
            {
                command_registry->PushCommand(
                                              new EditFunction<
                                                  Real,
                                                  ParticleEmitter,
                                                  &ParticleEmitter::SetScaleVariance>(
                                                                                      m_emitter,
                                                                                      m_emitter->m_scale_variance,
                                                                                      scale_variance
                                                                                     )
                                             );
            }
            ImGui::Separator();
        }
    }

    void ParticleEmitterComponent::Subscribe()
    {
        if (m_space != nullptr && m_emitter != nullptr)
        {
            m_space->GetScene()->AddParticleEmitter(m_emitter);
        }
    }

    void ParticleEmitterComponent::Unsubscribe()
    {
        if (m_space != nullptr && m_emitter != nullptr)
        {
            m_space->GetScene()->RemoveParticleEmitter(m_emitter);
        }
    }

    ParticleEmitterComponent::ParticleEmitterComponent(Object* owner)
        : Component(owner)
    {
    }

    void ParticleEmitterComponent::Clone(ParticleEmitterComponent* origin)
    {
        if (origin != nullptr && origin != this)
        {
            //copy data
        }
    }
}
