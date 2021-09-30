workspace "Animation-Project"
location ""
configurations {"Debug", "Release"}
platforms {"x64"}
filter {"platforms:x64"}
system "Windows"
architecture "x64"
filter {}

project "Animation-Project"
kind "WindowedApp"
language "C++"
location ""
targetname("Animation-Project")
targetdir "Bin/%{cfg.buildcfg}"
objdir "Obj/%{cfg.buildcfg}"
debugdir "Bin/%{cfg.buildcfg}"
systemversion "10.0"
flags {"MultiProcessorCompile", "NoMinimalRebuild", "NoBufferSecurityCheck"}
warnings "Extra"
includedirs {"Include", "Include/FBX"}
links {
    "dxgi", "d3d11", "d3dcompiler", "d2d1", "dwrite", "dsound", "dxguid",
    "winmm", "Xinput9_1_0", "libfbxsdk-md", "libxml2-md", "zlib-md"
}
libdirs {"Lib/%{cfg.buildcfg}/**"}
files {"**.h", "**.hpp", "**.cpp", "**.inl", "./Resource/**.json", "./Resource/**.shader"}
buildoptions {"-std:c++17"}
removefiles {"**Template.cpp"}
disablewarnings {
    "4505", "4456", "4201", "4100", "4189", "4458", "4819", "4127", "4701",
    "4291", "6385", "26451", "26812", "26495"
}
defines {"_CRT_SECURE_NO_WARNINGS", "WIN32_LEAN_AND_MEAN", "CS460_WIN32", "CS460_DIRECTX11", "IMGUI_INCLUDE_IMGUI_USER_H"}

filter "configurations:Debug"
defines {"DEBUG"}
symbols "On"
filter "configurations:Release"
defines {"NDEBUG"}
optimize "On"
filter {}

