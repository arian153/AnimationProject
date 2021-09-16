#include "PointConstraintFactory.hpp"
#include "../EngineComponent/PointConstraintComponent.hpp"

namespace CS460
{
    PointConstraintFactory::PointConstraintFactory()
    {
        type = typeid(PointConstraintComponent).name();
    }

    PointConstraintFactory::~PointConstraintFactory()
    {
    }

    Component* PointConstraintFactory::Create(Object* owner, Space* space)
    {
        auto create = new PointConstraintComponent(owner);
        create->m_type = type;
        create->SetSpace(space);
        return create;
    }

    Component* PointConstraintFactory::Clone(Component* origin, Object* dest, Space* space)
    {
        auto source = static_cast<PointConstraintComponent*>(origin);
        auto cloned = static_cast<PointConstraintComponent*>(this->Create(dest, space));
        cloned->Clone(source);
        return cloned;
    }
}
