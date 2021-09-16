#pragma once
#include "../../GraphicsAPI.hpp"
#include IncludeTextTextureBufferAPI

namespace CS460
{
    class RendererCommon;
    class Color;

    class TextTextureBufferCommon : public TextTextureBufferAPI
    {
    public:
        TextTextureBufferCommon();
        ~TextTextureBufferCommon();

        bool Initialize(RendererCommon* renderer);
        bool Update();
        void Render();
        void Shutdown();

        void SetText(const std::string& string) const;
        void SetText(const std::wstring& string) const;
        void SetFont(const std::wstring& font) const;
        void SetColor(Color color);

        U32   GetIndexCount() const;
        U32   GetVertexCount() const;
        Color GetColor() const;

    private:
    };
}
