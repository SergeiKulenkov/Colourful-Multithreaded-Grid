project "Colourful Multithreaded Grid"
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++20"
   targetdir "Binaries/%{cfg.buildcfg}"
   staticruntime "off"

   files { "Source/**.h", "Source/**.cpp", "Vendor/ImGUI/**.h", "Vendor/ImGUI/**.cpp" }

   includedirs
   {
      "Source",
      "Vendor/glfw/include",
      "Vendor/glm",
      "Vendor/ImGUI",
      "%{IncludeDir.VulkanSDK}",
      "%{IncludeDir.glm}",
   }

   links
   {
      "Vendor/glfw/lib/glfw3.lib",
      "%{Library.Vulkan}",
   }

   targetdir ("Binaries/" .. OutputDir .. "/%{prj.name}")
   objdir ("Binaries/Intermediates/" .. OutputDir .. "/%{prj.name}")

   filter "system:windows"
       systemversion "latest"
       defines { }

   filter "configurations:Debug"
       defines { "DEBUG" }
       runtime "Debug"
       symbols "On"

   filter "configurations:Release"
       defines { "RELEASE" }
       runtime "Release"
       optimize "On"
       symbols "On"

   filter "configurations:Dist"
       defines { "DIST" }
       runtime "Release"
       optimize "On"
       symbols "Off"