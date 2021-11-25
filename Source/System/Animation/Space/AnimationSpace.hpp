#pragma once
#include "SpacePath.hpp"
#include "../../Math/Primitive/ConvexHull3D/Box.hpp"
#include "../../Math/Primitive/ConvexHull3D/Sphere.hpp"
#include "../../Math/Primitive/Others/Plane.hpp"
#include "../../Math/Structure/Transform.hpp"
#include "../../Math/Structure/VQSTransform.hpp"

namespace CS460
{
    class IKComponent;

    class AnimationSpace
    {
    public:
        AnimationSpace();
        ~AnimationSpace();

        void Initialize();
        void Update(Real dt);
        void Shutdown();

        void Draw(PrimitiveRenderer* renderer) const;

        bool PickBox(const Ray& picking_ray, Real& min_t, Real& max_t);
        void AddControlPoint(const Vector3& point);

        int CreateNewPath(const std::string& name);
        //E5_TODO - add broad phase for performance later.

        void SetIDX(int idx);
        void SetSkeleton(Skeleton* skeleton);
        void SetIKCompo(IKComponent* compo);

        Vector3     GetPathPoint(Real s);
        std::string GetPathName(int idx);
        int         GetPathIDX(const std::string& name);
        void        SetEditable(bool b_edit);

        SpacePath* GetCurrentPath();
        SpacePath* GetPath(int idx);

    public:
        std::vector<std::string> path_names;
        bool                     b_edit_path = false;
        bool                     b_edit_coi  = false;

    private:
        //this can be changed terrain structure.
        Box          m_platform_box;
        VQSTransform m_box_transform;
        Sphere       m_drawing_sphere;
        int          m_path_idx     = 0;
        Skeleton*    m_skeleton     = nullptr;
        IKComponent* m_ik_compo     = nullptr;
        Real         m_elapsed_time = 0.0f;

        std::vector<SpacePath> m_space_paths;
    };
}
