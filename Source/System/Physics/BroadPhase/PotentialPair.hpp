#pragma once
#include <vector>

namespace CS460
{
    class SoftBody;
    class ColliderPrimitive;

    enum class ePairType
    {
        ColliderPair,
        CompoundPair,
        SoftBodyPair,
        Invalid
    };

    class ColliderPair
    {
    public:
        ColliderPair(ColliderPrimitive* a, ColliderPrimitive* b);
        ~ColliderPair();
    public:
        ColliderPrimitive* collider_a = nullptr;
        ColliderPrimitive* collider_b = nullptr;
    };

    class CompoundPair
    {
    public:
        CompoundPair(ColliderPrimitive* collider, SoftBody* softbody);
        ~CompoundPair();

    public:
        ColliderPrimitive* collider = nullptr;
        SoftBody*          softbody = nullptr;
    };

    class SoftBodyPair
    {
    public:
        SoftBodyPair(SoftBody* a, SoftBody* b);
        ~SoftBodyPair();

    public:
        SoftBody* softbody_a = nullptr;
        SoftBody* softbody_b = nullptr;
    };

    class PotentialPairs
    {
    public:
        PotentialPairs()  = default;
        ~PotentialPairs() = default;

        void emplace_back(ColliderPrimitive* a, ColliderPrimitive* b);
        void emplace_back(ColliderPrimitive* collider, SoftBody* softbody);
        void emplace_back(SoftBody* softbody, ColliderPrimitive* collider);
        void emplace_back(SoftBody* a, SoftBody* b);
        
        void clear();

    public:
        std::vector<ColliderPair> collider_pairs;
        std::vector<CompoundPair> compound_pairs;
        std::vector<SoftBodyPair> softbody_pairs;
    };
}
