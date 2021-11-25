#pragma once
#include <vector>

#include "../Math/Primitive/Others/Ray.hpp"
#include "../Math/Structure/Transform.hpp"
#include "../Math/Utility/MathDef.hpp"

namespace CS460
{
    class ObjectManager;
    class PrimitiveRenderer;
    class TextRenderer;
    class InputCommon;
    class LogicComponent;

    class LogicSubsystem
    {
    public:
        LogicSubsystem();
        ~LogicSubsystem();

        void Initialize();
        void Update(Real dt);
        void Render() const;
        void Shutdown();

        void AddLogic(LogicComponent* logic);
        void RemoveLogic(LogicComponent* logic);
        void InitializeLogic(LogicComponent* logic) const;
        void SetPrimitiveRenderer(PrimitiveRenderer* primitive_renderer);

        void SetPickingRay(const Ray& ray);
        void SetMouseOrtho(const Vector2& pos);

        void GetLogicComponents(const std::string& type, std::vector<LogicComponent*>& container);

        InputCommon* GetInput() const;
    private:
        friend class LogicSystem;
    private:
        std::vector<LogicComponent*> m_logic_components;
        std::vector<Transform*> m_transforms;

        //
        InputCommon*       m_input              = nullptr;
        TextRenderer*      m_text_renderer      = nullptr;
        PrimitiveRenderer* m_primitive_renderer = nullptr;



        Vector2 m_mouse_ortho;
        Ray     m_picking_ray;
    };
}
