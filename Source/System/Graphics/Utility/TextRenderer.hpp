#pragma once
#include "../../Math/Math.hpp"
#include "../Common/Buffer/TextLayoutBufferCommon.hpp"
#include <sstream>

namespace CS460
{
    class MatrixManager;
    class TextFormatBufferCommon;
    class RendererCommon;

    class TextRenderer
    {
    public:
        TextRenderer();
        ~TextRenderer();

        void Initialize(RendererCommon* renderer, MatrixManager* matrix_manager);
        void Render();
        void Shutdown();
        void ClearTextLayouts();

        void Print(const Vector2& position, const Color& color, const std::wstring& text);

        template <typename... Rest>
        void Output(const Vector2& position, const Color& color, const Rest&...rest)
        {
            std::wstringstream stream;
            Output(position, color, stream, rest...);
        }

        template <typename First, typename... Rest>
        void Output(const Vector2& position, const Color& color, std::wstringstream& stream, const First& first, const Rest&...rest)
        {
            stream << first;
            Output(position, color, stream, rest...);
        }

        template <typename T>
        void Output(const Vector2& position, const Color& color, std::wstringstream& stream, const T& t)
        {
            stream << t;
            auto text = stream.str();
            Print(position, color, text);
        }

        void Output();

    private:
        RendererCommon*         m_renderer       = nullptr;
        MatrixManager*          m_matrix_manager = nullptr;
        TextFormatBufferCommon* m_text_format    = nullptr;

        std::vector<TextLayoutBufferCommon> m_text_layouts;
    };

    namespace DebugTools
    {
        inline extern TextRenderer* g_text_renderer = nullptr;
    }

#define E5_DRAW_TEXT_OUTPUT(pos, color, ...) CS460::DebugTools::g_text_renderer->Output(pos, color, __VA_ARGS__);
}
