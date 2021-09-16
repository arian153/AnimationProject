#include "TextLayoutBufferDX11.hpp"
#include "../../Common/Buffer/TextLayoutBufferCommon.hpp"
#include "../../Common/Renderer/RendererCommon.hpp"
#include "../../Common/Buffer/TextFormatBufferCommon.hpp"
#include "../../DataType/Color.hpp"

namespace CS460
{
    TextLayoutBufferDX11::TextLayoutBufferDX11()
    {
    }

    TextLayoutBufferDX11::~TextLayoutBufferDX11()
    {
    }

    bool TextLayoutBufferDX11::SetTextLayout(const std::wstring& text, U32 width, U32 height)
    {
        HRESULT result = m_write_factory->CreateTextLayout(
                                                           text.c_str(),
                                                           (U32)text.length(),
                                                           m_text_format,
                                                           (Real)width,
                                                           (Real)height,
                                                           &m_text_layout
                                                          );
        if (FAILED(result))
        {
            return false;
        }
        return true;
    }

    void TextLayoutBufferDX11::SetTextFormat(TextFormatBufferCommon* text_format)
    {
        m_text_format = text_format->GetTextFormat();
    }

    TextLayoutBufferCommon::TextLayoutBufferCommon()
    {
    }

    TextLayoutBufferCommon::~TextLayoutBufferCommon()
    {
    }

    void TextLayoutBufferCommon::Initialize(RendererCommon* renderer)
    {
        m_write_factory      = renderer->GetDWWriteFactory();
        m_d2d_device_context = renderer->GetD2DDeviceContext();
    }

    void TextLayoutBufferCommon::Render() const
    {
        m_d2d_device_context->DrawTextLayout(D2D1::Point2F(m_position.x, m_position.y), m_text_layout, m_brush);
    }

    void TextLayoutBufferCommon::Shutdown()
    {
        if (m_brush != nullptr)
        {
            m_brush->Release();
            m_brush = nullptr;
        }
        if (m_text_layout != nullptr)
        {
            m_text_layout->Release();
            m_text_layout = nullptr;
        }
    }

    bool TextLayoutBufferCommon::SetPosition(const Vector2& position)
    {
        m_position = position;
        return true;
    }

    bool TextLayoutBufferCommon::SetColor(const Color& color)
    {
        m_d2d_device_context->CreateSolidColorBrush(D2D1::ColorF(color.r, color.g, color.b, color.a), &m_brush);
        return true;
    }
}
