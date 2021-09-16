#include "KeyboardInputWin32.hpp"
#include "../../Utility/CoreUtility.hpp"
#include "../../Input/KeyboardInput.hpp"
#include "../../../GUI/GUISystem.hpp"

namespace CS460
{
    KeyboardInputWin32::KeyboardInputWin32()
    {
    }

    KeyboardInputWin32::~KeyboardInputWin32()
    {
    }

    void KeyboardInputWin32::ProcessString(WPARAM wparam) const
    {
        if (m_string != nullptr)
        {
            m_string->push_back((wchar_t)wparam);
        }
    }

    eKeyCodeKeyboard KeyboardInputWin32::TranslateMessage(WPARAM wparam)
    {
        switch (wparam)
        {
        case VK_RETURN:
            return eKeyCodeKeyboard::Enter;
        case VK_ESCAPE:
            return eKeyCodeKeyboard::Escape;
        case VK_BACK:
            return eKeyCodeKeyboard::Backspace;
        case VK_TAB:
            return eKeyCodeKeyboard::Tab;
        case VK_SPACE:
            return eKeyCodeKeyboard::Space;
        case VK_OEM_COMMA:
            return eKeyCodeKeyboard::Comma;
        case VK_OEM_MINUS:
            return eKeyCodeKeyboard::Minus;
        case VK_OEM_PERIOD:
            return eKeyCodeKeyboard::Period;
        case 0x30:
            return eKeyCodeKeyboard::_0;
        case 0x31:
            return eKeyCodeKeyboard::_1;
        case 0x32:
            return eKeyCodeKeyboard::_2;
        case 0x33:
            return eKeyCodeKeyboard::_3;
        case 0x34:
            return eKeyCodeKeyboard::_4;
        case 0x35:
            return eKeyCodeKeyboard::_5;
        case 0x36:
            return eKeyCodeKeyboard::_6;
        case 0x37:
            return eKeyCodeKeyboard::_7;
        case 0x38:
            return eKeyCodeKeyboard::_8;
        case 0x39:
            return eKeyCodeKeyboard::_9;
        case VK_CAPITAL:
            return eKeyCodeKeyboard::CapsLock;
        case 0x41:
            return eKeyCodeKeyboard::A;
        case 0x42:
            return eKeyCodeKeyboard::B;
        case 0x43:
            return eKeyCodeKeyboard::C;
        case 0x44:
            return eKeyCodeKeyboard::D;
        case 0x45:
            return eKeyCodeKeyboard::E;
        case 0x46:
            return eKeyCodeKeyboard::F;
        case 0x47:
            return eKeyCodeKeyboard::G;
        case 0x48:
            return eKeyCodeKeyboard::H;
        case 0x49:
            return eKeyCodeKeyboard::I;
        case 0x4A:
            return eKeyCodeKeyboard::J;
        case 0x4B:
            return eKeyCodeKeyboard::K;
        case 0x4C:
            return eKeyCodeKeyboard::L;
        case 0x4D:
            return eKeyCodeKeyboard::M;
        case 0x4E:
            return eKeyCodeKeyboard::N;
        case 0x4F:
            return eKeyCodeKeyboard::O;
        case 0x50:
            return eKeyCodeKeyboard::P;
        case 0x51:
            return eKeyCodeKeyboard::Q;
        case 0x52:
            return eKeyCodeKeyboard::R;
        case 0x53:
            return eKeyCodeKeyboard::S;
        case 0x54:
            return eKeyCodeKeyboard::T;
        case 0x55:
            return eKeyCodeKeyboard::U;
        case 0x56:
            return eKeyCodeKeyboard::V;
        case 0x57:
            return eKeyCodeKeyboard::W;
        case 0x58:
            return eKeyCodeKeyboard::X;
        case 0x59:
            return eKeyCodeKeyboard::Y;
        case 0x5A:
            return eKeyCodeKeyboard::Z;
        case VK_F1:
            return eKeyCodeKeyboard::F1;
        case VK_F2:
            return eKeyCodeKeyboard::F2;
        case VK_F3:
            return eKeyCodeKeyboard::F3;
        case VK_F4:
            return eKeyCodeKeyboard::F4;
        case VK_F5:
            return eKeyCodeKeyboard::F5;
        case VK_F6:
            return eKeyCodeKeyboard::F6;
        case VK_F7:
            return eKeyCodeKeyboard::F7;
        case VK_F8:
            return eKeyCodeKeyboard::F8;
        case VK_F9:
            return eKeyCodeKeyboard::F9;
        case VK_F10:
            return eKeyCodeKeyboard::F10;
        case VK_F11:
            return eKeyCodeKeyboard::F11;
        case VK_F12:
            return eKeyCodeKeyboard::F12;
        case VK_SNAPSHOT:
            return eKeyCodeKeyboard::PrintScreen;
        case VK_SCROLL:
            return eKeyCodeKeyboard::ScrollLock;
        case VK_PAUSE:
            return eKeyCodeKeyboard::Pause;
        case VK_INSERT:
            return eKeyCodeKeyboard::Insert;
        case VK_HOME:
            return eKeyCodeKeyboard::Home;
        case VK_PRIOR:
            return eKeyCodeKeyboard::PageUp;
        case VK_DELETE:
            return eKeyCodeKeyboard::Delete;
        case VK_END:
            return eKeyCodeKeyboard::End;
        case VK_NEXT:
            return eKeyCodeKeyboard::PageDown;
        case VK_RIGHT:
            return eKeyCodeKeyboard::Arrow_Right;
        case VK_LEFT:
            return eKeyCodeKeyboard::Arrow_Left;
        case VK_DOWN:
            return eKeyCodeKeyboard::Arrow_Down;
        case VK_UP:
            return eKeyCodeKeyboard::Arrow_Up;
        case VK_APPS:
            return eKeyCodeKeyboard::Application;
        case VK_LMENU:
            return eKeyCodeKeyboard::Alt_Left;
        case VK_RMENU:
            return eKeyCodeKeyboard::Alt_Right;
        case VK_SELECT:
            return eKeyCodeKeyboard::Select;
        case VK_LCONTROL:
            return eKeyCodeKeyboard::Control_Left;
        case VK_LSHIFT:
            return eKeyCodeKeyboard::Shift_Left;
        case VK_RCONTROL:
            return eKeyCodeKeyboard::Control_Right;
        case VK_RSHIFT:
            return eKeyCodeKeyboard::Shift_Right;
        default:
            return eKeyCodeKeyboard::NONE;
        }
    }

