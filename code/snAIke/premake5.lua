project "snAIke"
    location ""
    language "C++"
    cppdialect (CPPDIALECT)
    targetdir (TARGET_DIR)
    objdir (OUTPUT_DIR .. "/%{prj.name}")

    files { "**.hpp", "**.cpp" }
    removefiles {"imgui/impl/*"}

    includedirs
    {
        INCLUDE_PATHS.ROOT,
        INCLUDE_PATHS.SFML,
        INCLUDE_PATHS.IMGUI,
        INCLUDE_PATHS.IMGUI_SFML
    }

    libdirs
    {
        LIB_PATHS.SFML,
        LIB_PATHS.SFML_EXTLIB
    }

    links
    {
        "opengl32",
        "freetype",
        "winmm",
        "gdi32",
        "flac",
        "vorbisenc",
        "vorbisfile",
        "vorbis",
        "ogg",
        "ws2_32",
    }

    defines 
    {
        "IMGUI_USER_CONFIG=<imconfig-SFML.h>",
        "SFML_STATIC",
        "RES_EXTENTION=\".png\""
    }

    links { "imGui" }

    filter {"configurations:Debug or Release"}
        defines
        {
            "RES_PATH=\"../../data\""
        }


    filter "configurations:Debug"
        kind "ConsoleApp"
        defines { "DEBUG" }
        symbols "On"
        runtime "Debug"

        links 
        {
            "sfml-graphics-s-d",
            "sfml-window-s-d",
            "sfml-system-s-d",
            "sfml-audio-s-d",
            "sfml-network-s-d"
        }

    filter "configurations:Release or Dist"
        kind "WindowedApp"
        defines { "NDEBUG"}
        optimize "On"
        runtime "Release"

        links
        {
            "sfml-graphics-s",
            "sfml-window-s",
            "sfml-system-s",
            "sfml-audio-s",
            "sfml-network-s",
            "sfml-main"
        }

    filter "configurations:Release"
        defines { "RELEASE" }

    filter "configurations:Dist"
        targetdir "%{wks.location}/../dist"
        libdirs { LIB_PATHS.SFML_DIST }
        debugcommand ""
        defines
        {
            "DIST",
            "RES_PATH=\"data\""
        }
        postbuildcommands
        {
            "{COPY} %{wks.location}../data %{cfg.buildtarget.directory}/data"
        }