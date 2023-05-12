workspace "hof"
   configurations { "Debug", "Release" }

project "hof"
   kind "ConsoleApp"

   language "C++"
   cppdialect "C++17"

   files {"*.h", "*.cpp", "*.hpp"}

   filter { "configurations:Debug" }
      defines { "DEBUG" }
      symbols "On"

   filter { "configurations:Release" }
      defines { "NDEBUG" }
      optimize "On"
