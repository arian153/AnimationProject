#include "World.hpp"
#include "../BroadPhase/DynamicBVH.hpp"
#include "../BroadPhase/NSquared.hpp"
#include "../BroadPhase/GridPartition.hpp"
#include "../NarrowPhase/NarrowPhase.hpp"
#include "../NarrowPhase/ManifoldTable.hpp"
#include "ColliderSet.hpp"
#include "../../../External/imgui/imgui.h"
#include "../ColliderPrimitive/ColliderPrimitive.hpp"
#include "../Resolution/Resolution.hpp"
#include "../../Graphics/Utility/PrimitiveRenderer.hpp"
#include "../../Core/Utility/CoreUtility.hpp"
#include "../Resolution/Force/Force.hpp"
#include "../../../Manager/Resource/ResourceType/JsonResource.hpp"
#include "../../../External/JSONCPP/json/json.h"
#include "../../Graphics/Utility/TextRenderer.hpp"

namespace CS460
{
    World::World()
    {
    }

    World::~World()
    {
    }

    void World::Initialize()
    {
        if (m_broad_phase == nullptr)
        {
            switch (m_mode)
            {
            case eBroadPhaseMode::DynamicBVH:
                m_broad_phase = new DynamicBVH();
                break;
            case eBroadPhaseMode::NSquared:
                m_broad_phase = new NSquared();
                break;
            case eBroadPhaseMode::GridPartition:
                m_broad_phase = new GridPartition();
                break;
            default:
                m_broad_phase = new DynamicBVH();
                break;
            }
            m_broad_phase->Initialize();
        }
        m_manifold_table = new ManifoldTable();
        m_narrow_phase   = new NarrowPhase();
        m_narrow_phase->Initialize();
        m_resolution_phase = new Resolution();
        m_resolution_phase->Initialize();
    }

    void World::Render() const
    {
        if (m_draw_potential_pair.b_flag)
        {
            DrawPotentialPair();
        }

        if (m_draw_primitive.b_flag || m_draw_broad_phase.b_flag)
        {
            m_broad_phase->Render(m_primitive_renderer, m_draw_broad_phase, m_draw_primitive);
        }

        if (m_draw_gjk.b_flag || m_draw_epa.b_flag)
        {
            m_narrow_phase->Render(m_draw_gjk, m_draw_epa);
        }
        m_resolution_phase->Render(m_draw_contact);
        Quaternion no_rotation;
        for (auto& body : m_rigid_bodies)
        {
            Vector3 vel = body->GetLinearVelocity();
            Vector3 pos = body->GetCentroid();

            if (m_draw_velocity.b_flag)
            {
                m_primitive_renderer->DrawArrow(pos, pos + vel, m_draw_velocity.color);
            }
            if (m_draw_position.b_flag)
            {
                m_primitive_renderer->DrawPrimitive(Sphere(pos, no_rotation, 0.05f), eRenderingMode::Face, m_draw_position.color);
            }
        }

        for (auto& data : m_rays)
        {
            if (data.option == eRayTestOption::Cast)
            {
                RayCastResult result(data.ray);
                m_broad_phase->CastRay(result, data.max_distance);
                if (result.hit_data.hit == true)
                {
                    m_primitive_renderer->DrawSegment(data.ray.position, result.hit_data.intersection, data.color);
                }
                else
                {
                    m_primitive_renderer->DrawRay(data.ray, data.color);
                }
            }
            else if (data.option == eRayTestOption::Intersect)
            {
                RayIntersectionResult result(data.ray);
                m_broad_phase->IntersectRay(result, data.max_distance);
                m_primitive_renderer->DrawRay(data.ray, data.color);
                for (auto& hit_data : result.hit_list)
                {
                    m_primitive_renderer->DrawPrimitive(Sphere(hit_data.intersection, no_rotation, 0.1f), eRenderingMode::Face, data.color);
                }
            }
            else
            {
                RayTraceResult result(data.ray);
                m_broad_phase->TraceRay(result, data.max_distance, data.reflect_count);
                if (!result.hit_list.empty())
                {
                    size_t hit_size = result.hit_list.size();
                    size_t ray_size = result.ray_list.size();
                    for (size_t i = 0; i < hit_size; ++i)
                    {
                        auto ray = result.GetRayData(i);
                        auto hit = result.GetHitData(i);
                        m_primitive_renderer->DrawSegment(ray.position, hit.intersection, data.color);
                        m_primitive_renderer->DrawPrimitive(Sphere(hit.intersection, no_rotation, 0.1f), eRenderingMode::Face, data.color);
                        //E5_DRAW_TEXT_OUTPUT(Vector2(0, i * 80.0f), data.color, hit.intersection);
                    }
                    if (ray_size > hit_size)
                    {
                        auto ray = result.GetRayData(hit_size);
                        m_primitive_renderer->DrawRay(ray, data.color);
                    }
                }
                else
                {
                    m_primitive_renderer->DrawRay(data.ray, data.color);
                }
            }
        }
    }

