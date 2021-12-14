#include "Octree.hpp"
#include "../ColliderPrimitive/ColliderPrimitive.hpp"
#include <queue>

namespace CS460
{
    OctreeNode::OctreeNode()
    {
        children[0] = nullptr;
        children[1] = nullptr;
        children[2] = nullptr;
        children[3] = nullptr;
        children[4] = nullptr;
        children[5] = nullptr;
        children[6] = nullptr;
        children[7] = nullptr;
    }

    OctreeNode::~OctreeNode()
    {
    }

    bool OctreeNode::IsLeaf() const
    {
        return children[0] == nullptr;
    }

    OctreeNode* OctreeNode::GetChild(const Vector3& pos) const
    {
        if (IsLeaf() == false)
        {
            if (pos.x > origin.x)
            {
                if (pos.y > origin.y)
                {
                    if (pos.z > origin.z)
                    {
                        return children[0];
                    }
                    return children[1];
                }
                if (pos.z > origin.z)
                {
                    return children[2];
                }
                return children[3];
            }
            if (pos.y > origin.y)
            {
                if (pos.z > origin.z)
                {
                    return children[4];
                }
                return children[5];
            }
            if (pos.z > origin.z)
            {
                return children[6];
            }
            return children[7];
        }
        return nullptr;
    }

    void OctreeNode::SetChild(OctreeNode* child, size_t index)
    {
        children[index] = child;
        child->parent   = this;
        child->depth    = depth + 1;
        child->origin   = origin + ScaleSector(index);
        child->scale    = scale.Half();
    }

    Vector3 OctreeNode::ScaleSector(size_t index) const
    {
        Vector3 result = scale.Half();
        switch (index)
        {
        case 0: //+++
            break;
        case 1: //++-
            result.z = -result.z;
            break;
        case 2: //+-+
            result.y = -result.y;
            break;
        case 3: //+--
            result.y = -result.y;
            result.z = -result.z;
            break;
        case 4: //-++
            result.x = -result.x;
            break;
        case 5: //-+-
            result.x = -result.x;
            result.z = -result.z;
            break;
        case 6: //--+
            result.x = -result.x;
            result.y = -result.y;
            break;
        case 7: //---
            result.x = -result.x;
            result.y = -result.y;
            result.z = -result.z;
            break;
        default:
            break;
        }
        return result;
    }

    bool OctreeNode::Contain(const Vector3& pos) const
    {
        if (parent != nullptr)
        {
            if (parent->GetChild(pos) != this)
            {
                return false;
            }
        }
        return true;
    }

    Octree::Octree()
    {
    }

    Octree::~Octree()
    {
    }

    void Octree::Initialize()
    {
    }

    void Octree::Update(Real dt)
    {
        if (m_root)
        {
            if (m_root->IsLeaf() == false)
            {
                m_invalid_aabbs.clear();
                UpdateNode(m_root);
                for (BoundingAABB* aabb : m_invalid_aabbs)
                {
                    Add(aabb);
                }
            }
        }
    }

    void Octree::Shutdown()
    {
        ShutdownNode(m_root);
    }

    void Octree::Copy(BroadPhase* other)
    {
        if (m_root != nullptr)
        {
            CopyNode(m_root, other);
        }
    }

    void Octree::Add(BoundingAABB* aabb)
    {
        if (m_root != nullptr)
        {
            InsertAABB(m_root, aabb);
        }
        else
        {
            m_root = new OctreeNode();
            m_root->aabb_list.push_back(aabb);
            m_root->origin = Vector3();
            m_root->parent = nullptr;
            m_root->depth  = 0;
            m_root->scale  = m_root_scale;
        }
    }

    void Octree::Remove(BoundingAABB* aabb)
    {
        RemoveAABB(m_root, aabb);
    }

    void Octree::Clear()
    {
        ClearNode(m_root);
    }

    void Octree::Release()
    {
        ReleaseNode(m_root);
    }

