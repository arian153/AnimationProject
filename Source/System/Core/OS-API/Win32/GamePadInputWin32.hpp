#pragma once
#include <windows.h>
#include <Xinput.h>

namespace CS460
{
    struct PadState;
    class GamePadInputWin32
    {
    public:
        GamePadInputWin32();
        ~GamePadInputWin32();

    protected:
        void ProcessButtons(XINPUT_GAMEPAD* game_pad, PadState& pad_state) const;
        void ProcessLeftThumb(XINPUT_GAMEPAD* game_pad, PadState& pad_state) const;
        void ProcessRightThumb(XINPUT_GAMEPAD* game_pad, PadState& pad_state) const;
        void ProcessTrigger(XINPUT_GAMEPAD* game_pad, PadState& pad_state) const;

    protected:
        const static size_t USER_MAX_COUNT = XUSER_MAX_COUNT;
        XINPUT_STATE        m_controllers[USER_MAX_COUNT];
    };
}
