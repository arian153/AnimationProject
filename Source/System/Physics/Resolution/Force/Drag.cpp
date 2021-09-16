#include "Drag.hpp"

#include "../../../../External/imgui/imgui.h"
#include "../../Dynamics/RigidBody.hpp"
#include "../../../../Manager/Resource/ResourceType/JsonResource.hpp"
#include "../../../../External/JSONCPP/json/json.h"

namespace CS460
{
    DragFactory::DragFactory()
    {
        type = typeid(Drag).name();
    }

    DragFactory::~DragFactory()
    {
    }

    Force* DragFactory::Create()
    {
        auto create = new Drag();
        return create;
    }

    Drag::Drag()
    {
        type = typeid(Drag).name();
    }

    Drag::~Drag()
    {
    }

    void Drag::SetAngularDrag(Real drag)
    {
        angular_drag = drag;
    }

    void Drag::SetLinearDrag(Real drag)
    {
        linear_drag = drag;
    }

    void Drag::Initialize()
    {
    }

    void Drag::Update(RigidBody* body, Real dt)
    {
        if (body->GetMotionMode() == eMotionMode::Dynamic)
        {
            //Real    speed           = body->GetLinearVelocity().Length();
            //Real    an_speed        = body->GetAngularVelocity().Length();
            //Real    drag_multiplier = 6.f * Math::PI * air_viscosity * density * drag;
            //Vector3 drag_force      = -body->GetLinearVelocity().Normalize();
            //drag_force *= (drag_multiplier * speed);
            //Vector3 drag_torque = -body->GetAngularVelocity().Normalize();
            //drag_torque *= (drag_multiplier * an_speed);
            //body->ApplyForceCentroid(drag_force);
            //body->ApplyTorque(drag_torque);

            Vector3 vel     = body->GetLinearVelocity();
            Vector3 changed = vel * powf(1.0f - linear_drag, dt);

            body->SetLinearVelocity(changed);
            body->SetAngularVelocity(body->GetAngularVelocity() * powf(1.0f - angular_drag, dt));

            if (draw_force.b_flag)
            {
                applied_forces.emplace_back(body->GetPosition(), (changed - vel) * body->Mass() * dt);
            }
        }
    }

    void Drag::Shutdown()
    {
    }

    void Drag::Load(const Json::Value& data)
    {
        if (JsonResource::HasMember(data, "Linear Drag") && data["Linear Drag"].isDouble())
        {
            linear_drag = data["Linear Drag"].asFloat();
        }
        if (JsonResource::HasMember(data, "Angular Drag") && data["Angular Drag"].isDouble())
        {
            angular_drag = data["Angular Drag"].asFloat();
        }
        if (JsonResource::HasMember(data, "Draw Force"))
        {
            Json::Value value = data["Draw Force"];
            if (JsonResource::HasMember(value, "Color") && JsonResource::IsColor(value["Color"]))
            {
                draw_force.color = JsonResource::AsColor(value["Color"]);
            }
            if (JsonResource::HasMember(value, "Flag") && value["Flag"].isBool())
            {
                draw_force.b_flag = value["Flag"].asBool();
            }
        }
    }

    void Drag::Save(const Json::Value& data)
    {
    }

    void Drag::Edit(CommandRegistry* registry)
    {
        if (ImGui::TreeNode("Drag Force"))
        {
            ImGui::Text("Linear Drag");
            ImGui::SliderFloat(("##Linear edit" + std::to_string(unique_id)).c_str(), &linear_drag, 0.0f, 1.0f);
            ImGui::Text("Angular Drag");
            ImGui::SliderFloat(("##Angular edit" + std::to_string(unique_id)).c_str(), &angular_drag, 0.0f, 1.0f);

            ImGui::Text("Show Force");
            ImGui::SameLine();
            ImGui::Checkbox(("##Show Drag Force" + std::to_string(unique_id)).c_str(), &draw_force.b_flag);
            ImGui::ColorEdit4(("##Edit Color Drag Force" + std::to_string(unique_id)).c_str(), &draw_force.color.r);
            ImGui::TreePop();
            ImGui::Separator();
        }
    }
}
