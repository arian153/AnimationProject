#include "DynamicBVH.hpp"
#include "../../Core/Utility/CoreUtility.hpp"
#include "..//ColliderPrimitive/ColliderPrimitive.hpp"
#include <list>
#include <vector>
#include <queue>
#include "../../Graphics/Utility/PrimitiveRenderer.hpp"

namespace CS460
{
    DynamicBVHNode::DynamicBVHNode()
    {
        children[0] = nullptr;
        children[1] = nullptr;
    }

    DynamicBVHNode::~DynamicBVHNode()
    {
    }

    bool DynamicBVHNode::IsLeaf() const
    {
        return children[0] == nullptr;
    }

    void DynamicBVHNode::SetBranch(DynamicBVHNode* n0, DynamicBVHNode* n1)
    {
        n0->parent  = this;
        n1->parent  = this;
        children[0] = n0;
        children[1] = n1;
    }

    void DynamicBVHNode::SetLeaf(BoundingAABB* bounding_aabb)
    {
        // create two-way link
        this->data                = bounding_aabb;
        bounding_aabb->m_userdata = this;
        children[0]               = nullptr;
        children[1]               = nullptr;
    }

    void DynamicBVHNode::UpdateAABB(Real margin)
    {
        if (this->IsLeaf() == true)
        {
            // make fat AABB
            Vector3 margin_vector(margin, margin, margin);
            aabb.m_min = data->m_min - margin_vector;
            aabb.m_max = data->m_max + margin_vector;
        }
        else
        {
            // make union of child AABBs of child nodes
            aabb = children[0]->aabb.Union(children[1]->aabb);
        }
    }

    DynamicBVHNode* DynamicBVHNode::GetSibling() const
    {
        return this == parent->children[0] ? parent->children[1] : parent->children[0];
    }

    DynamicBVH::DynamicBVH()
    {
    }

    DynamicBVH::~DynamicBVH()
    {
    }

    void DynamicBVH::Initialize()
    {
        m_drawing_box.SetUnit();
    }

    void DynamicBVH::Update(Real dt)
    {
        E5_UNUSED_PARAM(dt);
        if (m_root)
        {
            if (m_root->IsLeaf())
            {
                if (m_root->data != nullptr)
                {
                    if (m_root->data->GetCollider() != nullptr)
                    {
                        m_root->data->GetCollider()->UpdateBoundingVolume();
                    }
                }
                m_root->UpdateAABB(Physics::Collision::BROAD_PHASE_MARGIN);
            }
            else
            {
                // grab all invalid nodes
                m_invalid_nodes.clear();
                UpdateNodeRecursive(m_root, m_invalid_nodes);
                // re-insert all invalid nodes
                for (DynamicBVHNode* node : m_invalid_nodes)
                {
                    // grab parent link
                    // (pointer to the pointer that points to parent)
                    DynamicBVHNode*  parent      = node->parent;
                    DynamicBVHNode*  sibling     = node->GetSibling();
                    DynamicBVHNode** parent_link = parent->parent
                                                       ? (parent == parent->parent->children[0]
                                                              ? &parent->parent->children[0]
                                                              : &parent->parent->children[1])
                                                       : &m_root;
                    // replace parent with sibling
                    // root has null parent
                    sibling->parent = parent->parent ? parent->parent : nullptr;
                    *parent_link    = sibling;
                    delete parent;
                    // re-insert node
                    node->UpdateAABB(Physics::Collision::BROAD_PHASE_MARGIN);
                    InsertNodeRecursive(node, &m_root);
                }
                m_invalid_nodes.clear();
            }
        }
    }

    void DynamicBVH::Shutdown()
    {
        ShutdownNodeRecursive(m_root);
    }

    void DynamicBVH::Copy(BroadPhase* other)
    {
        CopyNodeRecursive(m_root, other);
    }

