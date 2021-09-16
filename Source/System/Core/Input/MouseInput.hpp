#pragma once
#include "../OS-API/OSAPI.hpp"
#include IncludeMouseAPI
#include "KeyCode.hpp"
#include "../../Math/Utility/MathDef.hpp"

namespace CS460
{
    struct MousePosition
    {
        int x;
        int y;
    };

    class MouseInput : public MouseInputAPI
    {
    public:
        MouseInput();
        ~MouseInput();

        bool IsDown(eKeyCodeMouse key_code) const;
        bool IsPressed(eKeyCodeMouse key_code) const;
        bool IsAnyKeyDown() const;
        bool IsAnyKeyPressed() const;

        size_t Repeated(eKeyCodeMouse key_code) const;

        MousePosition CurrentPosition() const;
        MousePosition PreviousPosition() const;

        int  CurrentMouseWheel() const;
        int  PreviousMouseWheel() const;
        bool IsWheelRolling() const;
        Real MouseWheelRollingDirection() const;

    private:
        const static size_t MAXIMUM_BUTTON_COUNT = (size_t)eKeyCodeMouse::Max;

        void Reset();
        void ResetPressed();
        void ProcessMouseEvent(bool b_down, eKeyCodeMouse code, int x, int y);
        void ProcessMouseEvent(int x, int y);
        void ProcessMouseWheel(short wheel);
        void ProcessPressed();

    private:
        friend class InputCommon;
        friend class OSAPI;

    private:
        MousePosition m_current_position;
        MousePosition m_previous_position;
        ButtonState   m_button_state[ MAXIMUM_BUTTON_COUNT ];
        short         m_current_wheel    = 0;
        short         m_previous_wheel   = 0;
        bool          m_b_was_wheel      = false;
        bool          m_b_was_mouse_move = false;
    };
}