    void World::Load(const Json::Value& data)
    {
        if (JsonResource::HasMember(data, "Draw Broad Phase"))
        {
            ColorFlag   flag;
            Json::Value value = data["Draw Broad Phase"];
            if (JsonResource::HasMember(value, "Color") && JsonResource::IsColor(value["Color"]))
            {
                flag.color = JsonResource::AsColor(value["Color"]);
            }
            if (JsonResource::HasMember(value, "Flag") && value["Flag"].isBool())
            {
                flag.b_flag = value["Flag"].asBool();
            }
            SetDrawFlagBP(flag.b_flag, flag.color);
        }
        if (JsonResource::HasMember(data, "Draw Contact"))
        {
            ColorFlag   flag;
            Json::Value value = data["Draw Contact"];
            if (JsonResource::HasMember(value, "Color") && JsonResource::IsColor(value["Color"]))
            {
                flag.color = JsonResource::AsColor(value["Color"]);
            }
            if (JsonResource::HasMember(value, "Flag") && value["Flag"].isBool())
            {
                flag.b_flag = value["Flag"].asBool();
            }
            SetDrawFlagContact(flag.b_flag, flag.color);
        }
        if (JsonResource::HasMember(data, "Draw EPA"))
        {
            ColorFlag   flag;
            Json::Value value = data["Draw EPA"];
            if (JsonResource::HasMember(value, "Color") && JsonResource::IsColor(value["Color"]))
            {
                flag.color = JsonResource::AsColor(value["Color"]);
            }
            if (JsonResource::HasMember(value, "Flag") && value["Flag"].isBool())
            {
                flag.b_flag = value["Flag"].asBool();
            }
            SetDrawFlagEPA(flag.b_flag, flag.color);
        }
        if (JsonResource::HasMember(data, "Draw GJK"))
        {
            ColorFlag   flag;
            Json::Value value = data["Draw GJK"];
            if (JsonResource::HasMember(value, "Color") && JsonResource::IsColor(value["Color"]))
            {
                flag.color = JsonResource::AsColor(value["Color"]);
            }
            if (JsonResource::HasMember(value, "Flag") && value["Flag"].isBool())
            {
                flag.b_flag = value["Flag"].asBool();
            }
            SetDrawFlagGJK(flag.b_flag, flag.color);
        }
        if (JsonResource::HasMember(data, "Draw Primitive"))
        {
            ColorFlag   flag;
            Json::Value value = data["Draw Primitive"];
            if (JsonResource::HasMember(value, "Color") && JsonResource::IsColor(value["Color"]))
            {
                flag.color = JsonResource::AsColor(value["Color"]);
            }
            if (JsonResource::HasMember(value, "Flag") && value["Flag"].isBool())
            {
                flag.b_flag = value["Flag"].asBool();
            }
            SetDrawFlagPrimitive(flag.b_flag, flag.color);
        }

        if (JsonResource::HasMember(data, "Draw Velocity"))
        {
            ColorFlag   flag;
            Json::Value value = data["Draw Velocity"];
            if (JsonResource::HasMember(value, "Color") && JsonResource::IsColor(value["Color"]))
            {
                flag.color = JsonResource::AsColor(value["Color"]);
            }
            if (JsonResource::HasMember(value, "Flag") && value["Flag"].isBool())
            {
                flag.b_flag = value["Flag"].asBool();
            }
            SetDrawFlagVelocity(flag.b_flag, flag.color);
        }

        if (JsonResource::HasMember(data, "Draw Position"))
        {
            ColorFlag   flag;
            Json::Value value = data["Draw Position"];
            if (JsonResource::HasMember(value, "Color") && JsonResource::IsColor(value["Color"]))
            {
                flag.color = JsonResource::AsColor(value["Color"]);
            }
            if (JsonResource::HasMember(value, "Flag") && value["Flag"].isBool())
            {
                flag.b_flag = value["Flag"].asBool();
            }
            SetDrawFlagPositionTrace(flag.b_flag, flag.color);
        }

        if (JsonResource::HasMember(data, "Do Broad Phase"))
        {
            m_do_broad_phase = data["Do Broad Phase"].asBool();
        }

        if (JsonResource::HasMember(data, "Do Narrow Phase"))
        {
            m_do_narrow_phase = data["Do Narrow Phase"].asBool();
        }

        if (JsonResource::HasMember(data, "Do Resolution"))
        {
            m_do_resolution = data["Do Resolution"].asBool();
        }

        if (JsonResource::HasMember(data, "Solve Constraints"))
        {
            m_solve_constraints = data["Solve Constraints"].asBool();
        }

        if (JsonResource::HasMember(data, "Enable Sleep"))
        {
            m_resolution_phase->m_b_enable_sleep = data["Enable Sleep"].asBool();
        }

        if (JsonResource::HasMember(data, "Forces") && data["Forces"].isArray())
        {
            for (auto it = data["Forces"].begin(); it != data["Forces"].end(); ++it)
            {
                //Load Components
                if (JsonResource::HasMember(*it, "Type") && (*it)["Type"].isString())
                {
                    std::string type    = (*it)["Type"].asString();
                    Force*      created = CreateForce(type);
                    if (created != nullptr)
                    {
                        created->Load((*it)["Value"]);
                        AddForce(created);
                    }
                }
            }
        }
        if (JsonResource::HasMember(data, "Rays") && data["Rays"].isArray())
        {
            for (auto it = data["Rays"].begin(); it != data["Rays"].end(); ++it)
            {
                RayTest ray_test;
                if (JsonResource::HasMember(*it, "Option") && (*it)["Option"].isString())
                {
                    std::string option_data = (*it)["Option"].asString();
                    if (option_data == "Cast")
                    {
                        ray_test.option = eRayTestOption::Cast;
                    }
                    if (option_data == "Intersect")
                    {
                        ray_test.option = eRayTestOption::Intersect;
                    }
                    if (option_data == "Trace")
                    {
                        ray_test.option = eRayTestOption::Trace;
                    }
                }
                if (JsonResource::HasMember(*it, "Position") && JsonResource::IsVector3((*it)["Position"]))
                {
                    ray_test.ray.position = JsonResource::AsVector3((*it)["Position"]);
                }
                if (JsonResource::HasMember(*it, "Direction") && JsonResource::IsVector3((*it)["Direction"]))
                {
                    ray_test.ray.direction = JsonResource::AsVector3((*it)["Direction"]).Normalize();
                }
                if (JsonResource::HasMember(*it, "Max Distance") && (*it)["Max Distance"].isDouble())
                {
                    ray_test.max_distance = (*it)["Max Distance"].asFloat();
                }
                if (JsonResource::HasMember(*it, "Reflection Count") && (*it)["Reflection Count"].isUInt())
                {
                    ray_test.reflect_count = static_cast<size_t>((*it)["Reflection Count"].asUInt());
                }
                if (JsonResource::HasMember(*it, "Color") && JsonResource::IsColor((*it)["Color"]))
                {
                    ray_test.color = JsonResource::AsColor((*it)["Color"]);
                }
                AddRay(ray_test);
            }
        }
    }

