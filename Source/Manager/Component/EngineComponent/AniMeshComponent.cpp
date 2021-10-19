#include "AniMeshComponent.hpp"

#include "TransformComponent.hpp"
#include "../../../External/JSONCPP/json/json.h"
#include "../../../System/Animation/AnimationSubsystem.hpp"
#include "../../../System/Animation/Skeleton/Skeleton.hpp"
#include "../../../System/Core/Utility/CoreUtility.hpp"
#include "../../../System/Graphics/Element/AniMesh.hpp"
#include "../../../System/Graphics/Element/Scene.hpp"
#include "../../Object/Object.hpp"
#include "../../Resource/ResourceManager.hpp"
#include "../../Resource/ResourceType/AniMeshResource.hpp"
#include "../../Resource/ResourceType/JsonResource.hpp"
#include "../../Space/Space.hpp"

namespace CS460
{
    AniMeshComponent::~AniMeshComponent()
    {
    }

    void AniMeshComponent::Initialize()
    {
        if (m_ani_mesh == nullptr)
        {
            m_ani_mesh              = new AniMesh();
            m_ani_mesh->m_component = this;
        }

        if (m_skeleton == nullptr)
        {
            m_skeleton              = new Skeleton();
            m_skeleton->m_component = this;
        }

        Subscribe();

        if (m_ani_mesh != nullptr)
        {
            m_ani_mesh->Initialize();
            m_ani_mesh->CreateBuffer();
        }

        if (m_skeleton != nullptr)
        {
            m_skeleton->Initialize();
        }

        if (m_transform == nullptr)
        {
            if (m_owner->HasComponent<TransformComponent>())
            {
                m_transform             = m_owner->GetComponent<TransformComponent>()->GetTransform();
                m_skeleton->m_transform = m_transform;
            }
        }
    }

    void AniMeshComponent::Update(Real dt)
    {
    }

    void AniMeshComponent::Shutdown()
    {
        Unsubscribe();
        if (m_ani_mesh != nullptr)
        {
            m_ani_mesh->Shutdown();
            delete m_ani_mesh;
            m_ani_mesh = nullptr;
        }

        if (m_skeleton != nullptr)
        {
            m_skeleton->Shutdown();
            delete m_skeleton;
            m_skeleton = nullptr;
        }
    }

    bool AniMeshComponent::Load(const Json::Value& data)
    {
        if (JsonResource::HasMember(data, "Shader Type"))
        {
            m_shader_type = data["Shader Type"].asString();
        }

        if (JsonResource::HasMember(data, "Mesh"))
        {
            auto mesh_data = data["Mesh"];

            if (mesh_data["Path"].isString())
            {
                m_mesh_resource_path = mesh_data["Path"].asString();
                m_mesh_resource      = m_space->GetResourceManager()->GetAniMeshResource(ToWString(m_mesh_resource_path));

                if (m_mesh_resource != nullptr)
                {
                    m_mesh_resource->CopyData(m_ani_mesh);
                    m_mesh_resource->CopyData(m_skeleton);
                }
            }
        }

        return true;
    }

    void AniMeshComponent::Save(Json::Value& data) const
    {
    }

    void AniMeshComponent::Edit(CommandRegistry* command_registry)
    {
        //GUI Editing.
        if (ImGui::CollapsingHeader(m_type.c_str(), &m_b_open))
        {
            ImGui::Text("Bone ");
            ImGui::SameLine();
            ImGui::Checkbox("##Draw Bone", &m_skeleton->m_b_draw);
            ImGui::Text("Bone Color");
            ImGui::ColorEdit4("##Bone Color", &m_skeleton->m_color.r);

            ImGui::Text("Animation : ");
            const char* pause_label = m_skeleton->m_b_pause ? "Play" : "Pause";
            ImGui::SameLine();
            if (ImGui::Button(pause_label))
            {
                m_skeleton->m_b_pause = !m_skeleton->m_b_pause;
            }

            int clip_count = (int)m_skeleton->m_clip_names.size();
            int clip_id    = m_skeleton->m_clip_id;
            if (ImGui::Combo("##Animation Clip", &m_skeleton->m_clip_id, VectorStringGetter, (void*)&m_skeleton->m_clip_names, clip_count))
            {
                clip_id = m_skeleton->m_clip_id;
            }

            ImGui::Text("Animation Speed ");
            ImGui::SameLine();
            if (ImGui::Button("Reset"))
            {
                m_skeleton->m_animation_clips[clip_id]->speed = 1.0f;
            }

            Real duration = m_skeleton->m_animation_clips[clip_id]->duration;
            ImGui::SliderFloat("##Animation Speed", &m_skeleton->m_animation_clips[clip_id]->speed, 0.1f * duration, 5.0f * duration);

            ImGui::Text("Track Status");
            if (ImGui::SliderFloat("##Elapsed time", &m_skeleton->m_animation_clips[clip_id]->elapsed_time, 0.0f, duration))
            {
                m_skeleton->m_animation_clips[clip_id]->UpdateTracks();
            }
        }
    }

    void AniMeshComponent::Subscribe()
    {
        if (m_space != nullptr)
        {
            if (m_ani_mesh != nullptr)
            {
                m_space->GetScene()->AddAniMesh(m_ani_mesh);
            }

            if (m_skeleton != nullptr)
            {
                m_space->GetAnimationSubsystem()->AddSkeleton(m_skeleton);
            }
        }
    }

    void AniMeshComponent::Unsubscribe()
    {
        if (m_space != nullptr)
        {
            if (m_ani_mesh != nullptr)
            {
                m_space->GetScene()->RemoveAniMesh(m_ani_mesh);
            }

            if (m_skeleton != nullptr)
            {
                m_space->GetAnimationSubsystem()->RemoveSkeleton(m_skeleton);
            }
        }
    }

    AniMeshComponent::AniMeshComponent(Object* owner)
        : Component(owner)
    {
    }

    void AniMeshComponent::Clone(AniMeshComponent* origin)
    {
    }
}
