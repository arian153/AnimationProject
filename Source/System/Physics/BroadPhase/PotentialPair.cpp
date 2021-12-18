#include "PotentialPair.hpp"

namespace CS460
{
    ColliderPair::ColliderPair(ColliderPrimitive* a, ColliderPrimitive* b)
        : collider_a(a), collider_b(b)
    {
    }

    ColliderPair::~ColliderPair()
    {
    }

    CompoundPair::CompoundPair(ColliderPrimitive* collider, SoftBody* softbody)
        : collider(collider), softbody(softbody)
    {
    }

    CompoundPair::~CompoundPair()
    {
    }

    SoftBodyPair::SoftBodyPair(SoftBody* a, SoftBody* b)
        : softbody_a(a), softbody_b(b)
    {
    }

    SoftBodyPair::~SoftBodyPair()
    {
    }

    void PotentialPairs::emplace_back(ColliderPrimitive* a, ColliderPrimitive* b)
    {
        collider_pairs.emplace_back(a, b);
    }

    void PotentialPairs::emplace_back(ColliderPrimitive* collider, SoftBody* softbody)
    {
        compound_pairs.emplace_back(collider, softbody);
    }

    void PotentialPairs::emplace_back(SoftBody* softbody, ColliderPrimitive* collider)
    {
        compound_pairs.emplace_back(collider, softbody);
    }

    void PotentialPairs::emplace_back(SoftBody* a, SoftBody* b)
    {
        softbody_pairs.emplace_back(a, b);
    }

    void PotentialPairs::clear()
    {
        collider_pairs.clear();
        compound_pairs.clear();
        softbody_pairs.clear();
    }
}
