#include "AniMeshComponent.hpp"

#include "../../../System/Animation/AnimationSubsystem.hpp"
#include "../../../System/Animation/Skeleton/Skeleton.hpp"
#include "../../../System/Graphics/Element/AniMesh.hpp"
#include "../../../System/Graphics/Element/Scene.hpp"
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
    }

    bool AniMeshComponent::Load(const Json::Value& data)
    {

        return true;
    }

    void AniMeshComponent::Save(Json::Value& data) const
    {
    }

    void AniMeshComponent::Edit(CommandRegistry* command_registry)
    {
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