    void Octree::Render(PrimitiveRenderer* primitive_renderer, const ColorFlag& broad_phase_color, const ColorFlag& primitive_color)
    {
    }

    void Octree::ComputePairs(std::list<PotentialPair>& result)
    {
        IntersectNode(m_root, result);
    }

    ColliderPrimitive* Octree::Pick(const Vector3& point) const
    {
        std::queue<OctreeNode*> queue;
        if (m_root != nullptr)
        {
            queue.push(m_root);
        }
        while (queue.empty() == false)
        {
            OctreeNode* node = queue.front();
            queue.pop();
            if (node->IsLeaf())
            {
                for (auto& aabb : node->aabb_list)
                {
                    if (aabb->Contains(point) == true)
                    {
                        return aabb->GetCollider();
                    }
                }
            }
            else
            {
                for (size_t i = 0; i < 8; ++i)
                {
                    queue.push(node->children[i]);
                }
            }
        }
        return nullptr;
    }

    void Octree::Query(const BoundingAABB& aabb, std::vector<ColliderPrimitive*>& output) const
    {
        std::queue<OctreeNode*> queue;
        if (m_root != nullptr)
        {
            queue.push(m_root);
        }
        while (queue.empty() == false)
        {
            OctreeNode* node = queue.front();
            queue.pop();
            if (node->IsLeaf())
            {
                for (auto& data : node->aabb_list)
                {
                    if (data->Intersect(aabb) == true)
                    {
                        output.push_back(data->GetCollider());
                    }
                }
            }
            else
            {
                for (size_t i = 0; i < 8; ++i)
                {
                    queue.push(node->children[i]);
                }
            }
        }
    }

    void Octree::CastRay(RayCastResult& result, Real max_distance) const
    {
    }

    void Octree::IntersectRay(RayIntersectionResult& result, Real max_distance) const
    {
    }

    void Octree::TraceRay(RayTraceResult& result, Real max_distance, size_t reflect_count) const
    {
    }

    void Octree::SetScale(Real x, Real y, Real z)
    {
        m_root_scale.Set(fabsf(x), fabsf(y), fabsf(z));
    }

    void Octree::InsertAABB(OctreeNode* node, BoundingAABB* aabb) const
    {
        Vector3 pos = aabb->Center();
        if (node->IsLeaf())
        {
            size_t size = node->aabb_list.size();
            if (size < m_max_aabb_count)
            {
                node->aabb_list.push_back(aabb);
            }
            else
            {
                CreateChildrenNode(node);
                for (auto prev_aabb : node->aabb_list)
                {
                    auto child = node->GetChild(prev_aabb->Center());
                    child->aabb_list.push_back(prev_aabb);
                }
                node->aabb_list.clear();
                node->GetChild(pos)->aabb_list.push_back(aabb);
            }
        }
        else
        {
            InsertAABB(node->GetChild(pos), aabb);
        }
    }

    void Octree::UpdateNode(OctreeNode* node)
    {
        if (node->IsLeaf())
        {
            std::vector<BoundingAABB*> missing_aabbs;
            for (auto& aabb : node->aabb_list)
            {
                if (aabb->GetCollider() != nullptr)
                {
                    aabb->GetCollider()->UpdateBoundingVolume();
                }
                if (node->Contain(aabb->Center()) == false)
                {
                    missing_aabbs.push_back(aabb);
                }
            }
            for (auto& aabb : missing_aabbs)
            {
                m_invalid_aabbs.push_back(aabb);
                auto begin = node->aabb_list.begin();
                auto end   = node->aabb_list.end();
                for (auto it = begin; it != end; ++it)
                {
                    if (*it == aabb)
                    {
                        node->aabb_list.erase(it++);
                        break;
                    }
                }
            }
            missing_aabbs.clear();
        }
        else
        {
            for (size_t i = 0; i < 8; ++i)
            {
                UpdateNode(node->children[i]);
            }
        }
    }

