#pragma once
#include "../../Math/Utility/MathDef.hpp"
#include "BlackBoard.hpp"

namespace CS460
{
    class Action;
    class BlackBoard;

    class ActionList
    {
    public:
        explicit ActionList(BlackBoard& global_board);
        ~ActionList();

        void Initialize();
        void Update(Real dt);
        void Shutdown();

        void PushFront(Action* action);
        void PushBack(Action* action);
        void PopFront();
        void PopBack();

    private:
        BlackBoard  m_black_board;
        BlackBoard& m_global_board;

        std::vector<Action*> m_actions;
    };
}
