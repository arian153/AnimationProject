#pragma once
#include <windows.h>
#include "../../Input/KeyCode.hpp"

namespace CS460
{
    class MouseInputWin32
    {
    public:
        MouseInputWin32();
        ~MouseInputWin32();

    protected:
        eKeyCodeMouse TranslateMessage(WPARAM wparam);
        int           TranslateMessage(eKeyCodeMouse button_code);
        
    private:
        friend class OSWin32;
    };
}
