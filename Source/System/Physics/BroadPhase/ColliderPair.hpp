#pragma once

namespace CS460
{
    class SoftBody;
    class ColliderPrimitive;

    enum class ePairType
    {
        ColliderPair,
        CollSoftPair,
        SoftBodyPair,
        Invalid
    };

    class PotentialPair
    {
    public:
        PotentialPair(ColliderPrimitive* a, ColliderPrimitive* b)
            : collider_a(a), collider_b(b)
        {
            pair_type = ePairType::ColliderPair;
        }

        PotentialPair(ColliderPrimitive* collider, SoftBody* softbody)
            : collider_a(collider), softbody_a(softbody)
        {
            pair_type = ePairType::CollSoftPair;
        }

        PotentialPair(SoftBody* a, SoftBody* b)
            : softbody_a(a), softbody_b(b)
        {
            pair_type = ePairType::SoftBodyPair;
        }

        ~PotentialPair()
        {
        }

    public:
        ePairType          pair_type  = ePairType::Invalid;
        ColliderPrimitive* collider_a = nullptr;
        ColliderPrimitive* collider_b = nullptr;
        SoftBody*          softbody_a = nullptr;
        SoftBody*          softbody_b = nullptr;
    };
}