    void DynamicBVH::Add(BoundingAABB* aabb)
    {
        if (m_root != nullptr)
        {
            // not first node, insert node to tree
            DynamicBVHNode* node = new DynamicBVHNode();
            node->SetLeaf(aabb);
            node->UpdateAABB(Physics::Collision::BROAD_PHASE_MARGIN);
            InsertNodeRecursive(node, &m_root);
        }
        else
        {
            // first node, make root
            m_root = new DynamicBVHNode();
            m_root->SetLeaf(aabb);
            m_root->UpdateAABB(Physics::Collision::BROAD_PHASE_MARGIN);
        }
    }

    void DynamicBVH::Remove(BoundingAABB* aabb)
    {
        DynamicBVHNode* node = static_cast<DynamicBVHNode*>(aabb->m_userdata);
        // remove two-way link
        node->data       = nullptr;
        aabb->m_userdata = nullptr;
        RemoveNodeRecursive(node);
    }

    void DynamicBVH::Clear()
    {
        ClearNodeRecursive(m_root);
    }

    void DynamicBVH::Release()
    {
        ReleaseNodeRecursive(m_root);
    }

    void DynamicBVH::Render(PrimitiveRenderer* primitive_renderer, const ColorFlag& broad_phase_color, const ColorFlag& primitive_color)
    {
        RenderNodeRecursive(m_root, primitive_renderer, broad_phase_color, primitive_color);
    }

    void DynamicBVH::ComputePairs(std::list<PotentialPair>& result)
    {
        result.clear();
        if (m_root == nullptr || m_root->IsLeaf())
            return;
        // clear Node::childrenCrossed flags
        ClearChildrenCrossFlagRecursive(m_root);
        // base recursive call
        ComputePairsRecursive(m_root->children[0], m_root->children[1], result);
    }

    ColliderPrimitive* DynamicBVH::Pick(const Vector3& point) const
    {
        std::queue<DynamicBVHNode*> queue;
        if (m_root != nullptr)
        {
            queue.push(m_root);
        }
        while (queue.empty() == false)
        {
            DynamicBVHNode& node = *queue.front();
            queue.pop();
            if (node.IsLeaf())
            {
                if (node.data->Contains(point) == true)
                {
                    return node.data->GetCollider();
                }
            }
            else
            {
                queue.push(node.children[0]);
                queue.push(node.children[1]);
            }
        }
        return nullptr;
    }

    void DynamicBVH::Query(const BoundingAABB& aabb, std::vector<ColliderPrimitive*>& output) const
    {
        std::queue<DynamicBVHNode*> queue;
        if (m_root != nullptr)
        {
            queue.push(m_root);
        }
        while (queue.empty() == false)
        {
            DynamicBVHNode& node = *queue.front();
            queue.pop();
            if (node.IsLeaf())
            {
                if (node.data->Intersect(aabb) == true)
                {
                    output.push_back(node.data->GetCollider());
                }
            }
            else
            {
                queue.push(node.children[0]);
                queue.push(node.children[1]);
            }
        }
    }

    void DynamicBVH::CastRay(RayCastResult& result, Real max_distance) const
    {
        std::queue<DynamicBVHNode*> queue;
        if (m_root != nullptr)
        {
            queue.push(m_root);
        }
        Ray ray = result.ray;
        while (!queue.empty())
        {
            DynamicBVHNode& node = *queue.front();
            queue.pop();
            BoundingAABB& collider_aabb = *node.data;
            BoundingAABB& aabb          = node.IsLeaf() ? collider_aabb : node.aabb;
            Real          aabb_t        = Math::REAL_POSITIVE_MAX;
            if (aabb.TestRayIntersection(ray, aabb_t, max_distance) == true)
            {
                // the node cannot possibly give closer results, skip
                if (result.hit_data.hit && result.hit_data.t < aabb_t)
                    continue;
                if (node.IsLeaf() == true)
                {
                    ColliderPrimitive* collider = collider_aabb.GetCollider();
                    RayCastResult      ray_cast_result(ray);
                    collider->CastRay(ray_cast_result, max_distance);
                    if (ray_cast_result.hit_data.hit == true)
                    {
                        if (result.hit_data.hit == true) // compare hit
                        {
                            if (ray_cast_result.hit_data.t < result.hit_data.t)
                            {
                                result = ray_cast_result;
                            }
                        }
                        else // first hit
                        {
                            result = ray_cast_result;
                        }
                    }
                }
                else // is branch
                {
                    queue.push(node.children[0]);
                    queue.push(node.children[1]);
                }
            }
        }
    }

