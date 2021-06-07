configurations
    {
        "Debug",
        "Release",
        "Dist",
    }

    outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

    include "../Library/SmokCore"

project "Mirror"
    kind "StaticLib"
    language "C++"

   targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-obj/" .. outputdir .. "/%{prj.name}")

    --pchheader "smpch.h"
    --pchsource "%{prj.name}/src/smpch.cpp"

    files 
    {
        "includes/**.h",
        "src/**.c",
        "includes/**.hpp",
        "src/**.cpp",
    }

    includedirs
    {
        
        "../Library/SmokCore/Library/glm/glm",
        "../Library/SmokCore/Library/Glfix/includes",
        "../Library/SmokCore/Library/GLFW/include",
        "../Library/SmokCore/Library/FastDelegate",
        "../Library/SmokCore/Library/entt/src",
        "../Library/SmokCore/Library",
        "includes",
        
    }
    
    links
    {
        "SmokCore"
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

    --stops C files from needing the PCH file
    filter "files:%{prj.name}/src/**.c"
    flags {"NoPCH"}

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
    
        --includes ImGUI
        includedirs
        {
            "../Library/Glfix/Library/GLAD/include",
            "../Library/ImGUI"
        }

        links
        {
            "ImGUI"
        }

    filter "configurations:Release"
        defines "SMOK_RELEASE"
        optimize "On"

         --includes ImGUI
         includedirs
         {
            "../Library/Glfix/Library/GLAD/include",
             "../Library/ImGUI"
         }
 
         links
         {
             "ImGUI"
         }
    
    filter "configurations:Dist"
        defines "SMOK_DIST"
        optimize "On"

        flags
        {
            "LinkTimeOptimization"
        }

        --excludes ImGUI
        defines {"Exclude_ImGUI"}

        removefiles
        {
            "src/Widgets/**.h",
            "src/Widgets/**.c",
            "src/Widgets/**.hpp",
            "src/Widgets/**.cpp",
        }