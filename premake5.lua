workspace "MirrorGame"
    architecture "x86" --"x64" --"x86"
    startproject "LandUnknown"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

    outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

--includes the premake files
include "Library/ImGUI"
include "Mirror"

project "LandUnknown"
    location "LandUnknown"
    kind "ConsoleApp"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-obj/" .. outputdir .. "/%{prj.name}")


    files 
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.c",
        "%{prj.name}/src/**.hpp",
        "%{prj.name}/src/**.cpp",
    }
    
    includedirs
    {
        
        "Library/SmokCore/Library/glm/glm",
        "Library/SmokCore/Library/Glfix/includes",
        "Library/SmokCore/Library/GLFW/include",
        "Library/SmokCore/Library/FastDelegate",
        "Library/SmokCore/Library/entt/src",
        "Library/SmokCore/Library",
        "Mirror/includes",
        "%{prj.name}/src",
    }
    
    links
    {
        --"Glfix"
        "Mirror",
    }

    defines
    {
        "GLFW_STATIC",
        "GLFW_INCLUDE_NONE",
        "GLM_FORCE_RADIANS",
        "GLM_FORCE_DEPTH_ZERO_TO_ONE",
    }

    flags
    {
        "MultiProcessorCompile"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"
    
        defines
        {
            "Window_Build",
            "Desktop_Build"
        }
    
    filter "configurations:Debug"
        defines "SMOK_DEBUG"
        symbols "On"
    
    filter "configurations:Release"
        defines "SMOK_RELEASE"
        optimize "On"
    
    filter "configurations:Dist"
        defines "SMOK_DIST"
        optimize "On"

        removefiles
        {
            "%{prj.name}/src/Widgets/**.h",
            "%{prj.name}/src/Widgets/**.c",
            "%{prj.name}/src/Widgets/**.hpp",
            "%{prj.name}/src/Widgets/**.cpp",
        }

--project "FloopyBird"
--    location "FloopyBird"
--    kind "ConsoleApp"
--    language "C++"
--
--    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
--    objdir ("bin-obj/" .. outputdir .. "/%{prj.name}")
--
--
--    files 
--    {
--        "%{prj.name}/src/**.h",
--        "%{prj.name}/src/**.c",
--        "%{prj.name}/src/**.hpp",
--        "%{prj.name}/src/**.cpp",
--    }
--    
--    includedirs
--    {
--        
--        "Library/glm/glm",
--        "Library/Glfix/includes",
--        "Library/GLFW/include",
--        "Library/entt/src",
--        "Library/SmokCore/includes",
--        "Mirror/includes",
--        "%{prj.name}/src",
--        
--    }
--    
--    links
--    {
--        --"Glfix"
--        "Mirror",
--    }
--
--    defines
--    {
--        "GLFW_STATIC",
--        "GLFW_INCLUDE_NONE",
--        "GLM_FORCE_RADIANS",
--        "GLM_FORCE_DEPTH_ZERO_TO_ONE",
--    }
--
--    flags
--    {
--        "MultiProcessorCompile"
--    }
--
--    filter "system:windows"
--        cppdialect "C++17"
--        staticruntime "On"
--        systemversion "latest"
--    
--        defines
--        {
--            "Window_Build",
--            "Desktop_Build"
--        }
--    
--    filter "configurations:Debug"
--        defines "SMOK_DEBUG"
--        symbols "On"
--    
--    filter "configurations:Release"
--        defines "SMOK_RELEASE"
--        optimize "On"
--    
--    filter "configurations:Dist"
--        defines "SMOK_DIST"
--        optimize "On"
--
--        removefiles
--        {
--            "%{prj.name}/src/Widgets/**.h",
--            "%{prj.name}/src/Widgets/**.c",
--            "%{prj.name}/src/Widgets/**.hpp",
--            "%{prj.name}/src/Widgets/**.cpp",
--        }