#pragma once

namespace CS460
{
    enum class eColliderType : int
    {
        Circle,
        Ellipse,
        Polygon,
        Rectangle,
        Triangle,
        Box,
        Capsule,
        Cone,
        Cylinder,
        Dome,
        Ellipsoid,
        Polyhedron,
        Sphere,
        Tetrahedron,
        Truncated,
        InvalidPrimitive
    };

    enum class eMotionMode : int
    {
        Dynamic,
        Static,
        Kinematic
    };

    enum class eBroadPhaseMode : int
    {
        DynamicBVH,
        NSquared,
        GridPartition
    };

    enum class eCollisionDetectionMode : int
    {
        Discrete,
        ContinuousStatic,
        ContinuousFull
    };

    namespace Physics
    {
        namespace Dynamics
        {
            const Real ELASTIC_THRESHOLD = 1.0f;
            const Real BAUMGRATE         = 0.2f;
        }

        namespace Collision
        {
            const Real   BROAD_PHASE_MARGIN           = 0.2f;
            const Real   PERSISTENT_THRESHOLD         = 0.2f;
            const Real   PERSISTENT_THRESHOLD_SQUARED = PERSISTENT_THRESHOLD * PERSISTENT_THRESHOLD;
            const size_t MAX_MANIFOLD_POINT_COUNT     = 4;

            const Real CONTACT_BETA  = 0.55f;
            const Real VELOCITY_SLOP = 0.1f;

            const Real POSITION_LINEAR_SLOP     = 0.05f;
            const Real POSITION_ANGULAR_SLOP    = Math::RADIAN * 2.0f;
            const Real MAX_LINEAR_CORRECTION    = 0.2f;
            const Real POSITION_SEPARATION_SLOP = 0.02f;

            const Real SLEEP_THRESHOLD         = 0.0007f;
            const Real SLEEP_THRESHOLD_EXTREME = SLEEP_THRESHOLD * 100.0f;
            const Real SLEEP_AWAKE             = SLEEP_THRESHOLD * 33.0f;
            const Real SLEEP_BIAS              = 0.75f;
        }

        namespace Primitive
        {
            const Real BOUNDING_VOLUME_MARGIN = 0.05f;
        }
    }
}
