#pragma once
#include "SupportPoint.hpp"
#include "Simplex.hpp"
#include "../Resolution/Contact/ContactPoint.hpp"
#include <unordered_map>
#include "../Resolution/Contact/ContactManifold.hpp"
#include "../BroadPhase/PotentialPair.hpp"

namespace CS460
{
    class MassPoint;
    class ColorFlag;
    class ManifoldTable;
    class ColliderPrimitive;
    class Polytope;

    class NarrowPhase
    {
    public:
        NarrowPhase();
        ~NarrowPhase();

        void Initialize();
        void Shutdown();

        void SetPrimitiveRenderer(PrimitiveRenderer* primitive_renderer);

        void GenerateContact(std::vector<ColliderPair>& potential_pairs, ManifoldTable* data_table);
        void GenerateContact(std::vector<SoftBodyPair>& potential_pairs);
        void GenerateContact(std::vector<CompoundPair>& potential_pairs);
        void Render(const ColorFlag& draw_gjk_flag, const ColorFlag& draw_epa_flag);

        SupportPoint GenerateCSOSupport(ColliderPrimitive* a, ColliderPrimitive* b, const Vector3& direction) const;
        SupportPoint GenerateCSOSupport(ColliderPrimitive* a, const Vector3& point, const Vector3& direction) const;
        SupportPoint GenerateCSOSupport(ColliderPrimitive* a, SoftBody* b, const MassPoint& mass_point, const Vector3& direction) const;

        bool GJKCollisionDetection(ColliderPrimitive* a, ColliderPrimitive* b, Simplex& simplex) const;
        bool GJKCollisionDetection(ColliderPrimitive* primitive, SoftBody* b, const MassPoint& mass_point, Simplex& simplex) const;
        bool GJKDistanceAlgorithm(ColliderPrimitive* collider, const Vector3& point, Simplex& simplex) const;
        bool EPAContactGeneration(ColliderPrimitive* a, ColliderPrimitive* b, Polytope& polytope, ContactPoint& result) const;
        bool EPAContactGeneration2D(ColliderPrimitive* a, ColliderPrimitive* b, Polytope& polytope, ContactPoint& result);

    private:
        size_t FindLeastSignificantComponent(const Vector3& vector3);
        void   BlowUpSimplexToTetrahedron(ColliderPrimitive* collider_a, ColliderPrimitive* collider_b, Simplex& simplex);

    private:
        PrimitiveRenderer*    m_primitive_renderer = nullptr;
        std::vector<Simplex>  m_simplexes;
        std::vector<Polytope> m_polytopes;

        size_t m_gjk_exit_iteration = 100;
        size_t m_epa_exit_iteration = 100;

        Vector3 m_search_dirs[6];
        Vector3 m_axes[3];
    };
}
