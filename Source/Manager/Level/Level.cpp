#include "Level.hpp"
#include "../Space/Space.hpp"

namespace CS460
{
    Level::Level(const std::string& name)
        : m_name(name)
    {
    }

    Level::~Level()
    {
    }

    void Level::Initialize() const
    {
    }

    void Level::Update(Real dt) const
    {
    }

    void Level::FixedUpdate(Real dt) const
    {
    }

    void Level::Shutdown() const
    {
    }

    void Level::Load() const
    {
    }

    void Level::Unload() const
    {
    }

    Space* Level::GetGlobalSpace() const
    {
        return m_global_space;
    }

    Space* Level::GetSpace(size_t index) const
    {
        return m_spaces.at(index);
    }

    Space* Level::GetSpace(const std::string& name) const
    {
        for (auto& space : m_spaces)
        {
            if (space->GetName() == name)
            {
                return space;
            }
        }
        return nullptr;
    }

    void Level::UpdateSpace(Real dt, size_t index, eSubsystemFlag flag) const
    {
        auto space = m_spaces.at(index);
        UpdateSpace(dt, space, flag);
    }

    void Level::UpdateSubsystem(Real dt, eSubsystemFlag flag) const
    {
        if (m_global_space->IsSubsystemUpdate(flag))
        {
            UpdateSpace(dt, m_global_space, flag);
        }
        for (auto& space : m_spaces)
        {
            if (space->IsSubsystemUpdate(flag))
            {
                UpdateSpace(dt, space, flag);
            }
        }
    }

    void Level::DrawSubsystem(eSubsystemFlag flag)
    {
        if (m_global_space->IsSubsystemUpdate(flag) || m_global_space->IsSubsystemFixedUpdate(flag))
        {
            RenderSpace(m_global_space, flag);
        }
        for (auto& space : m_spaces)
        {
            if (space->IsSubsystemUpdate(flag) || space->IsSubsystemFixedUpdate(flag))
            {
                RenderSpace(space, flag);
            }
        }
    }

    void Level::FixedUpdateSubsystem(Real dt, eSubsystemFlag flag) const
    {
        if (m_global_space->IsSubsystemFixedUpdate(flag))
        {
            UpdateSpace(dt, m_global_space, flag);
        }
        for (auto& space : m_spaces)
        {
            if (space->IsSubsystemFixedUpdate(flag))
            {
                UpdateSpace(dt, space, flag);
            }
        }
    }

    void Level::AddSpaceResource(JsonResource* resource)
    {
        if (resource != nullptr)
        {
            m_space_resources.push_back(resource);
        }
    }

    void Level::UpdateSpace(Real dt, Space* space, eSubsystemFlag flag) const
    {
        if (space->IsActivated())
        {
            space->UpdateSubsystem(dt, flag);
        }
    }

    void Level::RenderSpace(Space* space, eSubsystemFlag flag)
    {
        space->RenderSubsystem(flag);
    }
}
