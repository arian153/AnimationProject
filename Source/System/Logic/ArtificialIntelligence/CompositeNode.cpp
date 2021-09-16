#include "CompositeNode.hpp"

namespace CS460
{
    CompositeNode::CompositeNode()
    {
    }

    CompositeNode::~CompositeNode()
    {
    }

    void CompositeNode::AddChild(AINode* child)
    {
        m_children.push_back(child);
    }

    bool CompositeNode::IsEmpty() const
    {
        return m_children.empty();
    }

    SelectorNode::SelectorNode()
    {
    }

    SelectorNode::~SelectorNode()
    {
        for (auto& child : m_children)
        {
            delete child;
            child = nullptr;
        }
        m_children.clear();
    }

    void SelectorNode::OnStart()
    {
        m_iterator = m_children.begin();
    }

    eAINodeState SelectorNode::OnUpdate(Real dt)
    {
        if (IsEmpty())
        {
            return eAINodeState::None;
        }
        for (; m_iterator != m_children.end(); ++m_iterator)
        {
            eAINodeState state = (*m_iterator)->Invoke(dt);
            if (state != eAINodeState::Failure)
            {
                return state;
            }
        }
        return eAINodeState::Failure;
    }

    SequenceNode::SequenceNode()
    {
    }

    SequenceNode::~SequenceNode()
    {
        for (auto& child : m_children)
        {
            delete child;
            child = nullptr;
        }
        m_children.clear();
    }

    void SequenceNode::OnStart()
    {
        m_iterator = m_children.begin();
    }

    eAINodeState SequenceNode::OnUpdate(Real dt)
    {
        if (IsEmpty())
        {
            return eAINodeState::None;
        }
        for (; m_iterator != m_children.end(); ++m_iterator)
        {
            eAINodeState state = (*m_iterator)->Invoke(dt);
            if (state != eAINodeState::Success)
            {
                return state;
            }
        }
        return eAINodeState::Success;
    }
}