    void World::Save(const Json::Value& data)
    {
    }

    void World::Edit(CommandRegistry* registry)
    {
        if (ImGui::CollapsingHeader("Space-World"))
        {
            ImGui::Text("Object Count : %i", m_rigid_bodies.size());

            if (ImGui::TreeNode("Broad Phase"))
            {
                ImGui::Text("Broad Phase Type");
                const char* broad_phase_types[] = {"N-Squared", "Dynamic-BVH"};
                if (ImGui::Combo("##CalculateMethod", &m_broad_phase_mode, broad_phase_types, 2))
                {
                    if (m_broad_phase_mode == 0)
                    {
                        SetBroadPhaseMode(eBroadPhaseMode::NSquared);
                    }
                    else if (m_broad_phase_mode == 1)
                    {
                        SetBroadPhaseMode(eBroadPhaseMode::DynamicBVH);
                    }
                }

                ImGui::Text("Execute Broad Phase");
                ImGui::SameLine();
                ImGui::Checkbox("##Do Broad Phase", &m_do_broad_phase);
                ImGui::Text("Show Broad Phase");
                ImGui::SameLine();
                ImGui::Checkbox("##Show Broad Phase", &m_draw_broad_phase.b_flag);
                ImGui::ColorEdit4("##Edit Color Broad Phase", &m_draw_broad_phase.color.r);

                ImGui::Text("Show Primitive");
                ImGui::SameLine();
                ImGui::Checkbox("##Show Primitive", &m_draw_primitive.b_flag);
                ImGui::ColorEdit4("##Edit Color Primitive", &m_draw_primitive.color.r);

                ImGui::Text("Show Potential Pair");
                ImGui::SameLine();
                ImGui::Checkbox("##Show Potential Pair", &m_draw_potential_pair.b_flag);
                ImGui::ColorEdit4("##Edit Color Potential Pair", &m_draw_potential_pair.color.r);

                ImGui::TreePop();
                ImGui::Separator();
            }

            if (ImGui::TreeNode("Narrow Phase"))
            {
                ImGui::Text("Execute Narrow Phase");
                ImGui::SameLine();
                ImGui::Checkbox("##Do Narrow Phase", &m_do_narrow_phase);

                ImGui::Text("Show GJK");
                ImGui::ColorEdit4("##Edit Color GJK", &m_draw_gjk.color.r);
                ImGui::SameLine();
                ImGui::Checkbox("##Show GJK", &m_draw_gjk.b_flag);

                ImGui::Text("Show EPA");
                ImGui::ColorEdit4("##Edit Color EPA", &m_draw_epa.color.r);
                ImGui::SameLine();
                ImGui::Checkbox("##Show EPA", &m_draw_epa.b_flag);
                ImGui::TreePop();
                ImGui::Separator();
            }

            if (ImGui::TreeNode("Resolution Phase"))
            {
                ImGui::Text("Execute Resolution Phase");
                ImGui::SameLine();
                ImGui::Checkbox("##Do Resolution Phase", &m_do_resolution);

                ImGui::Text("Solve Constraints");
                ImGui::SameLine();
                ImGui::Checkbox("##Solve Constraints", &m_solve_constraints);

                ImGui::Text("Enable Sleep");
                ImGui::SameLine();
                if (ImGui::Checkbox("##Enable Sleep", &m_resolution_phase->m_b_enable_sleep))
                {
                    if (m_resolution_phase->m_b_enable_sleep == false)
                    {
                        for (auto& body : m_rigid_bodies)
                        {
                            body->SetAwake();
                        }
                    }
                }

                ImGui::Text("Enable Baumgarte");
                ImGui::SameLine();
                ImGui::Checkbox("##Enable Baumgarte", &m_resolution_phase->m_b_enable_baumgarte);

                ImGui::Text("Enable Warm Start");
                ImGui::SameLine();
                ImGui::Checkbox("##Enable Warm Start", &m_resolution_phase->m_b_warm_starting);

                ImGui::Text("Velocity Iteration");
                ImGui::SliderInt("##Velocity Iteration", &m_resolution_phase->m_velocity_iteration, 1, 10);

                ImGui::Text("Position Iteration");
                ImGui::SliderInt("##Position Iteration", &m_resolution_phase->m_position_iteration, 0, 5);

                ImGui::Text("Show Contact");
                ImGui::SameLine();
                ImGui::Checkbox("##Show Contact", &m_draw_contact.b_flag);
                ImGui::ColorEdit4("##Edit Color Contact", &m_draw_contact.color.r);

                ImGui::Text("Show Velocity");
                ImGui::SameLine();
                ImGui::Checkbox("##Show Velocity", &m_draw_velocity.b_flag);
                ImGui::ColorEdit4("##Edit Color Velocity", &m_draw_velocity.color.r);

                ImGui::Text("Show Position");
                ImGui::SameLine();
                ImGui::Checkbox("##Show Position", &m_draw_position.b_flag);
                ImGui::ColorEdit4("##Edit Color Position", &m_draw_position.color.r);

                ImGui::TreePop();
                ImGui::Separator();
            }

            ImGui::NewLine();
            ImGui::Text("Forces");
            m_resolution_phase->Edit(registry);
        }
    }

