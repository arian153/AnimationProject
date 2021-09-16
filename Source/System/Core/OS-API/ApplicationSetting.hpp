#pragma once
#include "WindowMode.hpp"
#include "../../Math/Utility/MathDef.hpp"
#include <string>

namespace CS460
{
    struct ApplicationSetting
    {
    public:
        bool b_confine_cursor = false;
        bool b_show_cursor    = true;
        bool b_v_sync         = true;

        int  screen_width  = 1280;
        int  screen_height = 720;
        Real screen_scale  = 1.0f;

        std::string caption       = "";
        std::string initial_level = "";
        eWindowMode window_mode   = eWindowMode::Windowed;
    };
}
