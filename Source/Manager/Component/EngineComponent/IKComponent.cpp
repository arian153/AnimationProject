#include "IKComponent.hpp"

namespace CS460
{
    IKComponent::~IKComponent()
    {
    }

    void IKComponent::Initialize()
    {
    }

    void IKComponent::Update(Real dt)
    {
    }

    void IKComponent::Shutdown()
    {
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
    }

    void IKComponent::Subscribe()
    {
    }

    void IKComponent::Unsubscribe()
    {
    }

    IKComponent::IKComponent(Object* owner)
        : Component(owner)
    {
    }

    void IKComponent::Clone(IKComponent* origin)
    {
    }
}
