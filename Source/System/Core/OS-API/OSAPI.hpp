#pragma once

#if defined(CS460_WIN32)
//classes
#define OSAPI OSWin32
#define KeyboardInputAPI KeyboardInputWin32
#define MouseInputAPI MouseInputWin32
#define GamePadInputAPI GamePadInputWin32
//includes
#define IncludeOSAPI "Win32/OSWin32.hpp"
#define IncludeKeyboardAPI "../OS-API/Win32/KeyboardInputWin32.hpp"
#define IncludeMouseAPI "../OS-API/Win32/MouseInputWin32.hpp"
#define IncludeGamePadAPI "../OS-API/Win32/GamePadInputWin32.hpp"

#endif