    int KeyboardInputWin32::TranslateMessage(eKeyCodeKeyboard key_code)
    {
        switch (key_code)
        {
        case eKeyCodeKeyboard::NONE:
            return 0;
        case eKeyCodeKeyboard::_0:
            return 0x30;
        case eKeyCodeKeyboard::_1:
            return 0x31;
        case eKeyCodeKeyboard::_2:
            return 0x32;
        case eKeyCodeKeyboard::_3:
            return 0x33;
        case eKeyCodeKeyboard::_4:
            return 0x34;
        case eKeyCodeKeyboard::_5:
            return 0x35;
        case eKeyCodeKeyboard::_6:
            return 0x36;
        case eKeyCodeKeyboard::_7:
            return 0x37;
        case eKeyCodeKeyboard::_8:
            return 0x38;
        case eKeyCodeKeyboard::_9:
            return 0x39;
        case eKeyCodeKeyboard::A:
            return 0x41;
        case eKeyCodeKeyboard::B:
            return 0x42;
        case eKeyCodeKeyboard::C:
            return 0x43;
        case eKeyCodeKeyboard::D:
            return 0x44;
        case eKeyCodeKeyboard::E:
            return 0x45;
        case eKeyCodeKeyboard::F:
            return 0x46;
        case eKeyCodeKeyboard::G:
            return 0x47;
        case eKeyCodeKeyboard::H:
            return 0x48;
        case eKeyCodeKeyboard::I:
            return 0x49;
        case eKeyCodeKeyboard::J:
            return 0x4A;
        case eKeyCodeKeyboard::K:
            return 0x4B;
        case eKeyCodeKeyboard::L:
            return 0x4C;
        case eKeyCodeKeyboard::M:
            return 0x4D;
        case eKeyCodeKeyboard::N:
            return 0x4E;
        case eKeyCodeKeyboard::O:
            return 0x4F;
        case eKeyCodeKeyboard::P:
            return 0x50;
        case eKeyCodeKeyboard::Q:
            return 0x51;
        case eKeyCodeKeyboard::R:
            return 0x52;
        case eKeyCodeKeyboard::S:
            return 0x53;
        case eKeyCodeKeyboard::T:
            return 0x54;
        case eKeyCodeKeyboard::U:
            return 0x55;
        case eKeyCodeKeyboard::V:
            return 0x56;
        case eKeyCodeKeyboard::W:
            return 0x57;
        case eKeyCodeKeyboard::X:
            return 0x58;
        case eKeyCodeKeyboard::Y:
            return 0x59;
        case eKeyCodeKeyboard::Z:
            return 0x5A;
        case eKeyCodeKeyboard::F1:
            return VK_F1;
        case eKeyCodeKeyboard::F2:
            return VK_F2;
        case eKeyCodeKeyboard::F3:
            return VK_F3;
        case eKeyCodeKeyboard::F4:
            return VK_F4;
        case eKeyCodeKeyboard::F5:
            return VK_F5;
        case eKeyCodeKeyboard::F6:
            return VK_F6;
        case eKeyCodeKeyboard::F7:
            return VK_F7;
        case eKeyCodeKeyboard::F8:
            return VK_F8;
        case eKeyCodeKeyboard::F9:
            return VK_F9;
        case eKeyCodeKeyboard::F10:
            return VK_F10;
        case eKeyCodeKeyboard::F11:
            return VK_F11;
        case eKeyCodeKeyboard::F12:
            return VK_F12;
        case eKeyCodeKeyboard::Arrow_Up:
            return VK_UP;
        case eKeyCodeKeyboard::Arrow_Down:
            return VK_DOWN;
        case eKeyCodeKeyboard::Arrow_Right:
            return VK_RIGHT;
        case eKeyCodeKeyboard::Arrow_Left:
            return VK_LEFT;
        case eKeyCodeKeyboard::Enter:
            return VK_RETURN;
        case eKeyCodeKeyboard::Escape:
            return VK_ESCAPE;
        case eKeyCodeKeyboard::Backspace:
            return VK_BACK;
        case eKeyCodeKeyboard::Tab:
            return VK_TAB;
        case eKeyCodeKeyboard::Space:
            return VK_SPACE;
        case eKeyCodeKeyboard::Control_Left:
            return VK_LCONTROL;
        case eKeyCodeKeyboard::Control_Right:
            return VK_RCONTROL;
        case eKeyCodeKeyboard::Shift_Left:
            return VK_LSHIFT;
        case eKeyCodeKeyboard::Shift_Right:
            return VK_RSHIFT;
        case eKeyCodeKeyboard::Alt_Left:
            return VK_LMENU;
        case eKeyCodeKeyboard::Alt_Right:
            return VK_RMENU;
        case eKeyCodeKeyboard::Quote:
            return 0;
        case eKeyCodeKeyboard::Comma:
            return VK_OEM_COMMA;
        case eKeyCodeKeyboard::Minus:
            return VK_OEM_MINUS;
        case eKeyCodeKeyboard::Period:
            return VK_OEM_PERIOD;
        case eKeyCodeKeyboard::Slash:
            return 0;
        case eKeyCodeKeyboard::Semicolon:
            return 0;
        case eKeyCodeKeyboard::Equal:
            return 0;
        case eKeyCodeKeyboard::Bracket_Left:
            return 0;
        case eKeyCodeKeyboard::Bracket_Right:
            return 0;
        case eKeyCodeKeyboard::Backslash:
            return 0;
        case eKeyCodeKeyboard::PrintScreen:
            return VK_SNAPSHOT;
        case eKeyCodeKeyboard::ScrollLock:
            return VK_SCROLL;
        case eKeyCodeKeyboard::Pause:
            return VK_PAUSE;
        case eKeyCodeKeyboard::CapsLock:
            return VK_CAPITAL;
        case eKeyCodeKeyboard::Insert:
            return VK_INSERT;
        case eKeyCodeKeyboard::Home:
            return VK_HOME;
        case eKeyCodeKeyboard::PageUp:
            return VK_PRIOR;
        case eKeyCodeKeyboard::Delete:
            return VK_DELETE;
        case eKeyCodeKeyboard::End:
            return VK_END;
        case eKeyCodeKeyboard::PageDown:
            return VK_NEXT;
        case eKeyCodeKeyboard::Application:
            return VK_APPS;
        case eKeyCodeKeyboard::Power:
            return 0;
        case eKeyCodeKeyboard::Select:
            return VK_SELECT;
        case eKeyCodeKeyboard::MAX:
            return 0;
        default:
            return 0;
        }
    }

