#pragma once
#include <Windows.h>

namespace CS460
{
    enum class eWindowMode;
    class InputCommon;
    class KeyboardInput;
    class MouseInput;
    class Application;
    class OSCommon;

    class OSWin32
    {
    public:
        explicit OSWin32(Application* application, OSCommon* os_common);
        ~OSWin32();

        LRESULT ProcessMessage(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) const;
        void    AdjustAndCenterWindow(DWORD style, RECT& size, int& x_start, int& y_start) const;
        DWORD   GetWindowedStyle() const;

        HINSTANCE AppInstance() const;
        HWND      AppHWnd() const;

        void SetInput(InputCommon* input);
    protected:
        DWORD GetWindowModeStyle(eWindowMode window_mode) const;

    protected:
        friend class Application;

    protected:
        const DWORD FULLSCREEN_STYLE      = WS_POPUP | WS_VISIBLE;
        const DWORD WINDOWED_STYLE        = WS_POPUP | WS_CAPTION;
        const DWORD WINDOWED_STYLE_COMMON = WS_OVERLAPPEDWINDOW;
        const DWORD BORDERLESS_STYLE      = WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP;
    protected:
        //window data
        HINSTANCE m_h_instance;
        HWND      m_h_wnd = nullptr;
        DWORD     m_style = WINDOWED_STYLE_COMMON;

        //api data
        Application* m_application = nullptr;
        OSCommon*    m_os_common   = nullptr;

        MouseInput*    m_mouse_input    = nullptr;
        KeyboardInput* m_keyboard_input = nullptr;
    };
}
