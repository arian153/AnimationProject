#include "DecoratorNode.hpp"

namespace CS460
{
    DecoratorNode::DecoratorNode()
    {
    }

    DecoratorNode::~DecoratorNode()
    {
    }

    void DecoratorNode::AddChild(AINode* child)
    {
        m_child = child;
    }

    bool DecoratorNode::IsEmpty() const
    {
        return m_child == nullptr;
    }

    RepeatNode::RepeatNode()
    {
    }

    RepeatNode::~RepeatNode()
    {
        if (m_child != nullptr)
        {
            delete m_child;
            m_child = nullptr;
        }
    }

    void RepeatNode::OnStart()
    {
        m_count = 0;
    }

    eAINodeState RepeatNode::OnUpdate(Real dt)
    {
        if (IsEmpty())
        {
            return eAINodeState::None;
        }
        m_child->Invoke(dt);
        if (m_end > 0 && ++m_count == m_end)
        {
            return eAINodeState::Success;
        }
        return eAINodeState::Persist;
    }

    SucceedNode::SucceedNode()
    {
    }

    SucceedNode::~SucceedNode()
    {
        if (m_child != nullptr)
        {
            delete m_child;
            m_child = nullptr;
        }
    }

    eAINodeState SucceedNode::OnUpdate(Real dt)
    {
        if (IsEmpty())
        {
            return eAINodeState::None;
        }
        m_child->Invoke(dt);
        return eAINodeState::Success;
    }

    FailNode::FailNode()
    {
    }

    FailNode::~FailNode()
    {
        if (m_child != nullptr)
        {
            delete m_child;
            m_child = nullptr;
        }
    }

    eAINodeState FailNode::OnUpdate(Real dt)
    {
        if (IsEmpty())
        {
            return eAINodeState::None;
        }
        m_child->Invoke(dt);
        return eAINodeState::Failure;
    }

    InvertNode::InvertNode()
    {
    }

    InvertNode::~InvertNode()
    {
        if (m_child != nullptr)
        {
            delete m_child;
            m_child = nullptr;
        }
    }

    eAINodeState InvertNode::OnUpdate(Real dt)
    {
        if (IsEmpty())
        {
            return eAINodeState::None;
        }
        eAINodeState state = m_child->Invoke(dt);
        switch (state)
        {
        case eAINodeState::Failure:
            return eAINodeState::Success;
        case eAINodeState::Success:
            return eAINodeState::Failure;
        case eAINodeState::Persist:
            return eAINodeState::Persist;
        case eAINodeState::None:
            return eAINodeState::None;
        default:
            return eAINodeState::None;
        }
    }
}
