# AnimationProject
## Assumption
You must place the resource folder in the parent directory twice than the executable. 
like "../../Resource". 

i.e)
Project/Bin/Debug/Animation-Projectd.exe
Project/Resource/..
Project/Source/..

## Build and Recompile
This framework build using visual studio 2019 and c++17, also it depends on win32 and DirectX11.
Use vs solution file to recompile, rebuild it.

## Execution
Execute Animation-Project.exe in "Bin/Release/Animation-Project.exe"

### Usage
1. Click File Tab, and Open Space Project#.
2. If you open project file, framework show objects Hierarchy.
3. Select object in Hierarchy window.
4. Now you can edit.

## Camera Control
    Hold Mouse RB & move - Change camera view direction on spherically");
    Roll Mouse Wheel     - Change spherical camera's radius on same view");
    Press W, S           - Change camera position Front & Back");
    Press A, D           - Change camera position Left & Right");
    Press R, F           - Change camera position Up & Down");

## Project 4 - Physically Based Simulation

### Info
- Project4A-Spring Mass Damper (Option A : 6 sticks and 2 anchor points connected by springs)
- Project4A-SMD+Texture (Option A : 6 sticks and 2 anchor points connected by springs, and Texturing, Lighting, Rotating Kinematic Object)
- Project4D-Hard Pendulum (Option D : 7 pendulum(ghost collider) connected by Hard String)
- Project4D-Soft Pendulum (Option D : 6 pendulum connected by Soft(allowed to stretch) String)

## Component Control
    Press Mouse LB & Drag mouse to move selected red anchor point

### Algorithm Description

    Physics Engine Loop:
    1. Do Broad Phase Collision Detection
        1-1. Update Spatial Data Structure (reinsert dynamic object)
        1-2. Compute Potential Pairs
    2. Do Narrow Phase Collision Detection
        2-1. Filtering Contact Manifolds (if collision was resolved, remove it)
        2-2. Generate Contact Point (Specify Contact Point with GJK+EPA)
    3. Do Resolution Phase
        3-1. Apply External(Global) Forces (Gravity, Drag, ETC...)
        3-2. Process Contact Constraints (Generate Contact Constrains from Contact Manifold)
        3-3. Solve Velocity Constraints (Solve contact, spring-damper, ETC...)
        3-4. Integrate Rigid Bodies (Using Numerical Integrate Methods)
        3-5. Solve Position Constrains (if it needs)

    Rigid Body
    Integrator : Semi Implicit Euler Method
        (Other integrators were also drafted, but did not apply them because there was no time to modify the physics engine loop.)
    
    Data
        linear velocity    (velocity holder)
        force accumulator  (total force per frame)
        linear constraints (linear DOF)
        inverse orientation (inv orientation holder)
        angular velocity
        torque accumulator
        angular constraints (angular DOF)
        mass
        inverse mass
        local centroid
        local inertia tensor
        local inverse inertia tensor
        global centroid
        global inertia tensor
        global inverse inertia tensor

        and other data...
        # "m_local" is a shareable transform data.
        

    Spring Constraint: 
    Basic Idea from Class Notes.
        K (Spring Constant)
        D (Damper Constant)
        q_a : spring end point which is connected to rigid body a
        q_b : spring end point which is connected to rigid body b 
        v_a : rigid body a's velocity
        v_b : rigid body b's velocity

        Spring Force 
        f_a = K * (q_b - q_a) + D * (v_b - v_a);
        f_b = K * (q_a - q_b) + D * (v_a - v_b); = -f_a;

        Apply Force on centroid
        Apply Torque on q_a, q_b;

        ApplyForce(force, at): Apply Force & Torque both.
            force_accumulator += force;
            torque_accumulator += (at - m_global_centroid).CrossProduct(force);

    Point Constraint: 
    Basic Idea from 
        Erin catto's GDC 2011 Soft Constraints Slides.
        Ming-Lun Chou's Soft Constraints video.
    
    1. Generate Coefficients & Constraints
        1-1. Gathering Frame Info
            m (mass), h (time step)
            DR (Damping Ratio) : (given value)
            Freq (Frequency)   : (given value)
        1-2. Generate DC & SC
            DC (Damping Coefficient) = 2 * m * DR * Freq;
            SC (Spring Coefficient)  = m * Freq * Freq;
        1-3. Generate Beta & Gamma
            Beta  (Positional Error Factor) = hk/(d+hk);
            Gamma (Softness Factor)         = 1/(d+hk);

    2. Solve constraints
        M (mass matrix), I (identity matrix)
        J*V2 + Beta/timestep * C + Gamma/timestep * Lambda = 0;
        V2 = V1 + invM*transJ*Lambda;
        J(V1 + invM*transJ*Lambda) + Beta/timestep * C + Gamma/timestep * Lambda = 0
        Lambda = inv(J*invM*transJ + Gamma/timestep * I) * (-(J*V1 + Beta/timestep * C));
        also set effM(effective matrix) = inv(J*invM*transJ + Gamma/timestep * I);

    3. Soft vs Hard
        Hard : Beta = 1.0, Gamma = 0.0;
        Soft : Beta&Gamma is calculated by Constriants Utility


### Relevant Source Codes
- GUI support & Data holder
    Source/Manager/Component/EngineComponent/AnchorComponent.cpp
    Source/Manager/Component/EngineComponent/AnchorComponent.hpp
    Source/Manager/Component/EngineComponent/PointConstraintComponent.cpp
    Source/Manager/Component/EngineComponent/PointConstraintComponent.hpp
    Source/Manager/Component/EngineComponent/SpringConstraintComponent.cpp
    Source/Manager/Component/EngineComponent/SpringConstraintComponent.hpp

- Dynamics (Rigid Body, Mass Data, ETC) : World has a physics loop
    Source/System/Physics/Dynamics/RigidBody.cpp
    Source/System/Physics/Dynamics/RigidBody.hpp
    Source/System/Physics/Dynamics/MassData.cpp
    Source/System/Physics/Dynamics/MassData.hpp
    Source/System/Physics/Dynamics/World.cpp
    Source/System/Physics/Dynamics/World.hpp

- Collider Shape (Box, Sphere, ETC...) : in this Project Only Use Collider Box
    Source/System/Physics/ColliderPrimitive/ConvexHull3D/ColliderBox.cpp
    Source/System/Physics/ColliderPrimitive/ConvexHull3D/ColliderBox.hpp

- Constraints & Resolution Phase:
    Source/System/Physics/Resolution/Constraints/SpringConstraint.cpp
    Source/System/Physics/Resolution/Constraints/SpringConstraint.hpp
    Source/System/Physics/Resolution/Constraints/PointConstraint.cpp
    Source/System/Physics/Resolution/Constraints/PointConstraint.hpp
    Source/System/Physics/Resolution/Force/Drag.cpp
    Source/System/Physics/Resolution/Force/Drag.hpp
    Source/System/Physics/Resolution/Force/Gravity.cpp
    Source/System/Physics/Resolution/Force/Gravity.hpp
    Source/System/Physics/Utility/ConstraintUtility.cpp
    Source/System/Physics/Utility/ConstraintUtility.hpp
