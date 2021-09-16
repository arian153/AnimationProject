#pragma once
#include "../../Core/Utility/CoreDef.hpp"
#include "../../Math/Utility/MathDef.hpp"

namespace CS460
{
    class Color
    {
    public:
        explicit Color(Real r = 1.0f, Real g = 1.0f, Real b = 1.0f, Real a = 1.0f);
        ~Color();

        bool operator==(const Color& rhs) const;

        Real ToReal(U8 color);
        Real ToReal(int color);
        U8   ToByte(Real color);
        int  ToInt(Real color);

    public:
        Real r, g, b, a;
    };

    namespace ColorDef
    {
        namespace Pure
        {
            const static Color White   = Color(1.000000f, 1.000000f, 1.000000f, 1.000000f); //255, 255, 255
            const static Color Gray    = Color(0.666666f, 0.666666f, 0.666666f, 1.000000f); //170, 170, 170
            const static Color Black   = Color(0.000000f, 0.000000f, 0.000000f, 1.000000f); //  0,   0,   0
            const static Color Red     = Color(1.000000f, 0.000000f, 0.000000f, 1.000000f); //255,   0,   0
            const static Color Green   = Color(0.000000f, 1.000000f, 0.000000f, 1.000000f); //  0, 255,   0
            const static Color Blue    = Color(0.000000f, 0.000000f, 1.000000f, 1.000000f); //  0,   0, 255
            const static Color Cyan    = Color(0.000000f, 1.000000f, 1.000000f, 1.000000f); //  0, 255, 255
            const static Color Magenta = Color(1.000000f, 0.000000f, 1.000000f, 1.000000f); //255,   0, 255
            const static Color Yellow  = Color(1.000000f, 1.000000f, 0.000000f, 1.000000f); //255, 255,   0
        }

        namespace UI
        {
            const static Color SelectedWhite       = Color(0.937254f, 0.921568f, 0.937254f, 1.000000f); //239, 235, 239
            const static Color HighlightedWhite    = Color(0.862745f, 0.862745f, 0.862745f, 1.000000f); //220, 220, 220
            const static Color HighlightedGray     = Color(0.619607f, 0.619607f, 0.619607f, 1.000000f); //158, 158, 158
            const static Color Gray                = Color(0.407843f, 0.407843f, 0.407843f, 1.000000f); //104, 104, 104
            const static Color HighlightedDarkGray = Color(0.247058f, 0.247058f, 0.274509f, 1.000000f); // 63,  63,  70
            const static Color DarkGray            = Color(0.176470f, 0.176470f, 0.188235f, 1.000000f); // 45,  45,  48
            const static Color BackgroundDarkGray  = Color(0.117647f, 0.117647f, 0.117647f, 1.000000f); // 30,  30,  30
        }
    }

    class ColorFlag
    {
    public:
        bool  b_flag = false;
        Color color  = ColorDef::Pure::White;
    };
}