    void Octree::CreateChildrenNode(OctreeNode* node) const
    {
        for (size_t i = 0; i < 8; ++i)
        {
            node->SetChild(new OctreeNode(), i);
        }
    }

    void Octree::ShutdownNode(OctreeNode* node)
    {
        if (node != nullptr)
        {
            for (size_t i = 0; i < 8; ++i)
            {
                if (node->children[i] != nullptr)
                {
                    ShutdownNode(node->children[i]);
                }
            }
            for (auto& aabb : node->aabb_list)
            {
                if (aabb != nullptr)
                {
                    delete aabb;
                    aabb = nullptr;
                }
            }
            node->aabb_list.clear();
            delete node;
            node = nullptr;
        }
    }

    void Octree::ClearNode(OctreeNode* node)
    {
        if (node != nullptr)
        {
            for (size_t i = 0; i < 8; ++i)
            {
                if (node->children[i] != nullptr)
                {
                    ShutdownNode(node->children[i]);
                }
            }
            node->aabb_list.clear();
            delete node;
            node = nullptr;
        }
    }

    void Octree::IntersectNode(OctreeNode* node, std::list<PotentialPair>& result)
    {
        if (node->IsLeaf())
        {
            size_t aabb_size = node->aabb_list.size();
            for (size_t i = 0; i < aabb_size; ++i)
            {
                BoundingAABB* aabb = node->aabb_list[i];
                // Update with the residing cell
                IntersectAABB(aabb, node, i + 1, result);
                if (node->parent != nullptr)
                {
                    for (OctreeNode* sibling : node->parent->children)
                    {
                        if (sibling != node)
                        {
                            //need to check big aabb
                            IntersectAABB(aabb, sibling, 0, result);
                        }
                    }
                }
            }
        }
        else
        {
            for (size_t i = 0; i < 8; ++i)
            {
                IntersectNode(node->children[i], result);
            }
        }
    }

    void Octree::IntersectAABB(BoundingAABB* aabb, OctreeNode* node, size_t index, std::list<PotentialPair>& result)
    {
        size_t size = node->aabb_list.size();
        for (size_t i = index; i < size; i++)
        {
            ColliderPrimitive* collider_a = aabb->GetCollider();
            ColliderPrimitive* collider_b = node->aabb_list[i]->GetCollider();
            if (aabb->Intersect(node->aabb_list[i]))
            {
                result.emplace_back(collider_a, collider_b);
            }
        }
    }

    bool Octree::RemoveAABB(OctreeNode* node, BoundingAABB* aabb)
    {
        if (node->IsLeaf() == true)
        {
            auto end = node->aabb_list.end();
            for (auto it = node->aabb_list.begin(); it != end; ++it)
            {
                if (*it == aabb)
                {
                    node->aabb_list.erase(it++);
                    return true;
                }
            }
        }
        else
        {
            for (auto child : node->children)
            {
                if (RemoveAABB(child, aabb) == true)
                    return true;
            }
        }
        return false;
    }

    void Octree::CopyNode(OctreeNode* node, BroadPhase* other) const
    {
        if (node != nullptr)
        {
            if (node->IsLeaf())
            {
                for (auto& aabb : node->aabb_list)
                {
                    other->Add(aabb);
                }
            }
            else
            {
                for (size_t i = 0; i < 8; ++i)
                {
                    CopyNode(node->children[i], other);
                }
            }
        }
    }

    void Octree::ReleaseNode(OctreeNode* node)
    {
        if (node != nullptr)
        {
            for (size_t i = 0; i < 8; ++i)
            {
                if (node->children[i] != nullptr)
                {
                    ShutdownNode(node->children[i]);
                }
            }
            for (auto& aabb : node->aabb_list)
            {
                if (aabb != nullptr)
                {
                    delete aabb;
                    aabb = nullptr;
                }
            }
        }
    }
}
