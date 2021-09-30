#include "AniMeshComponent.hpp"

namespace CS460
{
    AniMeshComponent::~AniMeshComponent()
    {
    }

    void AniMeshComponent::Initialize()
    {
    }

    void AniMeshComponent::Update(Real dt)
    {
    }

    void AniMeshComponent::Shutdown()
    {
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
    }

    void AniMeshComponent::Unsubscribe()
    {
    }

    AniMeshComponent::AniMeshComponent(Object* owner)
        : Component(owner)
    {
    }

    void AniMeshComponent::Clone(AniMeshComponent* origin)
    {
    }
}
