#pragma once
#include "../Resource.hpp"

namespace CS460
{
    class TextResource final : public Resource
    {
    public:
        explicit TextResource(const std::wstring& path);
        ~TextResource();

        void Initialize() override;
        void Shutdown() override;
        bool LoadText();

        std::wstring GetText() const;

    private:
        std::wstring m_text = L"";
    };
}
