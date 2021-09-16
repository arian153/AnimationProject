#pragma once
#include <vector>
#include <unordered_map>

namespace CS460
{
    class ForceFactory;
    class World;

    class PhysicsSystem
    {
    public:
        PhysicsSystem();
        ~PhysicsSystem();

        void Initialize();
        void Shutdown();

        World* CreateWorld();
        void   RemoveWorld(World* world);

        bool AddFactory(ForceFactory* factory);

    private:
        std::unordered_map<std::string, ForceFactory*> m_factories;
        std::vector<World*> m_worlds;
    };
}
