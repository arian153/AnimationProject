#include "CommandRegistry.hpp"
#include "EditorCommand.hpp"

namespace CS460
{
    CommandRegistry::CommandRegistry()
    {
    }

    CommandRegistry::~CommandRegistry()
    {
    }

    void CommandRegistry::Initialize()
    {
    }

    void CommandRegistry::Shutdown()
    {
        for (auto& command : m_command_registry)
        {
            delete command;
        }
        for (auto& undo_command : m_undo_registry)
        {
            delete undo_command;
        }
        m_command_registry.clear();
        m_undo_registry.clear();
    }

    void CommandRegistry::PushCommand(EditorCommand* command)
    {
        ClearUndoCommands();
        if (m_command_registry.size() >= m_registry_size)
        {
            auto front_command = m_command_registry.front();
            delete front_command;
            front_command = nullptr;
            m_command_registry.erase(m_command_registry.begin());
        }
        m_command_registry.push_back(command);
        command->SetRegistry(this);
        command->Execute();
    }

    void CommandRegistry::UndoCommand()
    {
        if (!m_command_registry.empty())
        {
            m_undo_registry.push_back(m_command_registry.back());
            m_command_registry.back()->UnExecute();
            m_command_registry.pop_back();
        }
    }

    void CommandRegistry::RedoCommand()
    {
        if (!m_undo_registry.empty())
        {
            m_command_registry.push_back(m_undo_registry.back());
            m_undo_registry.back()->Execute();
            m_undo_registry.pop_back();
        }
    }

    void CommandRegistry::CopyCommand(void* data)
    {
        //temp code
        m_copied = data;
    }

    void CommandRegistry::ClearUndoCommands()
    {
        if (!m_undo_registry.empty())
        {
            for (auto& undo_command : m_undo_registry)
            {
                delete undo_command;
                undo_command = nullptr;
            }
            m_undo_registry.clear();
        }
    }

    void CommandRegistry::Clear()
    {
        if (!m_undo_registry.empty())
        {
            for (auto& undo_command : m_undo_registry)
            {
                delete undo_command;
                undo_command = nullptr;
            }
            m_undo_registry.clear();
        }

        if (!m_command_registry.empty())
        {
            for (auto& command : m_command_registry)
            {
                delete command;
                command = nullptr;
            }
            m_command_registry.clear();
        }
    }
}
