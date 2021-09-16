#include "BehaviorTree.hpp"
#include "CompositeNode.hpp"
#include "BlackBoard.hpp"

namespace CS460
{
    RootNode::RootNode()
    {
    }

    RootNode::~RootNode()
    {
        if (m_child != nullptr)
        {
            delete m_child;
            m_child = nullptr;
        }
    }

    eAINodeState RootNode::OnUpdate(Real dt)
    {
        return m_child->Invoke(dt);
    }

    BehaviorTree::BehaviorTree(BlackBoard& global_board)
        : m_global_board(global_board)
    {
    }

    BehaviorTree::~BehaviorTree()
    {
    }

    void BehaviorTree::Initialize()
    {
        m_black_board.Clear();
    }

    void BehaviorTree::Update(Real dt)
    {
        m_state = m_root->Invoke(dt);
    }

    void BehaviorTree::Shutdown()
    {
        if (m_root != nullptr)
        {
            delete m_root;
            m_root = nullptr;
        }
        m_black_board.Clear();
    }
}
