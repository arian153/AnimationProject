#include "AniMeshComponent.hpp"

#include "../../../External/JSONCPP/json/json.h"
#include "../../../System/Animation/AnimationSubsystem.hpp"
#include "../../../System/Animation/Skeleton/Skeleton.hpp"
#include "../../../System/Core/Utility/CoreUtility.hpp"
#include "../../../System/Graphics/Element/AniMesh.hpp"
#include "../../../System/Graphics/Element/Scene.hpp"
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
        }

        if (m_skeleton != nullptr)
        {
            m_skeleton->Initialize();
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
        if (JsonResource::HasMember(data, "Mesh"))
        {
            auto mesh_data = data["Mesh"];

            if (mesh_data["Path"].isString())
            {
                m_mesh_resource_path = mesh_data["Path"].asString();
                m_mesh_resource      = m_space->GetResourceManager()->GetAniMeshResource(ToWString(m_mesh_resource_path));

                if (m_mesh_resource != nullptr)
                {
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
        if (ImGui::CollapsingHeader(m_type.c_str(), &m_b_open))
        {
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
