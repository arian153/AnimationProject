#pragma once
#include <vector>

namespace CS460
{
    class TextRenderer;
    class InputCommon;
    class LogicSubsystem;

    class LogicSystem
    {
    public:
        LogicSystem();
        ~LogicSystem();

        void Initialize();
        void Shutdown();

        void SetInput(InputCommon* input);
        void SetTextRenderer(TextRenderer* text_renderer);

        LogicSubsystem* CreateLogicSubsystem();
        void            RemoveLogicSubsystem(LogicSubsystem* logic_subsystem);

    private:
        std::vector<LogicSubsystem*> m_subsystems;

        //others
        InputCommon*  m_input         = nullptr;
        TextRenderer* m_text_renderer = nullptr;
    };
}
