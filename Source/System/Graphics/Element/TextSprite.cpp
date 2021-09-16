// ReSharper disable CppExpressionWithoutSideEffects
#include "TextSprite.hpp"

#include "../../../Manager/Component/EngineComponent/TextSpriteComponent.hpp"
#include "../Common/Buffer/TextTextureBufferCommon.hpp"
#include "../../Math/Structure/Transform.hpp"

namespace CS460
{
    TextSprite::TextSprite()
    {
    }

    TextSprite::~TextSprite()
    {
    }

    void TextSprite::Initialize(RendererCommon* renderer)
    {
        if (m_texture == nullptr)
        {
            m_texture = new TextureCommon();
        }

        if (m_text == nullptr)
        {
            m_text = new TextTextureBufferCommon();
            m_text->Initialize(renderer);
            m_texture->Initialize(m_text);
        }
    }

    void TextSprite::Render() const
    {
        m_text->Render();
    }

    void TextSprite::Shutdown()
    {
        if (m_component != nullptr)
        {
            m_component->m_text_sprite = nullptr;
        }

        if (m_texture != nullptr)
        {
            delete m_texture;
            m_texture = nullptr;
        }

        if (m_text != nullptr)
        {
            m_text->Shutdown();
            delete m_text;
            m_text = nullptr;
        }
    }

    void TextSprite::SetText(const std::string& text) const
    {
        m_text->SetText(text);
        m_text->SetUpTextureSize();
        m_text->SetUpVertexBuffer();
        m_text->SetUpFontTexture();
        m_text->SetUpTextLayout();
        m_texture->Initialize(m_text);
    }

    void TextSprite::SetText(const std::wstring& text) const
    {
        m_text->SetText(text);
        m_text->SetUpTextureSize();
        m_text->SetUpVertexBuffer();
        m_text->SetUpFontTexture();
        m_text->SetUpTextLayout();
        m_texture->Initialize(m_text);
    }

    void TextSprite::SetFont(const std::wstring& font) const
    {
        m_text->SetFont(font);
        m_text->SetUpTextFormats();
        m_text->SetUpTextLayout();
        m_texture->Initialize(m_text);
    }

    void TextSprite::SetColor(const Color& color) const
    {
        m_text->SetColor(color);
    }

    void TextSprite::SetTransform(Transform* transform)
    {
        m_transform = transform;
    }

    Matrix44 TextSprite::GetModelMatrix() const
    {
        if (m_transform != nullptr)
        {
            return m_transform->LocalToWorldMatrix();
        }
        return Matrix44();
    }

    TextureCommon* TextSprite::GetTexture() const
    {
        return m_texture;
    }

    Color TextSprite::GetColor() const
    {
        return m_text->GetColor();
    }

    U32 TextSprite::GetIndexCount() const
    {
        return m_text->GetIndexCount();
    }
}