    void World::Update(Real dt)
    {
        if (m_do_broad_phase)
        {
            m_broad_phase->Update(dt);
            m_broad_phase->ComputePairs(m_pairs);
        }

        if (m_do_narrow_phase)
        {
            m_manifold_table->FilteringManifolds();
            m_narrow_phase->GenerateContact(m_pairs, m_manifold_table);
        }

        if (m_do_resolution)
        {
            m_resolution_phase->ApplyForces(&m_rigid_bodies, dt);
            m_resolution_phase->ProcessContactConstraints(m_manifold_table);

            if (m_solve_constraints)
            {
                m_resolution_phase->SolveVelocityConstraints(dt);
            }

            m_resolution_phase->IntegrateRigidBodies(&m_rigid_bodies, dt);
            if (m_solve_constraints)
            {
                m_resolution_phase->SolvePositionConstraints(dt);
            }
        }
    }

    void World::Shutdown()
    {
        if (m_resolution_phase != nullptr)
        {
            m_resolution_phase->Shutdown();
            delete m_resolution_phase;
            m_resolution_phase = nullptr;
        }
        if (m_narrow_phase != nullptr)
        {
            m_narrow_phase->Shutdown();
            delete m_narrow_phase;
            m_narrow_phase = nullptr;
        }
        if (m_manifold_table != nullptr)
        {
            m_manifold_table->Shutdown();
            delete m_manifold_table;
            m_manifold_table = nullptr;
        }
        m_pairs.clear();
        for (auto& body : m_rigid_bodies)
        {
            body->Shutdown();
            delete body;
            body = nullptr;
        }
        m_rigid_bodies.clear();
        for (auto& collider : m_collider_sets)
        {
            collider->Shutdown();
            delete collider;
            collider = nullptr;
        }
        m_collider_sets.clear();
        if (m_broad_phase != nullptr)
        {
            m_broad_phase->Shutdown();
            delete m_broad_phase;
            m_broad_phase = nullptr;
        }
    }

