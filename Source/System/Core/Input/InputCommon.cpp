#include "InputCommon.hpp"
#include "MouseInput.hpp"
#include "GamePadInput.hpp"
#include "KeyboardInput.hpp"

namespace CS460
{
    InputCommon::InputCommon()
    {
    }

    InputCommon::~InputCommon()
    {
    }

    void InputCommon::Initialize()
    {
        m_mouse_input    = new MouseInput();
        m_keyboard_input = new KeyboardInput();
        m_game_pad_input = new GamePadInput();
    }

    void InputCommon::Update() const
    {
        m_game_pad_input->ProcessGamePad();
        m_game_pad_input->ProcessPressed();
        m_keyboard_input->ProcessPressed();
        m_mouse_input->ProcessPressed();
    }

    void InputCommon::Shutdown()
    {
        if (m_mouse_input != nullptr)
        {
            delete m_mouse_input;
            m_mouse_input = nullptr;
        }
        if (m_keyboard_input != nullptr)
        {
            delete m_keyboard_input;
            m_keyboard_input = nullptr;
        }
        if (m_game_pad_input != nullptr)
        {
            delete m_game_pad_input;
            m_game_pad_input = nullptr;
        }
    }

    MouseInput* InputCommon::GetMouseInput() const
    {
        return m_mouse_input;
    }

    KeyboardInput* InputCommon::GetKeyboardInput() const
    {
        return m_keyboard_input;
    }

    GamePadInput* InputCommon::GetGamePadInput() const
    {
        return m_game_pad_input;
    }
}