    void DynamicBVH::IntersectRay(RayIntersectionResult& result, Real max_distance) const
    {
        std::queue<DynamicBVHNode*> queue;
        if (m_root != nullptr)
        {
            queue.push(m_root);
        }
        Ray ray = result.ray;
        while (!queue.empty())
        {
            DynamicBVHNode& node = *queue.front();
            queue.pop();
            BoundingAABB& collider_aabb = *node.data;
            BoundingAABB& aabb          = node.IsLeaf() ? collider_aabb : node.aabb;
            Real          aabb_t        = 0.0f;
            if (aabb.TestRayIntersection(ray, aabb_t, max_distance) == true)
            {
                if (node.IsLeaf() == true)
                {
                    ColliderPrimitive* collider = collider_aabb.GetCollider();
                    collider->IntersectRay(result, max_distance);
                }
                else // is branch
                {
                    queue.push(node.children[0]);
                    queue.push(node.children[1]);
                }
            }
        }
    }

    void DynamicBVH::TraceRay(RayTraceResult& result, Real max_distance, size_t reflect_count) const
    {
        std::queue<DynamicBVHNode*> queue;
        if (m_root == nullptr)
        {
            return;
        }
        result.hit_list.reserve(reflect_count);
        result.ray_list.reserve(reflect_count);
        Ray prev_ray = result.initial_ray;
        for (size_t i = 0; i < reflect_count; ++i)
        {
            Ray ray = result.ray_list.at(i);
            if (i > 0 && prev_ray == ray)
            {
                break;
            }
            HitData hit_data;
            hit_data.t = Math::REAL_POSITIVE_MAX;
            queue.push(m_root);
            while (!queue.empty())
            {
                DynamicBVHNode& node = *queue.front();
                queue.pop();
                BoundingAABB& collider_aabb = *node.data;
                BoundingAABB& aabb          = node.IsLeaf() ? collider_aabb : node.aabb;
                Real          aabb_t        = Math::REAL_POSITIVE_MAX;
                if (aabb.TestRayIntersection(ray, aabb_t, max_distance) == true)
                {
                    // the node cannot possibly give closer results, skip
                    if (hit_data.hit && hit_data.t < aabb_t)
                        continue;
                    if (node.IsLeaf() == true)
                    {
                        ColliderPrimitive* collider = collider_aabb.GetCollider();
                        RayCastResult      ray_cast_result(ray);
                        collider->CastRay(ray_cast_result, max_distance);
                        if (ray_cast_result.hit_data.hit == true)
                        {
                            if (hit_data.hit == true) // compare hit
                            {
                                if (ray_cast_result.hit_data.t < hit_data.t)
                                {
                                    hit_data = ray_cast_result.hit_data;
                                }
                            }
                            else // first hit
                            {
                                hit_data = ray_cast_result.hit_data;
                            }
                        }
                    }
                    else // is branch
                    {
                        queue.push(node.children[0]);
                        queue.push(node.children[1]);
                    }
                }
            }
            if (hit_data.hit == true)
            {
                if (!result.hit_list.empty())
                {
                    auto prev_data = result.hit_list.back();
                    if (prev_data.intersection == hit_data.intersection)
                    {
                        break;
                    }
                }
                Ray reflected = ray.GetReflectedRay(hit_data.normal, hit_data.intersection);
                if (reflected.direction == ray.direction)
                {
                    break;
                }
                result.hit_list.push_back(hit_data);
                result.ray_list.push_back(reflected);
                prev_ray = ray;
            }
            else
            {
                break;
            }
        }
    }

