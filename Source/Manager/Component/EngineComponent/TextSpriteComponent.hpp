#pragma once
#include "..//Component.hpp"

namespace CS460
{
    class RendererCommon;
    class Quaternion;
    class Vector3;
    class Transform;
    class TextSprite;

    class TextSpriteComponent final : public Component
    {
    public:
        ~TextSpriteComponent();
        TextSpriteComponent()                                          = delete;
        TextSpriteComponent(const TextSpriteComponent& rhs)            = delete;
        TextSpriteComponent& operator=(const TextSpriteComponent& rhs) = delete;

        void Initialize() override;
        void Update(Real dt) override;
        void Shutdown() override;

    protected:
        bool Load(const Json::Value& data) override;
        void Save(Json::Value& data) const override;
        void Edit(CommandRegistry* command_registry) override;
        void Subscribe() override;
        void Unsubscribe() override;

    private:
        friend class TextSpriteFactory;
        friend class TextSprite;

    private:
        explicit TextSpriteComponent(Object* owner);
        void     Clone(TextSpriteComponent* origin);

    private:
        TextSprite* m_text_sprite = nullptr;
        Transform*  m_transform   = nullptr;
    };
}
