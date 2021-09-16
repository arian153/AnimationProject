#include "MouseInputWin32.hpp"
#include "../../Utility/CoreUtility.hpp"
#include "../../Input/MouseInput.hpp"
#include "../../../GUI/GUISystem.hpp"

namespace CS460
{
    MouseInputWin32::MouseInputWin32()
    {
    }

    MouseInputWin32::~MouseInputWin32()
    {
    }

    eKeyCodeMouse MouseInputWin32::TranslateMessage(WPARAM wparam)
    {
        switch (wparam)
        {
        case VK_LBUTTON:
            return eKeyCodeMouse::Left;
        case VK_RBUTTON:
            return eKeyCodeMouse::Right;
        case VK_MBUTTON:
            return eKeyCodeMouse::Middle;
        case VK_XBUTTON1:
            return eKeyCodeMouse::X1;
        case VK_XBUTTON2:
            return eKeyCodeMouse::X2;
        default:
            return eKeyCodeMouse::NONE;
        }
    }

    int MouseInputWin32::TranslateMessage(eKeyCodeMouse button_code)
    {
        switch (button_code)
        {
        case eKeyCodeMouse::NONE:
            return 0;
        case eKeyCodeMouse::Left:
            return VK_LBUTTON;
        case eKeyCodeMouse::Right:
            return VK_RBUTTON;
        case eKeyCodeMouse::Middle:
            return VK_MBUTTON;
        case eKeyCodeMouse::X1:
            return VK_XBUTTON1;
        case eKeyCodeMouse::X2:
            return VK_XBUTTON2;
        case eKeyCodeMouse::Max:
            return 0;
        default:
            return 0;
        }
    }

    MouseInput::MouseInput()
        : m_current_position(), m_previous_position(), m_button_state{}
    {
        Reset();
    }

    MouseInput::~MouseInput()
    {
    }

    void MouseInput::Reset()
    {
        for (size_t i = 0; i < MAXIMUM_BUTTON_COUNT; ++i)
        {
            m_button_state[i].b_down         = false;
            m_button_state[i].b_curr_pressed = false;
            m_button_state[i].b_prev_pressed = false;
        }
        m_current_wheel    = 0;
        m_previous_wheel   = 0;
        m_b_was_wheel      = false;
        m_b_was_mouse_move = false;
    }

    void MouseInput::ResetPressed()
    {
        for (size_t i = 0; i < MAXIMUM_BUTTON_COUNT; ++i)
        {
            m_button_state[i].b_curr_pressed = false;
        }
    }

    void MouseInput::ProcessMouseEvent(bool b_down, eKeyCodeMouse code, int x, int y)
    {
        m_button_state[static_cast<size_t>(code)].b_down = b_down;
        /*if (down == false)
        {
            MouseEvent ms_event(ButtonEventState::ButtonUp);
            ms_event.key = code;
            m_subject->Notify(ms_event, MouseEvent::StateToString(ButtonEventState::ButtonUp));
        }*/
        m_previous_position.x = m_current_position.x;
        m_previous_position.y = m_current_position.y;
        m_current_position.x  = x;
        m_current_position.y  = y;
        m_b_was_mouse_move    = true;
    }

    void MouseInput::ProcessMouseEvent(int x, int y)
    {
        m_previous_position.x = m_current_position.x;
        m_previous_position.y = m_current_position.y;
        m_current_position.x  = x;
        m_current_position.y  = y;
        m_b_was_mouse_move    = true;
    }

    void MouseInput::ProcessMouseWheel(short wheel)
    {
        m_previous_wheel = m_current_wheel;
        m_current_wheel += wheel;
        m_b_was_wheel = true;
    }

