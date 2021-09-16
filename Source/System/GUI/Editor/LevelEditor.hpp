#pragma once
#include "../../../External/imgui/imgui.h"
#include "../../Math/Utility/MathDef.hpp"

namespace CS460
{
    class Level;

    class LevelEditor
    {
    public:
        LevelEditor();
        ~LevelEditor();

        bool Open(Level* space);
        void Close();

        void Update(Real dt);

    private:
        friend class GameEditor;

    private:
        bool   m_b_open        = false;
        Level* m_editing_level = nullptr;
    };
}