    void World::SetBroadPhaseMode(eBroadPhaseMode mode)
    {
        if (mode != m_mode && m_broad_phase != nullptr)
        {
            BroadPhase* broad_phase;
            switch (mode)
            {
            case eBroadPhaseMode::DynamicBVH:
                broad_phase = new DynamicBVH();
                break;
            case eBroadPhaseMode::NSquared:
                broad_phase = new NSquared();
                break;
            case eBroadPhaseMode::GridPartition:
                broad_phase = new GridPartition();
                broad_phase->Initialize();
                break;
            default:
                broad_phase = new DynamicBVH();
                break;
            }
            m_broad_phase->Copy(broad_phase);
            m_broad_phase->Clear();
            delete m_broad_phase;
            m_broad_phase = broad_phase;
            m_mode        = mode;
        }
    }

    void World::SetPrimitiveRenderer(PrimitiveRenderer* primitive_renderer)
    {
        if (primitive_renderer != nullptr)
        {
            m_primitive_renderer = primitive_renderer;
            m_narrow_phase->SetPrimitiveRenderer(primitive_renderer);
            m_resolution_phase->SetPrimitiveRenderer(primitive_renderer);
        }
    }

    void World::SetForceFactory(std::unordered_map<std::string, ForceFactory*>* factories)
    {
        if (factories != nullptr)
        {
            m_factories = factories;
        }
    }

