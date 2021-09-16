#include "ActionList.hpp"

namespace CS460
{
    ActionList::ActionList(BlackBoard& global_board)
        : m_global_board(global_board)
    {
    }

    ActionList::~ActionList()
    {
    }

    void ActionList::Initialize()
    {
        m_black_board.Clear();
    }

    void ActionList::Update(Real dt)
    {
        //need to modify
    }

    void ActionList::Shutdown()
    {
        m_black_board.Clear();
    }

    void ActionList::PushFront(Action* action)
    {
        m_actions.push_back(action);
        std::rotate(m_actions.rbegin(), m_actions.rbegin() + 1, m_actions.rend());
    }

    void ActionList::PushBack(Action* action)
    {
        m_actions.push_back(action);
    }

    void ActionList::PopFront()
    {
        if (!m_actions.empty())
            m_actions.erase(m_actions.begin());
    }

    void ActionList::PopBack()
    {
        if (!m_actions.empty())
            m_actions.pop_back();
    }
}
