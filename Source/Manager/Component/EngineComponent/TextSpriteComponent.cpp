#include "TextSpriteComponent.hpp"
#include "../../../System/Graphics/Element/TextSprite.hpp"
#include "TransformComponent.hpp"
#include "../../Object/Object.hpp"
#include "../../Space/Space.hpp"
#include "../../../System/Graphics/Element/Scene.hpp"
#include "../../../System/Core/Utility/CoreUtility.hpp"
#include "../../../External/JSONCPP/json/json.h"
#include "../../../System/Graphics/DataType/Color.hpp"
#include "../../Resource/ResourceType/JsonResource.hpp"
#include "../../Resource/ResourceManager.hpp"
#include "../../Resource/ResourceType/TextResource.hpp"

namespace CS460
{
    TextSpriteComponent::~TextSpriteComponent()
    {
    }

    void TextSpriteComponent::Initialize()
    {
        if (m_text_sprite == nullptr)
        {
            m_text_sprite = new TextSprite();
            m_space->GetScene()->InitializeTextSprite(m_text_sprite);
            m_text_sprite->m_component = this;
            Subscribe();
        }
        if (m_transform == nullptr)
        {
            if (m_owner->HasComponent<TransformComponent>())
            {
                m_transform = m_owner->GetComponent<TransformComponent>()->GetTransform();
                m_text_sprite->SetTransform(m_transform);
            }
        }
    }

    void TextSpriteComponent::Update(Real dt)
    {
        E5_UNUSED_PARAM(dt);
    }

    void TextSpriteComponent::Shutdown()
    {
        Unsubscribe();
        if (m_text_sprite != nullptr)
        {
            m_text_sprite->Shutdown();
            delete m_text_sprite;
            m_text_sprite = nullptr;
        }
    }

    bool TextSpriteComponent::Load(const Json::Value& data)
    {
        if (JsonResource::HasMember(data, "Color") && JsonResource::IsColor(data["Color"]))
        {
            m_text_sprite->SetColor(JsonResource::AsColor(data["Color"]));
        }
        if (JsonResource::HasMember(data, "Font") && data["Font"].isString())
        {
            m_text_sprite->SetFont(ToWString(data["Font"].asString()));
        }
        if (JsonResource::HasMember(data, "Text") && data["Text"].isString())
        {
            m_text_sprite->SetText(ToWString(data["Text"].asString()));
        }
        if (JsonResource::HasMember(data, "Text File") && data["Text File"].isString())
        {
            std::string text_file = data["Text File"].asString();
            auto        resource  = m_space->GetResourceManager()->GetTextResource(ToWString(text_file));
            m_text_sprite->SetText(resource->GetText());
        }
        return true;
    }

    void TextSpriteComponent::Save(Json::Value& data) const
    {
    }

    void TextSpriteComponent::Edit(CommandRegistry* command_registry)
    {
        ImGui::CollapsingHeader(m_type.c_str(), &m_b_open);
    }

    void TextSpriteComponent::Subscribe()
    {
        if (m_space != nullptr && m_text_sprite != nullptr)
        {
            m_space->GetScene()->AddTextSprite(m_text_sprite);
        }
    }

    void TextSpriteComponent::Unsubscribe()
    {
        if (m_space != nullptr && m_text_sprite != nullptr)
        {
            m_space->GetScene()->RemoveTextSprite(m_text_sprite);
        }
    }

    TextSpriteComponent::TextSpriteComponent(Object* owner)
        : Component(owner)
    {
    }

    void TextSpriteComponent::Clone(TextSpriteComponent* origin)
    {
    }
}
