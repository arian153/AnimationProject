#pragma once
#include "MathDef.hpp"

namespace CS460
{
    class AxisRadian;
    class Quaternion;
    class Matrix44;
    class Matrix33;
    class EulerAngle;
    class Vector3;
    class Vector4;

    namespace Math
    {
        namespace Matrix44
        {
            //rotation
            CS460::Matrix44 Rotation(const Quaternion& quaternion);
            CS460::Matrix44 Rotation(const CS460::Matrix33& rotation_matrix);
            CS460::Matrix44 Rotation(const EulerAngle& euler_angle);
            CS460::Matrix44 Rotation(const AxisRadian& axis_radian);

            //principle rotation
            CS460::Matrix44 RotationX(Real radian);
            CS460::Matrix44 RotationY(Real radian);
            CS460::Matrix44 RotationZ(Real radian);

            //scale
            CS460::Matrix44 Scale(const CS460::Vector4& scale);
            CS460::Matrix44 Scale(const CS460::Vector3& scale, Real w = 1.0f);
            CS460::Matrix44 Scale(Real x, Real y, Real z, Real w = 1.0f);


            //translation
            CS460::Matrix44 Translation(const CS460::Vector3& translation);

            //Matrix Generation
            CS460::Matrix44 AffineTransformation
            (
                const CS460::Vector3& scale,
                const CS460::Vector3& origin,
                const Quaternion&     rotation,
                const CS460::Vector3& translation
            );

            CS460::Matrix44 PerspectiveMatrix
            (
                Real screen_aspect,
                Real field_of_view,
                Real far_plane,
                Real near_plane
            );

            CS460::Matrix44 OrthoGraphicMatrix
            (
                size_t client_width,
                size_t client_height,
                Real   far_plane,
                Real   near_plane
            );

            CS460::Matrix44 OrthoGraphicCenterMatrix
            (
                Real right,
                Real left,
                Real top,
                Real bottom,
                Real far_plane,
                Real near_plane
            );

            CS460::Matrix44 LookAt
            (
                const CS460::Vector3& eye_position,
                const CS460::Vector3& focus_position,
                const CS460::Vector3& up_direction
            );

            CS460::Matrix44 LookTo
            (
                const CS460::Vector3& eye_position,
                const CS460::Vector3& eye_direction,
                const CS460::Vector3& up_direction
            );
        }

        namespace Matrix33
        {
            //rotation
            CS460::Matrix33 Rotation(const Quaternion& quaternion);
            CS460::Matrix33 Rotation(const CS460::Matrix44& rotation_matrix);
            CS460::Matrix33 Rotation(const EulerAngle& euler_angle);
            CS460::Matrix33 Rotation(const AxisRadian& axis_radian);

            //principle rotation
            CS460::Matrix33 RotationX(Real radian);
            CS460::Matrix33 RotationY(Real radian);
            CS460::Matrix33 RotationZ(Real radian);

            //scale
            CS460::Matrix33 Scale(const CS460::Vector3& scale);
        }
    }
}
