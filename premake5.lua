workspace "snAIke"
    location "code"
    configurations { "Debug", "Release" }
    architecture "x64"
    platforms { "Win64" }

    flags { "MultiProcessorCompile" , "FatalWarnings", "NoPCH"}

    CPPDIALECT = "C++17"

    TARGET_DIR = "%{wks.location}/../bin/%{cfg.buildcfg}-%{cfg.platform}-%{cfg.architecture}"
    OUTPUT_DIR = "%{wks.location}/../bin/obj/%{cfg.buildcfg}-%{cfg.platform}-%{cfg.architecture}"

    INCLUDE_PATHS = 
    {
        SFML = "%{wks.location}/SFML/include",
        ROOT = "%{wks.location}"
    }

    LIB_PATHS = 
    {
        SFML = "%{wks.location}/SFML/build/lib/%{cfg.buildcfg}",
        SFML_EXTLIB = ""
    }

    filter "action:vs*"
        LIB_PATHS.SFML_EXTLIB = "%{wks.location}/SFML/extlibs/libs-msvc/x64"

    filter "system:windows"
        systemversion "latest"

    include "code/snAIke"
