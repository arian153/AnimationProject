#pragma once
#include "../../Math/Algebra/Quaternion.hpp"
#include "../../Math/Algebra/Vector3.hpp"

namespace CS460
{
    class SpacePath;

    enum class eCOIMode : int
    {
        OrbitMode,
        ForwardMode
    };

    class OrientationController
    {
    public:
        OrientationController();
        ~OrientationController();

        void SetCOI(const Vector3& coi);
        void SetDelta(Real du);
        void SetPath(SpacePath* path);
        void SetMode(eCOIMode coi_mode);

        eCOIMode   GetMode() const;
        Quaternion GetOrientation(Real s);

    public:
        SpacePath* m_space_path = nullptr;
        Vector3    m_coi;
        Vector3    m_saved_direction;
        int        m_mode = 1;
        Real       m_du   = 0.01f;
    };
}
