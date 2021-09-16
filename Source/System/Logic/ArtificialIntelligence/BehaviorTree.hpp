#pragma once
#include "../../Math/Utility/MathDef.hpp"
#include "AINode.hpp"
#include "BlackBoard.hpp"

namespace CS460
{
    class SequenceNode;
    class BlackBoard;

    class RootNode final : public AINode
    {
    public:
        RootNode();
        ~RootNode();

        eAINodeState OnUpdate(Real dt) override;

    private:
        AINode* m_child = nullptr;
    };

    class BehaviorTree
    {
    public:
        explicit BehaviorTree(BlackBoard& global_board);
        ~BehaviorTree();

        void Initialize();
        void Update(Real dt);
        void Shutdown();

    private:
        eAINodeState m_state    = eAINodeState::None;
        RootNode*    m_root     = nullptr;
        BlackBoard   m_black_board;
        BlackBoard&  m_global_board;
    };
}
