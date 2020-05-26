workspace "snAIke"
    location "code"
    configurations { "Debug", "Release", "Dist" }
    architecture "x64"
    platforms { "Win64" }
    startproject "snAIke"

    flags { "MultiProcessorCompile" , "FatalWarnings", "NoPCH"}

    CPPDIALECT = "C++17"

    TARGET_DIR = "%{wks.location}/../bin/%{cfg.buildcfg}-%{cfg.platform}-%{cfg.architecture}"
    OUTPUT_DIR = "%{wks.location}/../bin/obj/%{cfg.buildcfg}-%{cfg.platform}-%{cfg.architecture}"

    INCLUDE_PATHS = 
    {
        SFML = "%{wks.location}/SFML/include",
        ROOT = "%{wks.location}",
        IMGUI = "%{wks.location}/ImGui/imgui",
        IMGUI_SFML = "%{wks.location}/ImGui/imgui-sfml"
    }

    LIB_PATHS = 
    {
        SFML = "%{wks.location}/SFML/build/lib/%{cfg.buildcfg}",
        SFML_EXTLIB = "",

        SFML_DIST = "%{wks.location}/SFML/build/lib/Release"
    }

    filter "action:vs*"
        LIB_PATHS.SFML_EXTLIB = "%{wks.location}/SFML/extlibs/libs-msvc/x64"
        buildoptions {"/Zc:__cplusplus"}

    filter "system:windows"
        systemversion "latest"

    include "code/imgui"
    include "code/snAIke"
