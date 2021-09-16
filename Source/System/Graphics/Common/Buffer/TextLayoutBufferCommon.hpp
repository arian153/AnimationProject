#pragma once
#include "../../GraphicsAPI.hpp"
#include IncludeTextLayoutBufferAPI

namespace CS460
{
    class RendererCommon;
    class Color;
    class Vector2;

    class TextLayoutBufferCommon : public TextLayoutBufferAPI
    {
    public:
        TextLayoutBufferCommon();
        ~TextLayoutBufferCommon();

        void Initialize(RendererCommon* renderer);
        void Render() const;
        void Shutdown();

        bool SetPosition(const Vector2& position);
        bool SetColor(const Color& color);

    private:
    };
}
