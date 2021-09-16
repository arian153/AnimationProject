#pragma once
#include "../OS-API/OSAPI.hpp"
#include IncludeKeyboardAPI
#include "KeyCode.hpp"
#include <string>

namespace CS460
{
    class KeyboardInput : public KeyboardInputAPI
    {
    public:
        KeyboardInput();
        ~KeyboardInput();

    public: //interface 
        bool IsDown(eKeyCodeKeyboard key_code) const;
        bool IsPressed(eKeyCodeKeyboard key_code) const;
        bool IsAnyKeyDown() const;
        bool IsAnyKeyPressed() const;

        size_t        Repeated(eKeyCodeKeyboard key_code) const;
        std::string   TypedString() const;
        std::wstring* TypedWString() const;
        void          SetEditingWString(std::wstring* string);

    private:
        const static size_t MAXIMUM_KEY_COUNT = (size_t)eKeyCodeKeyboard::MAX;

        void Reset();
        void ResetPressed();
        void ProcessPressed();

    private:
        friend class InputCommon;

    private:
        ButtonState m_keyboard[ MAXIMUM_KEY_COUNT ];
    };
}
