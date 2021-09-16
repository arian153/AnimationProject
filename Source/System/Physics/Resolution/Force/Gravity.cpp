#include "Gravity.hpp"

#include "../../../../External/imgui/imgui.h"
#include "../../Dynamics/RigidBody.hpp"
#include "../../../../Manager/Resource/ResourceType/JsonResource.hpp"
#include "../../../../External/JSONCPP/json/json.h"

namespace CS460
{
    GravityFactory::GravityFactory()
    {
        type = typeid(Gravity).name();
    }

    GravityFactory::~GravityFactory()
    {
    }

    Force* GravityFactory::Create()
    {
        auto created = new Gravity();
        return created;
    }

    Gravity::Gravity()
    {
        type = typeid(Gravity).name();
    }

    Gravity::~Gravity()
    {
    }

    void Gravity::Initialize()
    {
    }

    void Gravity::Update(RigidBody* body, Real dt)
    {
        if (body->GetMotionMode() == eMotionMode::Dynamic)
        {
            if (gravity_type == 0)
            {
                body->AddLinearVelocity(global_gravity * dt);
                if (draw_force.b_flag)
                {
                    applied_forces.emplace_back(body->GetPosition(), global_gravity * body->Mass());
                }
            }
            else
            {
                Vector3 distance  = local_position - body->GetCentroid();
                Real    r_r       = distance.LengthSquared();
                Real    force     = r_r > 1.0f ? gravity_coefficient * local_mass * body->Mass() * dt / r_r : 0.0f;
                Vector3 force_vec = force * distance.Normalize();
                body->ApplyForceCentroid(force_vec);
                if (draw_force.b_flag)
                {
                    applied_forces.emplace_back(body->GetPosition(), force_vec * dt);
                }
            }
        }
    }

    void Gravity::Shutdown()
    {
    }

    void Gravity::Load(const Json::Value& data)
    {
        if (JsonResource::HasMember(data, "Gravity Coefficient") && data["Gravity Coefficient"].isDouble())
        {
            gravity_coefficient = data["Gravity Coefficient"].asFloat();
        }
        if (JsonResource::HasMember(data, "Local Mass") && data["Local Mass"].isDouble())
        {
            local_mass = data["Local Mass"].asFloat();
        }
        if (JsonResource::HasMember(data, "Local Position") && JsonResource::IsVector3(data["Local Position"]))
        {
            local_position = JsonResource::AsVector3(data["Local Position"]);
        }
        if (JsonResource::HasMember(data, "Global Gravity") && JsonResource::IsVector3(data["Global Gravity"]))
        {
            global_gravity = JsonResource::AsVector3(data["Global Gravity"]);
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

    void Gravity::Save(const Json::Value& data)
    {
    }

    void Gravity::Edit(CommandRegistry* registry)
    {
        if (ImGui::TreeNode("Gravity Force"))
        {
            ImGui::Text("Gravity Type");
            const char* gravity_types[] = {"Global", "Point"};
            ImGui::Combo(("##GravityType" + std::to_string(unique_id)).c_str(), &gravity_type, gravity_types, 2);

            if (gravity_type == 0)
            {
                ImGui::InputFloat3(("##Global edit" + std::to_string(unique_id)).c_str(), &global_gravity.x);
            }
            else
            {
                ImGui::Text("Gravity Point");
                ImGui::InputFloat3(("##Point edit" + std::to_string(unique_id)).c_str(), &local_position.x);
                ImGui::Text("Point Mass");
                ImGui::InputFloat(("##Mass edit" + std::to_string(unique_id)).c_str(), &local_mass);
                ImGui::Text("Point Coefficient");
                ImGui::InputFloat(("##Coef edit" + std::to_string(unique_id)).c_str(), &gravity_coefficient);
            }

            ImGui::Text("Show Force");
            ImGui::SameLine();
            ImGui::Checkbox(("##Show Force" + std::to_string(unique_id)).c_str(), &draw_force.b_flag);
            ImGui::ColorEdit4(("##Edit Color Force" + std::to_string(unique_id)).c_str(), &draw_force.color.r);
            ImGui::TreePop();
            ImGui::Separator();
        }
    }

    void Gravity::SetGlobalGravity(const Vector3& dir, Real magnitude)
    {
        global_gravity = dir.Normalize() * magnitude;
        gravity_type   = 0;
    }

    void Gravity::SetLocalGravity(const Vector3& pos, Real mass, Real coefficient)
    {
        local_position      = pos;
        local_mass          = mass;
        gravity_coefficient = coefficient;
        gravity_type        = 1;
    }
}
