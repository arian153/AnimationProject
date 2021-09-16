#pragma once
#include "../../Math/Utility/MathDef.hpp"

namespace CS460
{
    enum class eAINodeState
    {
        Failure
      , Success
      , Persist
      , None
    };

    class AINode
    {
    public:
        AINode()
        {
        }

        virtual ~AINode()
        {
        }

        virtual void OnStart()
        {
        }

        virtual void OnEnd()
        {
        }

        virtual eAINodeState OnUpdate(Real dt) = 0;

        eAINodeState Invoke(Real dt)
        {
            if (m_state != eAINodeState::Persist)
            {
                OnStart();
            }
            m_state = OnUpdate(dt);
            if (m_state != eAINodeState::Persist)
            {
                OnEnd();
            }
            return m_state;
        }

        bool IsSuccess() const
        {
            return m_state == eAINodeState::Success;
        }

        bool IsFailure() const
        {
            return m_state == eAINodeState::Failure;
        }

        bool IsPersist() const
        {
            return m_state == eAINodeState::Persist;
        }

        bool IsFinished() const
        {
            return m_state == eAINodeState::Success || m_state == eAINodeState::Failure;
        }

        void Reset()
        {
            m_state = eAINodeState::None;
        }

    protected:
        eAINodeState m_state  = eAINodeState::None;
    };
}
