#pragma once
#include "../../Math/Utility/MathDef.hpp"
#include <vector>
#include "RigidBody.hpp"
#include "../Utility/PhysicsDef.hpp"
#include "../BroadPhase/BroadPhase.hpp"
#include "../Resolution/Contact/ContactManifold.hpp"
#include <unordered_map>

#include "../../Graphics/DataType/Color.hpp"
#include "../Utility/FrictionUtility.hpp"
#include "../BroadPhase/RayTest.hpp"
#include "../BroadPhase/PotentialPair.hpp"

namespace Json
{
    class Value;
}

namespace CS460
{
    class IBroadPhaseData;
    class Simplex;
    class CommandRegistry;
    class RayTest;
    class ForceFactory;
    class Force;
    class Constraint;
    class ManifoldTable;
    class ConstraintUtility;

    class World
    {
    public:
        World();
        ~World();

        void Initialize();
        void Update(Real dt);
        void Shutdown();
        void Render() const;

        void Load(const Json::Value& data);
        void Save(const Json::Value& data);
        void Edit(CommandRegistry* registry);

        void SetBroadPhaseMode(eBroadPhaseMode mode);
        void SetPrimitiveRenderer(PrimitiveRenderer* primitive_renderer);
        void SetForceFactory(std::unordered_map<std::string, ForceFactory*>* factories);
        void SetDrawFlagGJK(bool b_draw, const Color& color = ColorDef::Pure::White);
        void SetDrawFlagEPA(bool b_draw, const Color& color = ColorDef::Pure::White);
        void SetDrawFlagContact(bool b_draw, const Color& color = ColorDef::Pure::White);
        void SetDrawFlagPrimitive(bool b_draw, const Color& color = ColorDef::Pure::White);
        void SetDrawFlagBP(bool b_draw, const Color& color = ColorDef::Pure::White);
        void SetDrawFlagVelocity(bool b_draw, const Color& color = ColorDef::Pure::White);
        void SetDrawFlagPositionTrace(bool b_draw, const Color& color = ColorDef::Pure::White);

        ColliderPrimitive* CreateCollider(ColliderSet* collider_set, eColliderType type) const;
        Force*             CreateForce(const std::string& type) const;

        RigidBody*   AddRigidBody(RigidBody* body);
        ColliderSet* AddColliderSet(ColliderSet* set);
        void         AddPrimitive(ColliderPrimitive* collider_primitive) const;
        void         AddConstraint(Constraint* constraint) const;
        void         AddForce(Force* force) const;

        void RemoveRigidBody(RigidBody* body);
        void RemoveColliderSet(ColliderSet* set);
        void RemovePrimitive(ColliderPrimitive* collider_primitive) const;
        void RemoveConstraint(Constraint* constraint) const;
        void RemoveForce(Force* force) const;

        void SetVelocityIteration(int iteration) const;
        void SetPositionIteration(int iteration) const;
        void SetWarmStarting(bool b_warm_starting) const;

        FrictionUtility*   GetFrictionUtility() const;
        ConstraintUtility* GetConstraintUtility() const;

        //query
        ColliderPrimitive* Pick(const Vector3& point) const;
        void               Query(const BoundingAABB& aabb, std::vector<ColliderPrimitive*>& output) const;
        //ray
        void CastRay(RayCastResult& result, Real max_distance = -1.0f) const;
        void IntersectRay(RayIntersectionResult& result, Real max_distance = -1.0f) const;
        void TraceRay(RayTraceResult& result, Real max_distance = -1.0f, size_t reflect_count = 1) const;

        void AddRay(const Ray& ray, eRayTestOption option, Real max_distance = -1.0f, size_t reflect_count = 0, const Color& color = ColorDef::Pure::White);
        void AddRay(const RayTest& ray);

        void SetPickingRay(const Ray& ray);

        ColliderSet* PickColliderSet(const Ray& ray) const;

        bool GJKDistanceTest(ColliderPrimitive* collider, const Vector3& point, Simplex& simplex) const;

    private:
        void DrawPotentialPair() const;
        void UpdateResolutionPhase();

    private:
        eBroadPhaseMode    m_mode               = eBroadPhaseMode::DynamicBVH;
        BroadPhase*        m_broad_phase        = nullptr;
        NarrowPhase*       m_narrow_phase       = nullptr;
        Resolution*        m_resolution_phase   = nullptr;
        ManifoldTable*     m_manifold_table     = nullptr;
        PrimitiveRenderer* m_primitive_renderer = nullptr;
        PotentialPairs     m_potential_pairs;

        std::vector<ColliderSet*> m_collider_sets;
        std::vector<RigidBody*>   m_rigid_bodies;

        std::vector<RayTest> m_rays;
        Ray                  m_picking_ray;

        //factory
        std::unordered_map<std::string, ForceFactory*>* m_factories = nullptr;

        ColorFlag m_draw_broad_phase;
        ColorFlag m_draw_primitive;
        ColorFlag m_draw_gjk;
        ColorFlag m_draw_epa;
        ColorFlag m_draw_contact;
        ColorFlag m_draw_potential_pair;

        ColorFlag m_draw_velocity;
        ColorFlag m_draw_position;

        bool m_do_broad_phase    = true;
        bool m_do_narrow_phase   = true;
        bool m_do_resolution     = true;
        bool m_solve_constraints = true;
        int  m_broad_phase_mode  = 1;
    };
}
