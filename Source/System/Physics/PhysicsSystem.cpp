#include "PhysicsSystem.hpp"
#include "Dynamics/World.hpp"
#include "Resolution/Force/Force.hpp"
#include "Resolution/Force/Drag.hpp"
#include "Resolution/Force/Gravity.hpp"

namespace CS460
{
    PhysicsSystem::PhysicsSystem()
    {
    }

    PhysicsSystem::~PhysicsSystem()
    {
    }

    void PhysicsSystem::Initialize()
    {
        AddFactory(new DragFactory());
        AddFactory(new GravityFactory());
    }

    void PhysicsSystem::Shutdown()
    {
        for (auto& world : m_worlds)
        {
            world->Shutdown();
            delete world;
            world = nullptr;
        }
        m_worlds.clear();

        for (auto it = m_factories.begin(); it != m_factories.end(); ++it)
        {
            delete it->second;
            it->second = nullptr;
        }
        m_factories.clear();
    }

    World* PhysicsSystem::CreateWorld()
    {
        World* world = new World();
        world->SetForceFactory(&m_factories);
        m_worlds.push_back(world);
        return world;
    }

    void PhysicsSystem::RemoveWorld(World* world)
    {
        if (world != nullptr)
        {
            auto found = std::find(m_worlds.begin(), m_worlds.end(), world);
            m_worlds.erase(found);
            world->Shutdown();
            delete world;
            world = nullptr;
        }
    }

    bool PhysicsSystem::AddFactory(ForceFactory* factory)
    {
        if (factory != nullptr)
        {
            if (m_factories.find(factory->type) == m_factories.end())
            {
                m_factories.emplace(factory->type, factory);
                return true;
            }
        }
        return false;
    }
}
