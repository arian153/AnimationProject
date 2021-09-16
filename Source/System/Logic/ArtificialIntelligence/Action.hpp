#pragma once
#include "../../Math/Utility/MathDef.hpp"
#include "AINode.hpp"
#include "BlackBoard.hpp"

namespace CS460
{
    class Action : public AINode
    {
    public:
        Action();
        virtual ~Action();

        void SetBlackBoard(BlackBoard* black_board);

        void OnStart() override = 0;
        void OnEnd() override = 0;

        eAINodeState OnUpdate(Real dt) override = 0;

    protected:
        bool        m_b_blocking   = false;
        size_t      m_lanes        = 0;
        Real        m_elapsed_time = 0.0f;
        Real        m_duration     = 1.0f;
        BlackBoard* m_black_board  = nullptr;
    };
}