    void DynamicBVH::UpdateNodeRecursive(DynamicBVHNode* node, std::vector<DynamicBVHNode*>& invalid_nodes)
    {
        if (node->IsLeaf())
        {
            if (node->data != nullptr)
            {
                if (node->data->GetCollider() != nullptr)
                {
                    node->data->GetCollider()->UpdateBoundingVolume();
                }
            }
            // check if fat AABB doesn't contain the collider AABB anymore
            if (node->aabb.Contains(node->data) == false)
            {
                invalid_nodes.push_back(node);
            }
        }
        else
        {
            UpdateNodeRecursive(node->children[0], invalid_nodes);
            UpdateNodeRecursive(node->children[1], invalid_nodes);
        }
    }

    void DynamicBVH::InsertNodeRecursive(DynamicBVHNode* node, DynamicBVHNode** parent) const
    {
        DynamicBVHNode* p = *parent;
        if (p->IsLeaf() == true)
        {
            // parent is leaf, simply split
            DynamicBVHNode* new_parent = new DynamicBVHNode();
            new_parent->parent         = p->parent;
            new_parent->SetBranch(node, p);
            *parent = new_parent;
        }
        else
        {
            // parent is branch, compute volume differences between pre-insert and post-insert
            BoundingAABB* aabb0        = &p->children[0]->aabb;
            BoundingAABB* aabb1        = &p->children[1]->aabb;
            Real          volume_diff0 = aabb0->Union(node->aabb).Volume() - aabb0->Volume();
            Real          volume_diff1 = aabb1->Union(node->aabb).Volume() - aabb1->Volume();
            // insert to the child that gives less volume increase
            if (volume_diff0 < volume_diff1)
            {
                InsertNodeRecursive(node, &p->children[0]);
            }
            else
            {
                InsertNodeRecursive(node, &p->children[1]);
            }
        }
        // update parent AABB (propagates back up the recursion stack)
        (*parent)->UpdateAABB(Physics::Collision::BROAD_PHASE_MARGIN);
    }

    void DynamicBVH::RemoveNodeRecursive(DynamicBVHNode* node)
    {
        // replace parent with sibling, remove parent node
        DynamicBVHNode* parent = node->parent;
        if (parent) // node is not root
        {
            if (parent->parent) // if there's a grandparent
            {
                // update links
                DynamicBVHNode* sibling = node->GetSibling();
                sibling->parent         = parent->parent;
                (parent == parent->parent->children[0]
                     ? parent->parent->children[0]
                     : parent->parent->children[1]) = sibling;
            }
            else // no grandparent
            {
                // make sibling root
                DynamicBVHNode* sibling = node->GetSibling();
                m_root                  = sibling;
                sibling->parent         = nullptr;
            }
            delete node;
            delete parent;
        }
        else // node is root
        {
            m_root = nullptr;
            delete node;
        }
    }

    void DynamicBVH::ClearNodeRecursive(DynamicBVHNode* node)
    {
        if (node != nullptr)
        {
            if (node->children[0] != nullptr)
            {
                ClearNodeRecursive(node->children[0]);
            }
            if (node->children[1] != nullptr)
            {
                ClearNodeRecursive(node->children[1]);
            }
            delete node;
            node = nullptr;
        }
    }

    void DynamicBVH::ReleaseNodeRecursive(DynamicBVHNode* node)
    {
        if (node != nullptr)
        {
            if (node->children[0] != nullptr)
            {
                ReleaseNodeRecursive(node->children[0]);
            }
            if (node->children[1] != nullptr)
            {
                ReleaseNodeRecursive(node->children[1]);
            }
            if (node->data != nullptr)
            {
                node->data->m_userdata = nullptr;
                delete node->data;
                node->data = nullptr;
            }
        }
    }

    void DynamicBVH::ShutdownNodeRecursive(DynamicBVHNode* node)
    {
        if (node != nullptr)
        {
            if (node->children[0] != nullptr)
            {
                ShutdownNodeRecursive(node->children[0]);
            }
            if (node->children[1] != nullptr)
            {
                ShutdownNodeRecursive(node->children[1]);
            }
            if (node->data != nullptr)
            {
                node->data->m_userdata = nullptr;
                delete node->data;
                node->data = nullptr;
            }
            delete node;
            node = nullptr;
        }
    }

