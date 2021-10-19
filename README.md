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
    Hold Mouse LB & move - Change camera view direction on spherically");
    Roll Mouse Wheel     - Change spherical camera's radius on same view");
    Press W, S           - Change camera position Front & Back");
    Press A, D           - Change camera position Left & Right");
    Press R, F           - Change camera position Up & Down");

## Project 1 - Hierarchical Modeling and Animation
### Info
- Model : Resource/AniMesh/Salamander.bin
- Bone : 17 bones are in model
- Animation Clip : 3 aniamtion clips
- clip 0 : 21 keyframes
- clip 1 : 41 keyframes
- clip 2 : 21 keyframes

### Interpolation Algorithm 

- SLerp : quaternion interpolation
- ELerp : scale interpolation
- Lerp : vector interpolation

### Relevant Source Codes
- GUI support
File     - Source/Manager/Component/EngineComponent/AniMeshComponent.cpp
File     - Source/Manager/Component/EngineComponent/AniMeshComponent.hpp

- Skeleton structure
File     - Source/System/Animation/Skeleton/Skeleton.cpp
File     - Source/System/Animation/Skeleton/Skeleton.hpp
File     - Source/System/Animation/Skeleton/Bone.cpp
File     - Source/System/Animation/Skeleton/Bone.hpp

- Animation Clip & Tracks
File     - Source/System/Animation/AnimationClip/AnimationClip.cpp
File     - Source/System/Animation/AnimationClip/AnimationClip.hpp
File     - Source/System/Animation/AnimationClip/KeyFrame.hpp
File     - Source/System/Animation/AnimationClip/Track.cpp
File     - Source/System/Animation/AnimationClip/Track.hpp

- VQS Structure
File     - Source/System/Math/Structure/VQSTransform.cpp
File     - Source/System/Math/Structure/VQSTransform.hpp

- Quaternion, SLerp
File     - Source/System/Math/Algebra/Quaternion.cpp
File     - Source/System/Math/Algebra/Quaternion.hpp

- Lerp, ELerp
File     - Source/System/Math/Utility/Interpolation.cpp
File     - Source/System/Math/Utility/Interpolation.hpp