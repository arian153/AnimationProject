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
            Real    u   = m_space_path->InvArcLength(s);
            Vector3 pos = m_space_path->SpaceCurve(u);
            Vector3 coi;
            bool    b_replace = false;

            if (m_mode == 0)
            {
                coi = m_coi;
            }
            else if (u + m_du > 1.0f)
            {
                //skip calculation
                b_replace = true;
            }
            else
            {
                coi = (m_space_path->SpaceCurve(u + m_du)
                    + m_space_path->SpaceCurve(u + 2.0f * m_du)
                    + m_space_path->SpaceCurve(u + 3.0f * m_du)) / 3.0f;
            }

            Vector3 w_vec = (coi - pos).Normalize();
            if (b_replace)
            {
                w_vec = m_saved_direction;
            }
            Vector3 u_vec     = CrossProduct(Math::Vector3::UP, w_vec).Normalize();
            Vector3 v_vec     = CrossProduct(w_vec, u_vec).Normalize();
            m_saved_direction = w_vec;

            result.Set(Vector3(1.0f), w_vec);
            return result;
        }

        return result;
    }

    Quaternion OrientationController::GetOrientation(Real s1, Real s2) const
    {
        if (m_mode == 2 && m_tether_path != nullptr && m_space_path != nullptr)
        {
            Vector3 pos = m_space_path->SpaceCurve(m_space_path->InvArcLength(s1));
            Vector3 coi = m_tether_path->SpaceCurve(m_tether_path->InvArcLength(s2));

            Vector3 w_vec = (coi - pos).Normalize();
            Vector3 u_vec = CrossProduct(Math::Vector3::UP, w_vec).Normalize();
            Vector3 v_vec = CrossProduct(w_vec, u_vec).Normalize();

            Matrix33 rot_mat;
            rot_mat.SetRows(u_vec, v_vec, w_vec);
            return rot_mat.ToQuaternion();
        }
        return Quaternion();
    }

    void OrientationController::SetPath(SpacePath* path)
    {
        m_space_path = path;
    }

    void OrientationController::SetTetherPath(SpacePath* path)
    {
        m_tether_path = path;
    }
}
