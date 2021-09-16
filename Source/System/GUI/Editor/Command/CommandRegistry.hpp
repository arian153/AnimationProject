#pragma once
#include <vector>

namespace CS460
{
    class EditorCommand;

    class CommandRegistry
    {
    public:
        CommandRegistry();
        ~CommandRegistry();

        void Initialize();
        void Shutdown();

        void PushCommand(EditorCommand* command);
        void UndoCommand();
        void RedoCommand();
        void CopyCommand(void* data);
        void ClearUndoCommands();
        void Clear();

    private:
        friend class GameEditor;
    private:
        std::vector<EditorCommand*> m_command_registry;
        std::vector<EditorCommand*> m_undo_registry;

        //data
        void*  m_copied        = nullptr;
        size_t m_registry_size = 40;
    };
}
