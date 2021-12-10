#include "AnchorComponent.hpp"

#include "CameraComponent.hpp"
#include "TransformComponent.hpp"
#include "../../../System/Core/Input/InputCommon.hpp"
#include "../../../System/Core/Input/KeyCode.hpp"
#include "../../../System/Core/Input/MouseInput.hpp"
#include "../../../System/Graphics/Utility/PrimitiveRenderer.hpp"
#include "../../../System/Logic/LogicSubsystem.hpp"
#include "../../../System/Math/Structure/Transform.hpp"
#include "../../Object/Object.hpp"
#include "../../Object/ObjectManager.hpp"
#include "../../Space/Space.hpp"

namespace CS460
{
    AnchorComponent::~AnchorComponent()
    {
    }

    void AnchorComponent::Initialize()
    {
        if (m_space != nullptr)
        {
            m_space->GetLogicSubsystem()->InitializeLogic(this);
        }
        Subscribe();

        if (m_transform == nullptr)
        {
            if (m_owner->HasComponent<TransformComponent>())
            {
                m_transform = m_owner->GetComponent<TransformComponent>()->GetTransform();
            }
        }

        auto input    = m_space->GetLogicSubsystem()->GetInput();
        m_mouse_input = input->GetMouseInput();
        m_key_input   = input->GetKeyboardInput();

        m_camera        = m_space->GetObjectManager()->FindObjectBegin("Camera")->GetComponent<CameraComponent>();
        m_anchor.radius = 0.35f;

        m_primitive_renderer->DrawPrimitiveInstancing(m_anchor, m_anchor.position, eRenderingMode::Face, Color(1, 0, 0));
    }

    void AnchorComponent::Update(Real dt)
    {
        m_basis           = m_camera->GetBasis();
        m_anchor.position = m_transform->position;
        m_picking_ray     = m_space->GetLogicSubsystem()->GetPickingRay();

        if (m_mouse_input->IsDown(eKeyCodeMouse::Left) == false)
        {
            m_b_edit      = false;
          
        }
        if (m_mouse_input->IsPressed(eKeyCodeMouse::Left))
        {
            Ray model_ray = m_picking_ray;
            model_ray.position -= m_anchor.position;
            Real min, max;

            if (m_anchor.TestRayIntersection(model_ray, min, max))
            {
                Vector3 depth = m_basis.ApplyTransform(m_anchor.position);
                m_plane.Set(m_basis.k, -depth.z);

                m_b_edit = true;
            }
        }


        if (m_mouse_input->IsDown(eKeyCodeMouse::Left))
        {
            if (m_b_edit)
            {
                Real ray_t;
                if (m_plane.IntersectRay(m_picking_ray, ray_t))
                {
                    m_transform->position = m_picking_ray.ParamToPoint(ray_t);
                }
            }
        }
    }

    void AnchorComponent::Shutdown()
    {
        Unsubscribe();
    }

    void AnchorComponent::Render()
    {
        m_primitive_renderer->DrawPrimitiveInstancing(m_anchor, m_anchor.position, eRenderingMode::Face, Color(1, 0, 0));
    }

    bool AnchorComponent::Load(const Json::Value& data)
    {
        return true;
    }

    void AnchorComponent::Save(Json::Value& data) const
    {
    }

    void AnchorComponent::Edit(CommandRegistry* command_registry)
    {
        ImGui::CollapsingHeader(m_type.c_str(), &m_b_open);
    }

    void AnchorComponent::Subscribe()
    {
        if (m_space != nullptr)
        {
            m_space->GetLogicSubsystem()->AddLogic(this);
        }
    }

    void AnchorComponent::Unsubscribe()
    {
        if (m_space != nullptr)
        {
            m_space->GetLogicSubsystem()->RemoveLogic(this);
        }
    }

    AnchorComponent::AnchorComponent(Object* owner)
        : LogicComponent(owner)
    {
    }

    void AnchorComponent::Clone(AnchorComponent* origin)
    {
    }
}
