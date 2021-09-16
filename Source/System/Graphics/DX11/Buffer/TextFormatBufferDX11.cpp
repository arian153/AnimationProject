#include "TextFormatBufferDX11.hpp"
#include "../../Common/Buffer/TextFormatBufferCommon.hpp"
#include "../../Common/Renderer/RendererCommon.hpp"

namespace CS460
{
    TextFormatBufferDX11::TextFormatBufferDX11()
    {
    }

    TextFormatBufferDX11::~TextFormatBufferDX11()
    {
    }

    bool TextFormatBufferDX11::BuildTextFormats(const std::wstring& font, Real font_size)
    {
        HRESULT result = m_write_factory->CreateTextFormat(
                                                           font.c_str(),
                                                           nullptr,
                                                           DWRITE_FONT_WEIGHT_NORMAL,
                                                           DWRITE_FONT_STYLE_NORMAL,
                                                           DWRITE_FONT_STRETCH_NORMAL,
                                                           font_size,
                                                           L"en-GB",
                                                           &m_text_format
                                                          );
        if (FAILED(result))
        {
            return false;
        }
        result = m_text_format->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
        if (FAILED(result))
        {
            return false;
        }
        result = m_text_format->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
        if (FAILED(result))
        {
            return false;
        }
        return true;
    }

    IDWriteTextFormat* TextFormatBufferDX11::GetTextFormat() const
    {
        return m_text_format;
    }

    TextFormatBufferCommon::TextFormatBufferCommon()
    {
    }

    TextFormatBufferCommon::~TextFormatBufferCommon()
    {
    }

    void TextFormatBufferCommon::Initialize(RendererCommon* renderer)
    {
        m_write_factory = renderer->GetDWWriteFactory();
    }

    void TextFormatBufferCommon::Shutdown()
    {
        if (m_text_format != nullptr)
        {
            m_text_format->Release();
            m_text_format = nullptr;
        }
    }
}
