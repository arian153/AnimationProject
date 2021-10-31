#pragma once
#include "../../Math/Algebra/Quaternion.hpp"
#include "../../Math/Algebra/Vector3.hpp"

namespace CS460
{
    class SpacePath;

    enum class eCOIMode : int
    {
        OrbitMode,
        ForwardMode,
        TetherMode
    };

    class OrientationController
    {
    public:
        OrientationController();
        ~OrientationController();

        void     SetMode(eCOIMode coi_mode);
        eCOIMode GetMode() const;

        void SetCOI(const Vector3& coi);
        void SetDelta(Real du);

        Quaternion GetOrientation(Real s);
        Quaternion GetOrientation(Real s1, Real s2) const;

        void SetPath(SpacePath* path);
        void SetTetherPath(SpacePath* path);

    public:
        SpacePath* m_space_path  = nullptr;
        SpacePath* m_tether_path = nullptr;
        Vector3    m_coi;
        Vector3    m_saved_direction;
        int        m_mode = 1;
        Real       m_du   = 0.01f;
    };
}
