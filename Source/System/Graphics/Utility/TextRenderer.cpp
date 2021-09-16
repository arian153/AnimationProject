#include "TextRenderer.hpp"
#include "../Common/Buffer/TextFormatBufferCommon.hpp"
#include "../Common/Renderer/RendererCommon.hpp"
#include "MatrixManager.hpp"

namespace CS460
{
    TextRenderer::TextRenderer()
    {
    }

    TextRenderer::~TextRenderer()
    {
    }

    void TextRenderer::Initialize(RendererCommon* renderer, MatrixManager* matrix_manager)
    {
        m_renderer       = renderer;
        m_matrix_manager = matrix_manager;
        m_text_format    = new TextFormatBufferCommon();
        m_text_format->Initialize(renderer);
        m_text_format->BuildTextFormats(L"courier new", 36.0f);
        DebugTools::g_text_renderer = this;
    }

    void TextRenderer::Render()
    {
        if (!m_text_layouts.empty())
        {
            m_renderer->SetUpDWRenderTarget();
            for (auto& layout : m_text_layouts)
            {
                layout.Render();
            }
            ClearTextLayouts();
        }
    }

    void TextRenderer::Shutdown()
    {
        ClearTextLayouts();
        if (m_text_format != nullptr)
        {
            m_text_format->Shutdown();
            delete m_text_format;
            m_text_format = nullptr;
        }
    }

    void TextRenderer::ClearTextLayouts()
    {
        for (auto& layout : m_text_layouts)
        {
            layout.Shutdown();
        }
        m_text_layouts.clear();
    }

    void TextRenderer::Print(const Vector2& position, const Color& color, const std::wstring& text)
    {
        TextLayoutBufferCommon text_buffer;
        text_buffer.Initialize(m_renderer);
        text_buffer.SetTextFormat(m_text_format);
        text_buffer.SetPosition(position);
        text_buffer.SetColor(color);
        text_buffer.SetTextLayout(text, m_matrix_manager->GetScreenWidth(), m_matrix_manager->GetScreenHeight());
        m_text_layouts.push_back(text_buffer);
    }

    void TextRenderer::Output()
    {
    }
}
