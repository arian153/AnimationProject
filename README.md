# AnimationProject
## Assumption
You must place the resource folder in the parent directory twice than the executable. 
like "../../Resource". 

i.e)
Project/Bin/Debug/Animation-Projectd.exe
Project/Resource/..
Project/Source/..

## Build and Recompile
This framework build using visual studio 2019 and c++17,  also it depends on win32 and DirectX11.
Use solution file to recompile, rebuild it.

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

## Project 2 - Motion along a Path

### Info
- Model : Resource/AniMesh/Salamander.bin
- Bone : 17 bones are in model
- Animation Clip : 3 aniamtion clips
- clip 0 : 21 keyframes
- clip 1 : 41 keyframes
- clip 2 : 21 keyframes

## Component Control
    Create New Path     - Button to make new Path for skeleton.
    Create New COI      - Button to make new COI point for skeleton.
    COI Mode            - Combo Box for select Orbit mode or Forward mode
    Select Current Path - Combo Box for select path.
    Path Duration       - Duration time it takes to proceed with the path. This value is normalized [0:1]. 
    T1                  - Slider for change t1 parameter of ease in velocity.
    T2                  - Slider for change t2 parameter of ease out velocity. 

    You can edit these GUI feature to adjust sliding and skidding control.
    Path duration, T1, T2.
    It generate v(t), and this is a animation speed of clip following.

### Algorithm Description
 - Space Curve Algorithm  : Multi-Segment of Cubic Bezier Curve
 - Arc Length Function    : Adaptive Method approach, using std::vector for table, but apply binary search to increase query speed.  
 - Distance-Time Function : Ease in/out velocity function
 - Orientation Control    : COI apporach, using Orbit mode & Forward mode.


### Relevant Source Codes
- GUI support
    Source/Manager/Component/EngineComponent/AniMeshComponent.cpp
    Source/Manager/Component/EngineComponent/AniMeshComponent.hpp

- Skeleton structure
    Source/System/Animation/Skeleton/Skeleton.cpp
    Source/System/Animation/Skeleton/Skeleton.hpp

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