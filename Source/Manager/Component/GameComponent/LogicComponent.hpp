#pragma once
#include "..//Component.hpp"

namespace CS460
{
    class PrimitiveRenderer;
    class TextRenderer;
    class InputCommon;

    class LogicComponent : public Component
    {
    public:
        virtual ~LogicComponent();
        LogicComponent()                                     = delete;
        LogicComponent(const LogicComponent& rhs)            = delete;
        LogicComponent& operator=(const LogicComponent& rhs) = delete;

        virtual void Render() = 0;
    protected:
        explicit LogicComponent(Object* owner);

    private:
        friend class LogicSubsystem;

    protected:
        InputCommon*       m_input              = nullptr;
        TextRenderer*      m_text_renderer      = nullptr;
        PrimitiveRenderer* m_primitive_renderer = nullptr;
    };
}
