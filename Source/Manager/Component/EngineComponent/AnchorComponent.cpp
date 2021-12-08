#include "AnchorComponent.hpp"

namespace CS460
{
    AnchorComponent::~AnchorComponent()
    {
    }

    void AnchorComponent::Initialize()
    {
    }

    void AnchorComponent::Update(Real dt)
    {
    }

    void AnchorComponent::Shutdown()
    {
    }

    void AnchorComponent::Draw(PrimitiveRenderer* renderer)
    {
    }

    bool AnchorComponent::Load(const Json::Value& data)
    {
        return true;
    }

    void AnchorComponent::Save(Json::Value& data) const
    {
    }

    void AnchorComponent::Edit(CommandRegistry* command_registry)
    {
    }

    void AnchorComponent::Subscribe()
    {
    }

    void AnchorComponent::Unsubscribe()
    {
    }

    AnchorComponent::AnchorComponent(Object* owner)
        : Component(owner)
    {
    }

    void AnchorComponent::Clone(AnchorComponent* origin)
    {
    }
}
