#include "TextSpriteFactory.hpp"
#include "../EngineComponent/TextSpriteComponent.hpp"

namespace CS460
{
    TextSpriteFactory::TextSpriteFactory()
    {
        type = typeid(TextSpriteComponent).name();
    }

    TextSpriteFactory::~TextSpriteFactory()
    {
    }

    Component* TextSpriteFactory::Create(Object* owner, Space* space)
    {
        auto create = new TextSpriteComponent(owner);
        create->m_type = type;
        create->SetSpace(space);
        return create;
    }

    Component* TextSpriteFactory::Clone(Component* origin, Object* dest, Space* space)
    {
        auto source = static_cast<TextSpriteComponent*>(origin);
        auto cloned = static_cast<TextSpriteComponent*>(this->Create(dest, space));
        cloned->Clone(source);
        return cloned;
    }
}
