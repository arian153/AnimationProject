#include "GamePadInputWin32.hpp"
#include "../../../Math/Utility/Utility.hpp"
#include "../../Input/KeyCode.hpp"
#include "../../Input/GamePadInput.hpp"
#include "../../../GUI/GUISystem.hpp"

namespace CS460
{
    GamePadInputWin32::GamePadInputWin32()
        : m_controllers{}
    {
    }

    GamePadInputWin32::~GamePadInputWin32()
    {
    }

    void GamePadInputWin32::ProcessButtons(XINPUT_GAMEPAD* game_pad, PadState& pad_state) const
    {
        pad_state.button_state[(size_t)eKeyCodeGamePad::DPAD_Up].b_down        = (game_pad->wButtons & XINPUT_GAMEPAD_DPAD_UP) != 0;
        pad_state.button_state[(size_t)eKeyCodeGamePad::DPAD_Down].b_down      = (game_pad->wButtons & XINPUT_GAMEPAD_DPAD_DOWN) != 0;
        pad_state.button_state[(size_t)eKeyCodeGamePad::DPAD_Left].b_down      = (game_pad->wButtons & XINPUT_GAMEPAD_DPAD_LEFT) != 0;
        pad_state.button_state[(size_t)eKeyCodeGamePad::DPAD_Right].b_down     = (game_pad->wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) != 0;
        pad_state.button_state[(size_t)eKeyCodeGamePad::Start].b_down          = (game_pad->wButtons & XINPUT_GAMEPAD_START) != 0;
        pad_state.button_state[(size_t)eKeyCodeGamePad::Back].b_down           = (game_pad->wButtons & XINPUT_GAMEPAD_BACK) != 0;
        pad_state.button_state[(size_t)eKeyCodeGamePad::X].b_down              = (game_pad->wButtons & XINPUT_GAMEPAD_X) != 0;
        pad_state.button_state[(size_t)eKeyCodeGamePad::Y].b_down              = (game_pad->wButtons & XINPUT_GAMEPAD_Y) != 0;
        pad_state.button_state[(size_t)eKeyCodeGamePad::A].b_down              = (game_pad->wButtons & XINPUT_GAMEPAD_A) != 0;
        pad_state.button_state[(size_t)eKeyCodeGamePad::B].b_down              = (game_pad->wButtons & XINPUT_GAMEPAD_B) != 0;
        pad_state.button_state[(size_t)eKeyCodeGamePad::Shoulder_Left].b_down  = (game_pad->wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) != 0;
        pad_state.button_state[(size_t)eKeyCodeGamePad::Shoulder_Right].b_down = (game_pad->wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) != 0;
        pad_state.button_state[(size_t)eKeyCodeGamePad::Thumb_Left].b_down     = (game_pad->wButtons & XINPUT_GAMEPAD_LEFT_THUMB) != 0;
        pad_state.button_state[(size_t)eKeyCodeGamePad::Thumb_Right].b_down    = (game_pad->wButtons & XINPUT_GAMEPAD_RIGHT_THUMB) != 0;
        for (size_t code = 0; code < (size_t)eKeyCodeGamePad::MAX; ++code)
        {
            /*if (pad_state.button_state[code].b_down == true)
            {
                GamePadEvent button_down_event(ButtonEventState::ButtonDown);
                button_down_event.key = static_cast<eKeyCodeGamePad>(code);
                m_subject->Notify(button_down_event, GamePadEvent::StateToString(ButtonEventState::ButtonDown));
            }
            else*/
            {
                /*if (pad_state.button_state[code].b_curr_pressed == false && pad_state.button_state[code].b_prev_pressed == true)
                {
                    GamePadEvent button_up_event(ButtonEventState::ButtonUp);
                    button_up_event.key = static_cast<eKeyCodeGamePad>(code);
                    m_subject->Notify(button_up_event, GamePadEvent::StateToString(ButtonEventState::ButtonUp));
                }*/
                pad_state.button_state[code].b_prev_pressed = false;
                pad_state.button_state[code].b_curr_pressed = false;
            }
        }
    }