    void MouseInput::ProcessPressed()
    {
        for (int i = 0; i < MAXIMUM_BUTTON_COUNT; ++i)
        {
            if (m_button_state[i].b_down == true)
            {
                //MouseEvent down_event(ButtonEventState::ButtonDown);
                //down_event.key = static_cast<KeyID_Mouse>(i);
                //m_subject->Notify(down_event, MouseEvent::StateToString(ButtonEventState::ButtonDown));
                if (m_button_state[i].b_curr_pressed == false && m_button_state[i].b_prev_pressed == false)
                {
                    m_button_state[i].b_curr_pressed = true;
                    m_button_state[i].b_prev_pressed = false;
                    //MouseEvent press_event(ButtonEventState::ButtonPressed);
                    //press_event.key = static_cast<KeyID_Mouse>(i);
                    //m_subject->Notify(press_event, MouseEvent::StateToString(ButtonEventState::ButtonPressed));
                }
                else if (m_button_state[i].b_curr_pressed == true && m_button_state[i].b_prev_pressed == false)
                {
                    m_button_state[i].b_curr_pressed = false;
                    m_button_state[i].b_prev_pressed = true;
                }
            }
            else
            {
                m_button_state[i].b_curr_pressed = false;
                m_button_state[i].b_prev_pressed = false;
            }
        }
        if (m_b_was_wheel == true)
        {
            //use goto and skip below code.
            m_b_was_wheel = false;
            goto mouse_move;
        }
        if (m_b_was_wheel == false && m_current_wheel != m_previous_wheel)
        {
            m_previous_wheel = m_current_wheel;
        }
    mouse_move:
        if (m_b_was_mouse_move == true)
        {
            m_b_was_mouse_move = false;
            return;
        }
        if (m_b_was_mouse_move == false && (m_current_position.x != m_previous_position.x || m_current_position.y != m_previous_position.y))
        {
            m_previous_position.x = m_current_position.x;
            m_previous_position.y = m_current_position.y;
        }
    }

    bool MouseInput::IsDown(eKeyCodeMouse key_code) const
    {
        return GUISystem::IsFocusGUI() ? false : m_button_state[static_cast<size_t>(key_code)].b_down;
    }

    bool MouseInput::IsPressed(eKeyCodeMouse key_code) const
    {
        return GUISystem::IsFocusGUI() ? false : m_button_state[static_cast<size_t>(key_code)].b_curr_pressed;
    }

    bool MouseInput::IsAnyKeyDown() const
    {
        if (GUISystem::IsFocusGUI())
            return false;
        for (size_t i = 0; i < MAXIMUM_BUTTON_COUNT; ++i)
        {
            if (m_button_state[i].b_down == true)
                return true;
        }
        return false;
    }

    bool MouseInput::IsAnyKeyPressed() const
    {
        if (GUISystem::IsFocusGUI())
            return false;
        for (int i = 0; i < MAXIMUM_BUTTON_COUNT; ++i)
        {
            if (m_button_state[i].b_curr_pressed == true)
                return true;
        }
        return false;
    }

    size_t MouseInput::Repeated(eKeyCodeMouse key_code) const
    {
        E5_UNUSED_PARAM(key_code);
        return 0;
    }

    MousePosition MouseInput::CurrentPosition() const
    {
        return GUISystem::IsFocusGUI() ? MousePosition() : m_current_position;
    }

    MousePosition MouseInput::PreviousPosition() const
    {
        return GUISystem::IsFocusGUI() ? MousePosition() : m_previous_position;
    }

    int MouseInput::CurrentMouseWheel() const
    {
        return GUISystem::IsFocusGUI() ? 0 : static_cast<int>(m_current_wheel);
    }

    int MouseInput::PreviousMouseWheel() const
    {
        return GUISystem::IsFocusGUI() ? 0 : static_cast<int>(m_previous_wheel);
    }

    bool MouseInput::IsWheelRolling() const
    {
        return GUISystem::IsFocusGUI() ? false : m_current_wheel != m_previous_wheel;
    }

    Real MouseInput::MouseWheelRollingDirection() const
    {
        int wheel_value = m_current_wheel - m_previous_wheel;
        return GUISystem::IsFocusGUI() ? 0.0f : static_cast<Real>(wheel_value) / fabsf(static_cast<Real>(wheel_value));
    }
}
