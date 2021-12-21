// ReSharper disable CppFunctionIsNotImplemented
#pragma once
#include <unordered_map>

namespace CS460
{
    enum class eCollisionState : int
    {
        None = 0
      , Start
      , Persist
      , End
      , Invalid
    };

    class ColliderPrimitive;
    class RigidContactManifold;
    class ColliderSet;

    class CollisionStateData
    {
    public:
        ColliderSet*   set_a = nullptr;
        ColliderSet*   set_b = nullptr;
        eCollisionState state = eCollisionState::None;
    };

    class ManifoldTable
    {
    public:
        ManifoldTable();
        ~ManifoldTable();

        void Initialize();
        void Shutdown();

        void SendHasCollision(ColliderSet* a, ColliderSet* b, bool was_collision);
        void SendNotCollision(ColliderSet* a, ColliderSet* b, bool was_collision);
        void SendInvalidCollision(ColliderSet* a, ColliderSet* b);

        size_t GenerateKey(ColliderSet* a, ColliderSet* b);
        size_t GenerateKey(ColliderPrimitive* a, ColliderPrimitive* b);
        size_t GenerateKey(RigidContactManifold* manifold);
        auto   FindAssociatedPairs(ColliderSet* a);
        auto   FindCollisionData(ColliderSet* a, ColliderSet* b, size_t at = 0) const;
        auto   FindCollisionState(ColliderSet* a, ColliderSet* b);

        RigidContactManifold* FindManifold(ColliderSet* a, ColliderSet* b);
        RigidContactManifold* CreateManifold(ColliderSet* a, ColliderSet* b);
        bool             HasManifold(ColliderSet* a, ColliderSet* b) const;
        void             FilteringManifolds();
        size_t           RegisterKey(ColliderSet* a, ColliderSet* b);
        void             DeRegisterKey(ColliderSet* a, ColliderSet* b);

    public:
        std::unordered_multimap<size_t, RigidContactManifold>    m_manifold_table;
        std::unordered_multimap<size_t, CollisionStateData> m_state_table;
        std::unordered_multimap<ColliderSet*, ColliderSet*> m_key_table;
    };
}
