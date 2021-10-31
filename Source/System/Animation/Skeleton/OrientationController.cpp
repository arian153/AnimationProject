#include "OrientationController.hpp"

#include "../../Math/Algebra/Matrix33.hpp"
#include "../../Math/Structure/Basis.hpp"
#include "../Space/SpacePath.hpp"

namespace CS460
{
    class Basis;

    OrientationController::OrientationController()
    {
    }

    OrientationController::~OrientationController()
    {
    }

    void OrientationController::SetMode(eCOIMode coi_mode)
    {
        m_mode = (int)coi_mode;
    }

    eCOIMode OrientationController::GetMode() const
    {
        return (eCOIMode)m_mode;
    }

    void OrientationController::SetCOI(const Vector3& coi)
    {
        m_coi = coi;
    }

    void OrientationController::SetDelta(Real du)
    {
        m_du = du;
    }

    Quaternion OrientationController::GetOrientation(Real s)
    {
        Quaternion result;
        if (m_space_path != nullptr)
        {
            //Get u param from s param.
            Real    u   = m_space_path->InvArcLength(s);
            Vector3 pos = m_space_path->SpaceCurve(u);
            Vector3 coi;
            bool    b_replace = false;

            if (m_mode == 0)
            {
                //orbit mode center of interest.
                coi = m_coi;
            }
            else if (u + m_du > 1.0f)
            {
                //skip calculation
                b_replace = true;
            }
            else
            {
                //forward mode center of interest.
                coi = (m_space_path->SpaceCurve(u + m_du)
                    + m_space_path->SpaceCurve(u + 2.0f * m_du)
                    + m_space_path->SpaceCurve(u + 3.0f * m_du)) / 3.0f;
            }

            //calculate W, U, V for construct orientation.
            Vector3 w_vec = (coi - pos).Normalize();
            if (b_replace)
            {
                w_vec = m_saved_direction;
            }
            Vector3 u_vec     = CrossProduct(Math::Vector3::UP, w_vec).Normalize();
            Vector3 v_vec     = CrossProduct(w_vec, u_vec).Normalize();
            m_saved_direction = w_vec;

            //generate quaternion orientation.
            result.Set(Vector3(1.0f), w_vec);
            return result;
        }

        return result;
    }

    void OrientationController::SetPath(SpacePath* path)
    {
        m_space_path = path;
    }
}
