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
    Press Mouse LB       - Make point on platform (it will be control point of path)

## Project 4 - Physically Based Simulation

### Info
- Model : There was no suitable model using data loader. 
          (previous salamander model contain max 4 links manipulator)
- Link : 7 link


## Component Control
    Create New Path     - Button to make new Path for skeleton.
    Create New COI      - Button to make new COI point for skeleton.
    COI Mode            - Combo Box for select Orbit mode or Forward mode
    Select Current Path - Combo Box for select path.
    Path Duration       - Duration time it takes to proceed with the path. This value is normalized [0:1]. 
    T1                  - Slider for change t1 parameter of ease in velocity.
    T2                  - Slider for change t2 parameter of ease out velocity. 

    Press Mouse LB button to create path on current root to target object
    Press Mouse LB & Hold Left Shift to extend path on platform

### Algorithm Description
    Jacobian Matrix Solution per frame
    while (dist(p_dest, p_curr) > threshold && CF > 0) 
    1. obtain first derivatives from links to construct jacobian matrix
        => J(q), but matrix J(q) has problems : Invertibility, Complexity, Redundancy, Singularity
    2. use pseudo inverse jacobian
        => J(q)^+ = J(q)^t * [J(q) * J(q)^t]^-1
    3. calculate velocity of position 
        => p_dot = (p_dest - p_curr)/CF;
    4. calculate velocity of angle params 
        => q_dot = J(q)^+ x p_dot;
    5. integrate and apply q_dot to q
        => q = q + q_dot / dt;
    6. do forward kinematics to obtain new p_curr
        => p_curr = forward(q);
    7. decrease converge frames
        => CF--;
    8. update links and ready for render
        => Update(links); Draw(links);


### Relevant Source Codes
- GUI support & Data holder
    Source/Manager/Component/EngineComponent/IKComponent.cpp
    Source/Manager/Component/EngineComponent/IKComponent.hpp

- Inverse Kinematics structure
    Source/System/Animation/InverseKinematics/Link.cpp
    Source/System/Animation/InverseKinematics/Link.hpp
    Source/System/Animation/InverseKinematics/Manipulator.cpp
    Source/System/Animation/InverseKinematics/Manipulator.hpp

- Path Interpolation & Arc Length Calculation:
    Source/System/Animation/Space/AnimationSpace.cpp
    Source/System/Animation/Space/AnimationSpace.hpp
    Source/System/Animation/Space/SpacePath.cpp
    Source/System/Animation/Space/SpacePath.hpp
    Source/System/Math/Curve/BezierCurve.cpp
    Source/System/Math/Curve/BezierCurve.hpp

- Speed & Orientation Control:
    Source/System/Animation/Skeleton/OrientationController.cpp
    Source/System/Animation/Skeleton/OrientationController.hpp    
    Source/System/Animation/Skeleton/SpeedController.hpp   
    Source/System/Animation/Skeleton/SpeedController.hpp   