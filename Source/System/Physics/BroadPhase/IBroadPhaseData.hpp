#pragma once

namespace CS460
{
    class BoundingAABB;
    class ColliderPrimitive;
    class ColliderSet;

    enum class eBPDType
    {
        Collider,
        SoftBody,
        Invalid
    };

    class IBroadPhaseData
    {
    public:
        IBroadPhaseData() = default;
        virtual ~IBroadPhaseData();

        virtual void SetBoundingVolume(BoundingAABB* volume) = 0;
        virtual bool SkipIntersectionTest(BoundingAABB* other) = 0;

    public:
        eBPDType bpd_type = eBPDType::Invalid;
    protected:
        BoundingAABB* bounding_volume = nullptr;
    };

    class BPDCollider final : public IBroadPhaseData
    {
    public:
        BPDCollider();
        explicit BPDCollider(ColliderPrimitive* collider);
        BPDCollider(ColliderPrimitive* collider, ColliderSet* set);
        ~BPDCollider();

        void SetBoundingVolume(BoundingAABB* volume) override;
        bool SkipIntersectionTest(BoundingAABB* other) override;

    public:
        ColliderPrimitive* m_collider     = nullptr;
        ColliderSet*       m_collider_set = nullptr;
    private:
    };
}
