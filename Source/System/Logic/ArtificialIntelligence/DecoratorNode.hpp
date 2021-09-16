#pragma once
#include "AINode.hpp"

namespace CS460
{
    class DecoratorNode : public AINode
    {
    public:
        DecoratorNode();
        virtual ~DecoratorNode();

        void AddChild(AINode* child);
        bool IsEmpty() const;

    protected:
        AINode* m_child = nullptr;
    };

    class RepeatNode final : public DecoratorNode
    {
    public:
        RepeatNode();
        ~RepeatNode();
        void         OnStart() override;
        eAINodeState OnUpdate(Real dt) override;
    private:
        size_t m_count = 0;
        size_t m_end   = 1;
    };

    class SucceedNode final : public DecoratorNode
    {
    public:
        SucceedNode();
        ~SucceedNode();
        eAINodeState OnUpdate(Real dt) override;
    private:
    };

    class FailNode final : public DecoratorNode
    {
    public:
        FailNode();
        ~FailNode();
        eAINodeState OnUpdate(Real dt) override;
    private:
    };

    class InvertNode final : public DecoratorNode
    {
    public:
        InvertNode();
        ~InvertNode();
        eAINodeState OnUpdate(Real dt) override;
    private:
    };
}
