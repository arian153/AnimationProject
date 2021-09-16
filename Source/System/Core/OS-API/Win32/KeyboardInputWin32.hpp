#pragma once
#include <string>
#include <windows.h>
#include "../../Input/KeyCode.hpp"

namespace CS460
{
    class KeyboardInputWin32
    {
    public:
        KeyboardInputWin32();
        ~KeyboardInputWin32();

    protected:
        void             ProcessString(WPARAM wparam) const;
        eKeyCodeKeyboard TranslateMessage(WPARAM wparam);
        int              TranslateMessage(eKeyCodeKeyboard key_code);
    private:
        friend class OSWin32;
    protected:
        std::wstring* m_string = nullptr;
    };
}
