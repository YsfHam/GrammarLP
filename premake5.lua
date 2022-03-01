workspace "GrammarLP"

architecture "x64"
configurations
{
  "Debug",
  "Release"
}

project "GrammarLP"

  kind "ConsoleApp"
  language "C++"
  cppdialect "c++17"
  location "%{prj.name}"
  targetdir "bin/bin/%{cfg.name}_%{cfg.architecture}"
  objdir "bin/obj/%{cfg.name}_%{cfg.architecture}"
  files
  {
      "%{prj.name}/src/**.cpp"
  }
filter "configurations:Debug"
  symbols "On"
filter "configurations:Release"
  optimize "On"