    KeyboardInput::KeyboardInput()
    {
        Reset();
    }

    KeyboardInput::~KeyboardInput()
    {
    }

    void KeyboardInput::Reset()
    {
        for (size_t i = 0; i < MAXIMUM_KEY_COUNT; ++i)
        {
            m_keyboard[i].b_down         = false;
            m_keyboard[i].b_curr_pressed = false;
            m_keyboard[i].b_prev_pressed = false;
        }
    }

    void KeyboardInput::ResetPressed()
    {
        for (size_t i = 0; i < MAXIMUM_KEY_COUNT; ++i)
        {
            m_keyboard[i].b_curr_pressed = false;
        }
    }

    void KeyboardInput::ProcessPressed()
    {
        for (int i = 0; i < MAXIMUM_KEY_COUNT; ++i)
        {
            if (GetAsyncKeyState(TranslateMessage(static_cast<eKeyCodeKeyboard>(i))) & 0x8000)
            {
                m_keyboard[i].b_down = true;
                /* KeyboardEvent key_down_event(ButtonEventState::ButtonDown);
                 key_down_event.key = static_cast<KeyID_Keyboard>(i);
                 m_subject->Notify(key_down_event, KeyboardEvent::StateToString(ButtonEventState::ButtonDown));*/
            }
            else
            {
                /*if (m_keyboard[ i ].b_curr_pressed== false && m_keyboard[ i ].b_prev_pressed == true)
                {
                    KeyboardEvent key_up_event(ButtonEventState::ButtonUp);
                    key_up_event.key = static_cast<KeyID_Keyboard>(i);
                    m_subject->Notify(key_up_event, KeyboardEvent::StateToString(ButtonEventState::ButtonUp));
                }*/
                m_keyboard[i].b_down         = false;
                m_keyboard[i].b_prev_pressed = false;
                m_keyboard[i].b_curr_pressed = false;
            }
            if (m_keyboard[i].b_down == true)
            {
                if (m_keyboard[i].b_curr_pressed == false && m_keyboard[i].b_prev_pressed == false)
                {
                    m_keyboard[i].b_curr_pressed = true;
                    m_keyboard[i].b_prev_pressed = false;
                    /* KeyboardEvent key_press_event(ButtonEventState::ButtonPressed);
                     key_press_event.key = static_cast<KeyID_Keyboard>(i);
                     m_subject->Notify(key_press_event, KeyboardEvent::StateToString(ButtonEventState::ButtonPressed));*/
                }
                else if (m_keyboard[i].b_curr_pressed == true && m_keyboard[i].b_prev_pressed == false)
                {
                    m_keyboard[i].b_curr_pressed = false;
                    m_keyboard[i].b_prev_pressed = true;
                }
            }
        }
    }

