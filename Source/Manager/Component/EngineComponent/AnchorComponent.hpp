#pragma once

#include "..//Component.hpp"
#include "../../../System/Math/Primitive/ConvexHull3D/Sphere.hpp"
#include "../../../System/Math/Primitive/Others/Plane.hpp"
#include "../../../System/Math/Primitive/Others/Ray.hpp"
#include "../../../System/Math/Structure/Basis.hpp"
#include "../GameComponent/LogicComponent.hpp"

namespace CS460
{
    class CameraComponent;
    class MouseInput;
    class KeyboardInput;
    class SpacePath;
    class AnimationSpace;
    class Manipulator;
    class Transform;
    class PrimitiveRenderer;

    class AnchorComponent final : public LogicComponent
    {
    public:
        ~AnchorComponent();
        AnchorComponent()                                      = delete;
        AnchorComponent(const AnchorComponent& rhs)            = delete;
        AnchorComponent& operator=(const AnchorComponent& rhs) = delete;

        void Initialize() override;
        void Update(Real dt) override;
        void Shutdown() override;
        void Render() override;

    protected:
        bool Load(const Json::Value& data) override;
        void Save(Json::Value& data) const override;
        void Edit(CommandRegistry* command_registry) override;
        void Subscribe() override;
        void Unsubscribe() override;

    private:
        friend class AnchorFactory;

    private:
        explicit AnchorComponent(Object* owner);
        void     Clone(AnchorComponent* origin);

    private:
        CameraComponent* m_camera      = nullptr;
        Transform*       m_transform   = nullptr;
        KeyboardInput*   m_key_input   = nullptr;
        MouseInput*      m_mouse_input = nullptr;

        Basis  m_basis;
        Plane  m_plane;
        Ray    m_picking_ray;
        Sphere m_anchor;

        bool m_b_edit = false;
        int m_click_count = 0;
    };
}
