#include "LogicComponent.hpp"

namespace CS460
{
    LogicComponent::~LogicComponent()
    {
    }

    LogicComponent::LogicComponent(Object* owner)
        : Component(owner)
    {
    }
}
