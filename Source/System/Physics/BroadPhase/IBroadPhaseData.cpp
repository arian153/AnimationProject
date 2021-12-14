#include "IBroadPhaseData.hpp"

#include "../ColliderPrimitive/ColliderPrimitive.hpp"

namespace CS460
{
    IBroadPhaseData::~IBroadPhaseData()
    {
    }

    BPDCollider::BPDCollider()
    {
        bpd_type = eBPDType::Collider;
    }

    BPDCollider::BPDCollider(ColliderPrimitive* collider)
        : m_collider(collider)
    {
        bpd_type       = eBPDType::Collider;
        m_collider_set = collider->GetColliderSet();
    }

    BPDCollider::BPDCollider(ColliderPrimitive* collider, ColliderSet* set)
        : m_collider(collider), m_collider_set(set)
    {
        bpd_type = eBPDType::Collider;
    }

    BPDCollider::~BPDCollider()
    {
    }

    void BPDCollider::SetBoundingVolume(BoundingAABB* volume)
    {
        bounding_volume = volume;
        if (m_collider != nullptr)
        {
            m_collider->m_bounding_volume = volume;
        }
    }

    bool BPDCollider::SkipIntersectionTest(BoundingAABB* other)
    {
        auto other_set = other->GetColliderSet();
        return m_collider_set != nullptr && other_set != nullptr && m_collider_set == other_set;
    }
}
