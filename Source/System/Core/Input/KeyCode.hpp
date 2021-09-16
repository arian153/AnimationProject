#pragma once

namespace CS460
{
    enum class eKeyCodeMouse
    {
        NONE
      , Left
      , Right
      , Middle
      , X1
      , X2
      , Max
    };

    enum class eKeyCodeKeyboard
    {
        NONE
      , _0
      , _1
      , _2
      , _3
      , _4
      , _5
      , _6
      , _7
      , _8
      , _9
      , A
      , B
      , C
      , D
      , E
      , F
      , G
      , H
      , I
      , J
      , K
      , L
      , M
      , N
      , O
      , P
      , Q
      , R
      , S
      , T
      , U
      , V
      , W
      , X
      , Y
      , Z
      , F1
      , F2
      , F3
      , F4
      , F5
      , F6
      , F7
      , F8
      , F9
      , F10
      , F11
      , F12
      , Arrow_Up
      , Arrow_Down
      , Arrow_Right
      , Arrow_Left
      , Enter
      , Escape
      , Backspace
      , Tab
      , Space
      , Control_Left
      , Control_Right
      , Shift_Left
      , Shift_Right
      , Alt_Left
      , Alt_Right
      , Period
      , Comma
      , Slash
      , Semicolon
      , Quote
      , Bracket_Left
      , Bracket_Right
      , Backslash
      , Minus
      , Equal
      , PrintScreen
      , ScrollLock
      , Pause
      , CapsLock
      , Insert
      , Home
      , PageUp
      , Delete
      , End
      , PageDown
      , Application
      , Power
      , Select
      , MAX
    };

    enum class eKeyCodeGamePad
    {
        NONE
      , DPAD_Up
      , DPAD_Down
      , DPAD_Right
      , DPAD_Left
      , Start
      , Back
      , X
      , Y
      , A
      , B
      , Shoulder_Left
      , Shoulder_Right
      , Thumb_Left
      , Thumb_Right
      , MAX
    };

    enum class eKeyCodeTextEditor
    {
        Tab
      , LeftArrow
      , RightArrow
      , UpArrow
      , DownArrow
      , PageUp
      , PageDown
      , Home
      , End
      , Insert
      , Delete
      , Backspace
      , Space
      , Enter
      , Esc
      , A //CTRL+A: select all
      , C //CTRL+C: copy
      , V //CTRL+V: paste
      , X //CTRL+X: cut
      , Y //CTRL+Y: redo
      , Z //CTRL+Z: undo
      , MAX
    };

    enum class eKeyCodeGeneralEditor
    {
        Delete
      , C //CTRL+C: copy
      , V //CTRL+V: paste
      , X //CTRL+X: cut
      , Y //CTRL+Y: redo
      , Z //CTRL+Z: undo
      , MAX
    };

    struct ButtonState
    {
        bool b_prev_pressed = false;
        bool b_curr_pressed = false;
        bool b_down         = false;
    };

    enum class ButtonEventState
    {
        ButtonUp
      , ButtonDown
      , ButtonPressed
    };
}