    void DynamicBVH::CopyNodeRecursive(DynamicBVHNode* node, BroadPhase* other)
    {
        if (node != nullptr)
        {
            if (node->children[0] != nullptr)
            {
                CopyNodeRecursive(node->children[0], other);
            }
            if (node->children[1] != nullptr)
            {
                CopyNodeRecursive(node->children[1], other);
            }
            if (node->data != nullptr)
            {
                other->Add(node->data);
            }
        }
    }

    void DynamicBVH::RenderNodeRecursive(DynamicBVHNode* node, PrimitiveRenderer* primitive_renderer, const ColorFlag& broad_phase_color, const ColorFlag& primitive_color) const
    {
        if (node != nullptr)
        {
            if (node->IsLeaf() == true)
            {
                if (node->data != nullptr)
                {
                    if (node->data->GetCollider() != nullptr && primitive_color.b_flag)
                    {
                        if (node->data->GetCollider()->GetRigidBody()->IsSleep())
                        {
                            node->data->GetCollider()->Draw(primitive_renderer, eRenderingMode::Line, Color(0.23f, 0.23f, 0.23f, 1.0f));
                        }
                        else
                        {
                            node->data->GetCollider()->Draw(primitive_renderer, eRenderingMode::Line, primitive_color.color);
                        }
                    }
                }
            }
            if (broad_phase_color.b_flag)
            {
                primitive_renderer->DrawPrimitiveInstancing(m_drawing_box, node->aabb.Center(), node->aabb.Size(), eRenderingMode::Line, broad_phase_color.color);
                //primitive_renderer->DrawBox(node->aabb.Center(), Quaternion(), node->aabb.Size(), eRenderingMode::Line, broad_phase_color.color);
            }
            if (node->children[0] != nullptr)
            {
                RenderNodeRecursive(node->children[0], primitive_renderer, broad_phase_color, primitive_color);
            }
            if (node->children[1] != nullptr)
            {
                RenderNodeRecursive(node->children[1], primitive_renderer, broad_phase_color, primitive_color);
            }
        }
    }

    void DynamicBVH::ComputePairsRecursive(DynamicBVHNode* n0, DynamicBVHNode* n1, std::list<PotentialPair>& result)
    {
        if (n0->IsLeaf() == true)
        {
            // 2 leaves, check proxies instead of fat AABBs
            if (n1->IsLeaf() == true)
            {
                if (n0->data->Intersect(n1->data) == true)
                {
                    result.emplace_back(n0->data->GetCollider(), n1->data->GetCollider());
                }
            }
            else // 1 branch / 1 leaf, 2 cross checks
            {
                CrossChildren(n1, result);
                ComputePairsRecursive(n0, n1->children[0], result);
                ComputePairsRecursive(n0, n1->children[1], result);
            }
        }
        else
        {
            // 1 branch / 1 leaf, 2 cross checks
            if (n1->IsLeaf() == true)
            {
                CrossChildren(n0, result);
                ComputePairsRecursive(n0->children[0], n1, result);
                ComputePairsRecursive(n0->children[1], n1, result);
            }
            else // 2 branches, 4 cross checks
            {
                CrossChildren(n0, result);
                CrossChildren(n1, result);
                ComputePairsRecursive(n0->children[0], n1->children[0], result);
                ComputePairsRecursive(n0->children[0], n1->children[1], result);
                ComputePairsRecursive(n0->children[1], n1->children[0], result);
                ComputePairsRecursive(n0->children[1], n1->children[1], result);
            }
        } // end of if (n0->IsLeaf())
    }

    void DynamicBVH::ClearChildrenCrossFlagRecursive(DynamicBVHNode* node)
    {
        node->children_crossed = false;
        if (node->IsLeaf() == false)
        {
            ClearChildrenCrossFlagRecursive(node->children[0]);
            ClearChildrenCrossFlagRecursive(node->children[1]);
        }
    }

    void DynamicBVH::CrossChildren(DynamicBVHNode* node, std::list<PotentialPair>& result)
    {
        if (node->children_crossed == false)
        {
            ComputePairsRecursive(node->children[0], node->children[1], result);
            node->children_crossed = true;
        }
    }
}
