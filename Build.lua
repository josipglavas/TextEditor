-- premake5.lua
workspace "Text Editor"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject "TextEditor-App"

   -- Workspace-wide build options for MSVC
   filter "system:windows"
      buildoptions { "/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus" }

OutputDir = "%{cfg.system}-%{cfg.architecture}/%{cfg.buildcfg}"

group "Core"
	include "TextEditor-Core/Build-Core.lua"
   include "TextEditor-Core/Vendor/ImGui/premake5.lua"
group ""

include "TextEditor-App/Build-App.lua"