    bool KeyboardInput::IsDown(eKeyCodeKeyboard key_code) const
    {
        return GUISystem::IsFocusGUI() ? false : m_keyboard[static_cast<int>(key_code)].b_down;
    }

    bool KeyboardInput::IsPressed(eKeyCodeKeyboard key_code) const
    {
        return GUISystem::IsFocusGUI() ? false : m_keyboard[static_cast<int>(key_code)].b_curr_pressed;
    }

    bool KeyboardInput::IsAnyKeyDown() const
    {
        if (GUISystem::IsFocusGUI())
            return false;
        for (size_t i = 0; i < MAXIMUM_KEY_COUNT; ++i)
        {
            if (m_keyboard[i].b_down == true)
                return true;
        }
        return false;
    }

    bool KeyboardInput::IsAnyKeyPressed() const
    {
        if (GUISystem::IsFocusGUI())
            return false;
        for (int i = 0; i < MAXIMUM_KEY_COUNT; ++i)
        {
            if (m_keyboard[i].b_curr_pressed == true)
                return true;
        }
        return false;
    }

    size_t KeyboardInput::Repeated(eKeyCodeKeyboard key_code) const
    {
        E5_UNUSED_PARAM(key_code);
        return 0;
    }

    std::string KeyboardInput::TypedString() const
    {
        return ToString(*m_string);
    }

    std::wstring* KeyboardInput::TypedWString() const
    {
        return m_string;
    }

    void KeyboardInput::SetEditingWString(std::wstring* string)
    {
        m_string = string;
    }
}