    void GamePadInputWin32::ProcessLeftThumb(XINPUT_GAMEPAD* game_pad, PadState& pad_state) const
    {
        Real left_x = game_pad->sThumbLX;
        Real left_y = game_pad->sThumbLY;
        //determine how far the controller is pushed
        Real magnitude = sqrt(left_x * left_x + left_y * left_y);
        //determine the direction the controller is pushed
        Real normalized_lx = left_x / magnitude;
        Real normalized_ly = left_y / magnitude;
        Real normalized_magnitude;
        //check if the controller is outside a circular dead zone
        if (magnitude > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
        {
            //clip the magnitude at its expected maximum value
            if (magnitude > 32767)
                magnitude = 32767;
            //adjust magnitude relative to the end of the dead zone
            magnitude -= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
            //optionally normalize the magnitude with respect to its expected range
            //giving a magnitude value of 0.0 to 1.0
            normalized_magnitude = magnitude / (32767 - XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
        }
        else //if the controller is in the dead zone zero out the magnitude
        {
            normalized_magnitude = 0.0;
        }
        pad_state.thumb_stick_left_x = normalized_lx * normalized_magnitude;
        pad_state.thumb_stick_left_y = normalized_ly * normalized_magnitude;
    }

    void GamePadInputWin32::ProcessRightThumb(XINPUT_GAMEPAD* game_pad, PadState& pad_state) const
    {
        Real right_x = game_pad->sThumbRX;
        Real right_y = game_pad->sThumbRY;
        //determine how far the controller is pushed
        Real magnitude = sqrtf(right_x * right_x + right_y * right_y);
        //determine the direction the controller is pushed
        Real normalized_rx = right_x / magnitude;
        Real normalized_ry = right_y / magnitude;
        Real normalized_magnitude;
        //check if the controller is outside a circular dead zone
        if (magnitude > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
        {
            //clip the magnitude at its expected maximum value
            if (magnitude > 32767)
                magnitude = 32767;
            //adjust magnitude relative to the end of the dead zone
            magnitude -= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
            //optionally normalize the magnitude with respect to its expected range
            //giving a magnitude value of 0.0 to 1.0
            normalized_magnitude = magnitude / (32767 - XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
        }
        else //if the controller is in the dead zone zero out the magnitude
        {
            normalized_magnitude = 0.0;
        }
        pad_state.thumb_stick_right_x = normalized_rx * normalized_magnitude;
        pad_state.thumb_stick_right_y = normalized_ry * normalized_magnitude;
    }

    void GamePadInputWin32::ProcessTrigger(XINPUT_GAMEPAD* game_pad, PadState& pad_state) const
    {
        Real right_trigger      = game_pad->bRightTrigger;
        Real left_trigger       = game_pad->bLeftTrigger;
        pad_state.trigger_left  = left_trigger / 255.0f;
        pad_state.trigger_right = right_trigger / 255.0f;
    }

    GamePadInput::GamePadInput()
        : m_game_pad_state{}
    {
        Reset();
    }

    GamePadInput::~GamePadInput()
    {
    }

    void GamePadInput::Reset()
    {
        for (size_t j = 0; j < USER_MAX_COUNT; ++j)
        {
            for (size_t i = 0; i < MAXIMUM_GAME_PAD_BUTTON_COUNT; ++i)
            {
                m_game_pad_state[j].button_state[i].b_down         = false;
                m_game_pad_state[j].button_state[i].b_curr_pressed = false;
                m_game_pad_state[j].button_state[i].b_prev_pressed = false;
            }
        }
    }

    void GamePadInput::ResetPressed()
    {
        for (size_t j = 0; j < USER_MAX_COUNT; ++j)
        {
            for (size_t i = 0; i < MAXIMUM_GAME_PAD_BUTTON_COUNT; ++i)
            {
                m_game_pad_state[j].button_state[i].b_curr_pressed = false;
            }
        }
    }

    void GamePadInput::ProcessGamePad()
    {
        for (DWORD controller_index = 0; controller_index < XUSER_MAX_COUNT; ++controller_index)
        {
            if (XInputGetState(controller_index, &m_controllers[controller_index]) == ERROR_SUCCESS)
            {
                m_game_pad_state[controller_index].b_plugged = true;
                //Controller is plugged in.
                //dwPacketNumber
                XINPUT_GAMEPAD* game_pad = &m_controllers[controller_index].Gamepad;
                ProcessButtons(game_pad, m_game_pad_state[controller_index]);
                ProcessLeftThumb(game_pad, m_game_pad_state[controller_index]);
                ProcessRightThumb(game_pad, m_game_pad_state[controller_index]);
                ProcessTrigger(game_pad, m_game_pad_state[controller_index]);
            }
            else
            {
                m_game_pad_state[controller_index].b_plugged = false;
                //not plugged in.
            }
        }
    }

    void GamePadInput::ProcessPressed()
    {
        for (size_t pad = 0; pad < USER_MAX_COUNT; ++pad)
            if (m_game_pad_state[pad].b_plugged == true)
            {
                for (size_t i = 0; i < MAXIMUM_GAME_PAD_BUTTON_COUNT; ++i)
                {
                    if (m_game_pad_state[pad].button_state[i].b_down == true)
                    {
                        if (m_game_pad_state[pad].button_state[i].b_curr_pressed == false && m_game_pad_state[pad].button_state[i].b_prev_pressed == false)
                        {
                            m_game_pad_state[pad].button_state[i].b_curr_pressed = true;
                            m_game_pad_state[pad].button_state[i].b_prev_pressed = false;
                            /*GamePadEvent press_event(ButtonEventState::ButtonPressed);
                            press_event.key = static_cast<eKeyCodeGamePad>(i);
                            m_subject->Notify(press_event, GamePadEvent::StateToString(ButtonEventState::ButtonPressed));*/
                        }
                        else if (m_game_pad_state[pad].button_state[i].b_curr_pressed == true && m_game_pad_state[pad].button_state[i].b_prev_pressed == false)
                        {
                            m_game_pad_state[pad].button_state[i].b_curr_pressed = false;
                            m_game_pad_state[pad].button_state[i].b_prev_pressed = true;
                        }
                    }
                    /*else
                    {
                        m_game_pad_state[pad].button_state[i].b_prev_pressed = false;
                        m_game_pad_state[pad].button_state[i].b_curr_pressed = false;
                    }*/
                }
            }
    }

    bool GamePadInput::IsDown(eKeyCodeGamePad key_code, size_t pad) const
    {
        return GUISystem::IsFocusGUI() ? false : m_game_pad_state[pad].button_state[static_cast<size_t>(key_code)].b_down;
    }

    bool GamePadInput::IsPressed(eKeyCodeGamePad key_code, size_t pad) const
    {
        return GUISystem::IsFocusGUI() ? false : m_game_pad_state[pad].button_state[static_cast<size_t>(key_code)].b_curr_pressed;
    }

    bool GamePadInput::IsAnyKeyDown() const
    {
        if (GUISystem::IsFocusGUI())
            return false;
        for (size_t j = 0; j < USER_MAX_COUNT; ++j)
        {
            for (size_t i = 0; i < MAXIMUM_GAME_PAD_BUTTON_COUNT; ++i)
            {
                if (m_game_pad_state[j].button_state[i].b_down == true)
                {
                    return true;
                }
            }
        }
        return false;
    }

    bool GamePadInput::IsAnyKeyPressed() const
    {
        if (GUISystem::IsFocusGUI())
            return false;
        for (size_t j = 0; j < USER_MAX_COUNT; ++j)
        {
            for (size_t i = 0; i < MAXIMUM_GAME_PAD_BUTTON_COUNT; ++i)
            {
                if (m_game_pad_state[j].button_state[i].b_curr_pressed == true)
                {
                    return true;
                }
            }
        }
        return false;
    }

    AnalogStick GamePadInput::LeftAnalogStick(size_t pad) const
    {
        AnalogStick result;
        result.x = m_game_pad_state[pad].thumb_stick_left_x;
        result.y = m_game_pad_state[pad].thumb_stick_left_y;
        return GUISystem::IsFocusGUI() ? AnalogStick() : result;
    }

    AnalogStick GamePadInput::RightAnalogStick(size_t pad) const
    {
        AnalogStick result;
        result.x = m_game_pad_state[pad].thumb_stick_right_x;
        result.y = m_game_pad_state[pad].thumb_stick_right_y;
        return GUISystem::IsFocusGUI() ? AnalogStick() : result;
    }

    Real GamePadInput::LeftStickAngle_Rad(size_t pad) const
    {
        return GUISystem::IsFocusGUI() ? 0.0f : Math::XYToRadian(m_game_pad_state[pad].thumb_stick_left_x, m_game_pad_state[pad].thumb_stick_left_y);
    }

    Real GamePadInput::RightStickAngle_Rad(size_t pad) const
    {
        return GUISystem::IsFocusGUI() ? 0.0f : Math::XYToRadian(m_game_pad_state[pad].thumb_stick_right_x, m_game_pad_state[pad].thumb_stick_right_y);
    }

    Real GamePadInput::LeftStickAngle_Deg(size_t pad) const
    {
        return GUISystem::IsFocusGUI() ? 0.0f : Math::RadiansToDegrees(LeftStickAngle_Rad(pad));
    }

    Real GamePadInput::RightStickAngle_Deg(size_t pad) const
    {
        return GUISystem::IsFocusGUI() ? 0.0f : Math::RadiansToDegrees(RightStickAngle_Rad(pad));
    }

    Real GamePadInput::LeftTrigger(size_t pad) const
    {
        return GUISystem::IsFocusGUI() ? 0.0f : m_game_pad_state[pad].trigger_left;
    }

    Real GamePadInput::RightTrigger(size_t pad) const
    {
        return GUISystem::IsFocusGUI() ? 0.0f : m_game_pad_state[pad].trigger_right;
    }

    bool GamePadInput::IsGamePadPluggedIn(size_t pad) const
    {
        return GUISystem::IsFocusGUI() ? false : m_game_pad_state[pad].b_plugged;
    }

    void GamePadInput::MakeVibration(int left_motor, int right_motor, int pad)
    {
        XINPUT_VIBRATION Vibration;
        ZeroMemory(&Vibration, sizeof(XINPUT_VIBRATION));
        Vibration.wLeftMotorSpeed  = (WORD)left_motor;
        Vibration.wRightMotorSpeed = (WORD)right_motor;
        XInputSetState(pad, &Vibration);
    }
}
