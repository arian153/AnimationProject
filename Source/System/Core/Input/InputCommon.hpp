#pragma once

namespace CS460
{
    class MouseInput;
    class KeyboardInput;
    class GamePadInput;

    class InputCommon
    {
    public:
        InputCommon();
        ~InputCommon();

        void Initialize();
        void Update() const;
        void Shutdown();

        MouseInput*    GetMouseInput() const;
        KeyboardInput* GetKeyboardInput() const;
        GamePadInput*  GetGamePadInput() const;
    private:
        MouseInput*    m_mouse_input    = nullptr;
        KeyboardInput* m_keyboard_input = nullptr;
        GamePadInput*  m_game_pad_input = nullptr;
    };
}
