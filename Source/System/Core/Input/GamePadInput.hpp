#pragma once
#include "../OS-API/OSAPI.hpp"
#include IncludeGamePadAPI
#include "../../Math/Utility/MathDef.hpp"
#include "KeyCode.hpp"

namespace CS460
{
    struct AnalogStick
    {
        Real x;
        Real y;
    };

    struct PadState
    {
        bool        b_plugged = false;
        ButtonState button_state[ (size_t)eKeyCodeGamePad::MAX ];

        //analog data
        Real trigger_right       = 0.0f;
        Real trigger_left        = 0.0f;
        Real thumb_stick_right_x = 0.0f;
        Real thumb_stick_right_y = 0.0f;
        Real thumb_stick_left_x  = 0.0f;
        Real thumb_stick_left_y  = 0.0f;
    };

    class GamePadInput : public GamePadInputAPI
    {
    public:
        GamePadInput();
        ~GamePadInput();

    public: //interface 
        bool IsDown(eKeyCodeGamePad key_code, size_t pad = 0) const;
        bool IsPressed(eKeyCodeGamePad key_code, size_t pad = 0) const;

        bool IsAnyKeyDown() const;
        bool IsAnyKeyPressed() const;

        AnalogStick LeftAnalogStick(size_t pad = 0) const;
        AnalogStick RightAnalogStick(size_t pad = 0) const;

        Real LeftStickAngle_Rad(size_t pad = 0) const;
        Real RightStickAngle_Rad(size_t pad = 0) const;

        Real LeftStickAngle_Deg(size_t pad = 0) const;
        Real RightStickAngle_Deg(size_t pad = 0) const;

        Real LeftTrigger(size_t pad = 0) const;
        Real RightTrigger(size_t pad = 0) const;

        bool IsGamePadPluggedIn(size_t pad = 0) const;

        void MakeVibration(int left_motor = 4096, int right_motor = 4096, int pad = 0);
    private:
        const size_t MAXIMUM_GAME_PAD_BUTTON_COUNT = (size_t)eKeyCodeGamePad::MAX;

        void Reset();
        void ResetPressed();
        void ProcessGamePad();
        void ProcessPressed();
    private:
        friend class InputCommon;
    private:
        PadState m_game_pad_state[ USER_MAX_COUNT ];
    };
}