    void World::SetDrawFlagGJK(bool b_draw, const Color& color)
    {
        m_draw_gjk.b_flag = m_primitive_renderer != nullptr ? b_draw : false;
        m_draw_gjk.color  = color;
    }

    void World::SetDrawFlagEPA(bool b_draw, const Color& color)
    {
        m_draw_epa.b_flag = m_primitive_renderer != nullptr ? b_draw : false;
        m_draw_epa.color  = color;
    }

    void World::SetDrawFlagContact(bool b_draw, const Color& color)
    {
        m_draw_contact.b_flag = m_primitive_renderer != nullptr ? b_draw : false;
        m_draw_contact.color  = color;
    }

    void World::SetDrawFlagPrimitive(bool b_draw, const Color& color)
    {
        m_draw_primitive.b_flag = m_primitive_renderer != nullptr ? b_draw : false;
        m_draw_primitive.color  = color;
    }

    void World::SetDrawFlagBP(bool b_draw, const Color& color)
    {
        m_draw_broad_phase.b_flag = m_primitive_renderer != nullptr ? b_draw : false;
        m_draw_broad_phase.color  = color;
    }

    void World::SetDrawFlagVelocity(bool b_draw, const Color& color)
    {
        m_draw_velocity.b_flag = m_primitive_renderer != nullptr ? b_draw : false;
        m_draw_velocity.color  = color;
    }

    void World::SetDrawFlagPositionTrace(bool b_draw, const Color& color)
    {
        m_draw_position.b_flag = m_primitive_renderer != nullptr ? b_draw : false;
        m_draw_position.color  = color;
    }

    ColliderPrimitive* World::CreateCollider(ColliderSet* collider_set, eColliderType type) const
    {
        if (collider_set != nullptr)
        {
            auto primitive = collider_set->AddCollider(type);
            AddPrimitive(primitive);
            primitive->Initialize();
            return primitive;
        }
        return nullptr;
    }

    Force* World::CreateForce(const std::string& type) const
    {
        auto found = m_factories->find(type);
        if (found != m_factories->end())
        {
            auto created = found->second->Create();
            return created;
        }
        return nullptr;
    }

    RigidBody* World::AddRigidBody(RigidBody* body)
    {
        m_rigid_bodies.push_back(body);
        return body;
    }

    ColliderSet* World::AddColliderSet(ColliderSet* set)
    {
        m_collider_sets.push_back(set);
        return set;
    }

    void World::AddPrimitive(ColliderPrimitive* collider_primitive) const
    {
        BoundingAABB* bounding_volume = new BoundingAABB(collider_primitive);
        m_broad_phase->Add(bounding_volume);
    }

    void World::AddConstraint(Constraint* constraint) const
    {
        m_resolution_phase->m_constraints.push_back(constraint);
    }

    void World::AddForce(Force* force) const
    {
        force->Initialize();
        m_resolution_phase->m_forces.push_back(force);
    }

    void World::RemoveRigidBody(RigidBody* body)
    {
        auto found = std::find(m_rigid_bodies.begin(), m_rigid_bodies.end(), body);
        m_rigid_bodies.erase(found);
    }

