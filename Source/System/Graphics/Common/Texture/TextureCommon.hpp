#pragma once
#include "../../GraphicsAPI.hpp"
#include IncludeTextureAPI
#include <string>

namespace CS460
{
    class RenderTextureBufferCommon;
    class TextTextureBufferCommon;
    class RendererCommon;

    class TextureCommon : public TextureAPI
    {
    public:
        TextureCommon();
        ~TextureCommon();

        bool Initialize(RendererCommon* renderer, const std::wstring& file_path, const std::wstring& file_type);
        bool Initialize(TextTextureBufferCommon* text_buffer);
        bool Initialize(RenderTextureBufferCommon* render_texture_buffer);
        bool InitializeDefault(RendererCommon* renderer);
        bool Shutdown();
        void Bind() const;

    private:
        friend class TextureArrayCommon;
        bool m_b_created = false;
    };
}
