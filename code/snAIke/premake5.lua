project "snAIke"
    location ""
    language "C++"
    cppdialect (CPPDIALECT)
    targetdir (TARGET_DIR)
    objdir (OUTPUT_DIR .. "/%{prj.name}")

    files { "**.hpp", "**.cpp" }

    includedirs
    {
        INCLUDE_PATHS.ROOT,
        INCLUDE_PATHS.SFML
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

    defines { "SFML_STATIC" }

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

    filter "configurations:Release"
        kind "WindowedApp"
        defines { "NDEBUG", "RELEASE" }
        optimize "On"
        runtime "Release"

        links
        {
            "sfml-graphics-s",
            "sfml-window-s",
            "sfml-system-s",
            "sfml-audio-s",
            "sfml-network-s",
            "sfml-main-s"
        }