    void World::RemoveColliderSet(ColliderSet* set)
    {
        auto found = std::find(m_collider_sets.begin(), m_collider_sets.end(), set);
        m_collider_sets.erase(found);
    }

    void World::RemovePrimitive(ColliderPrimitive* collider_primitive) const
    {
        m_broad_phase->Remove(collider_primitive->GetBoundingVolume());
    }

    void World::RemoveConstraint(Constraint* constraint) const
    {
        auto found = std::find(
                               m_resolution_phase->m_constraints.begin(),
                               m_resolution_phase->m_constraints.end(), constraint);
        m_resolution_phase->m_constraints.erase(found);
    }

    void World::RemoveForce(Force* force) const
    {
        auto found = std::find(
                               m_resolution_phase->m_forces.begin(),
                               m_resolution_phase->m_forces.end(), force);
        m_resolution_phase->m_forces.erase(found);
        force->Shutdown();
    }

    void World::SetVelocityIteration(int iteration) const
    {
        m_resolution_phase->m_velocity_iteration = iteration;
    }

    void World::SetPositionIteration(int iteration) const
    {
        m_resolution_phase->m_position_iteration = iteration;
    }

    void World::SetWarmStarting(bool b_warm_starting) const
    {
        m_resolution_phase->m_b_warm_starting = b_warm_starting;
    }

    FrictionUtility* World::GetFrictionUtility() const
    {
        return &m_resolution_phase->m_friction_utility;
    }

    ConstraintUtility* World::GetConstraintUtility() const
    {
        return &m_resolution_phase->m_constraint_utility;
    }

    ColliderPrimitive* World::Pick(const Vector3& point) const
    {
        return m_broad_phase->Pick(point);
    }

    void World::Query(const BoundingAABB& aabb, std::vector<ColliderPrimitive*>& output) const
    {
        m_broad_phase->Query(aabb, output);
    }

    void World::CastRay(RayCastResult& result, Real max_distance) const
    {
        m_broad_phase->CastRay(result, max_distance);
    }

    void World::IntersectRay(RayIntersectionResult& result, Real max_distance) const
    {
        m_broad_phase->IntersectRay(result, max_distance);
    }

    void World::TraceRay(RayTraceResult& result, Real max_distance, size_t reflect_count) const
    {
        m_broad_phase->TraceRay(result, max_distance, reflect_count);
    }

    void World::AddRay(const Ray& ray, eRayTestOption option, Real max_distance, size_t reflect_count, const Color& color)
    {
        m_rays.emplace_back(ray, option, max_distance, reflect_count, color);
    }

    void World::AddRay(const RayTest& ray)
    {
        m_rays.push_back(ray);
    }

    void World::SetPickingRay(const Ray& ray)
    {
        m_picking_ray = ray;
    }

    ColliderSet* World::PickColliderSet(const Ray& ray) const
    {
        RayCastResult result(ray);
        m_broad_phase->CastRay(result);

        if (result.hit_data.hit)
        {
            return result.hit_data.collider->GetColliderSet();
        }

        return nullptr;
    }

    bool World::GJKDistanceTest(ColliderPrimitive* collider, const Vector3& point, Simplex& simplex) const
    {
        return m_narrow_phase->GJKDistanceAlgorithm(collider, point, simplex);
    }

    void World::DrawPotentialPair() const
    {
        for (auto& pair : m_pairs)
        {
            auto a = pair.first->GetBoundingVolume();
            auto b = pair.second->GetBoundingVolume();
            m_primitive_renderer->DrawBox(a->Center(), Quaternion(), a->Size(), eRenderingMode::Line, m_draw_potential_pair.color);
            m_primitive_renderer->DrawBox(b->Center(), Quaternion(), b->Size(), eRenderingMode::Line, m_draw_potential_pair.color);
        }
    }

    void World::UpdateResolutionPhase()
    {
    }
}
