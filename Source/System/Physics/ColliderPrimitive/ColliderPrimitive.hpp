#pragma once
#include "../../Math/Math.hpp"
#include "..//Dynamics/RigidBody.hpp"
#include "../BroadPhase/RayCast.hpp"
#include "../Dynamics/ColliderSet.hpp"
#include "../Utility/MaterialDef.hpp"

namespace Json
{
    class Value;
}

namespace CS460
{
    class CommandRegistry;
    class Color;
    enum class eRenderingMode;
    class PrimitiveRenderer;
    class BoundingAABB;
    class RigidBody;
    class Transform;

    class ColliderPrimitive
    {
    public:
        ColliderPrimitive();
        virtual ~ColliderPrimitive();

        void CastRay(RayCastResult& result, Real max_distance = -1.0f);
        void IntersectRay(RayIntersectionResult& result, Real max_distance = -1.0f);

        Vector3 LocalToWorldPoint(const Vector3& local_point) const;
        Vector3 WorldToLocalPoint(const Vector3& world_point) const;
        Vector3 LocalToWorldVector(const Vector3& local_vector) const;
        Vector3 WorldToLocalVector(const Vector3& world_vector) const;

        Vector3 ClosestPointWorld(const Vector3& point);
        Vector3 ClosestPointLocal(const Vector3& point);
        Vector3 ClosestPointSimplex(const Vector3& point, bool b_global, Simplex& simplex);

        Matrix33 WorldInertia() const;
        Vector3  WorldCentroid() const;

        Vector3    GetBodyPosition() const;
        Quaternion GetBodyOrientation() const;
        Transform* GetBodyTransform() const;

        RigidBody*    GetRigidBody() const;
        BoundingAABB* GetBoundingVolume() const;
        ColliderSet*  GetColliderSet() const;
        Real          GetDensity() const;

        Vector3               ConvertBodyWorldPoint(const Vector3& local_point) const;
        void                  SetScale(const Vector3& scale);
        void                  SetMass(Real density);
        void                  SetMaterial(Physics::eMaterial material, Real d = 0.0f, Real r = 0.0f);
        Physics::eMaterial    GetMaterialCode() const;
        Physics::MaterialData GetMaterial() const;

        eColliderType Type() const;

        bool    Is2DPrimitive() const;
        Vector3 PlaneNormal2D() const;

    public: //virtual methods
        virtual void Initialize() = 0;
        virtual void Shutdown() = 0;

        //minkowski support - gjk, epa
        virtual Vector3 Support(const Vector3& direction) = 0;

        //ray - collider intersection
        virtual bool    TestRayIntersection(const Ray& local_ray, Real& minimum_t, Real& maximum_t) const = 0;
        virtual Vector3 GetNormal(const Vector3& local_point_on_collider) = 0;

        //physics
        virtual Real GetVolume() = 0;

        //collider 
        virtual void SetUnit() = 0;
        virtual void UpdateBoundingVolume() = 0;
        virtual void Draw(PrimitiveRenderer* renderer, eRenderingMode mode, const Color& color) const = 0;

    protected:
        void UpdatePrimitive();
        void UpdateMassData() const;
        void UpdateScaleData();
        void LoadMaterial(const Json::Value& data);
        void LoadTransform(const Json::Value& data);
        void LoadMass(const Json::Value& data);
        void EditCollider(CommandRegistry* registry);

    protected:
        virtual void SetMassData(Real density) = 0;
        virtual void SetScaleData(const Vector3& scale) = 0;
        virtual void Clone(ColliderPrimitive* origin) = 0;

        virtual void Load(const Json::Value& data) = 0;
        virtual void Save(const Json::Value& data) = 0;
        virtual void EditPrimitive(CommandRegistry* registry) = 0;

    private:
        void UpdateRigidBody();

    private:
        friend class ColliderSet;
        friend class RigidBody;
        friend class NarrowPhase;
        friend class BoundingAABB;
        friend class World;
        friend class ManifoldTable;
        friend class ColliderComponent;

    protected:
        //collider local space data
        Transform m_local;
        bool      m_is_2D    = false;

        //collider mass data
        Vector3  m_centroid; //center of mass
        Real     m_mass;
        Matrix33 m_local_inertia_tensor;

        //other data
        RigidBody*            m_rigid_body      = nullptr;
        ColliderSet*          m_collider_set    = nullptr;
        BoundingAABB*         m_bounding_volume = nullptr;
        eColliderType         m_type;
        Physics::MaterialData m_material;
    };
}
