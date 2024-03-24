# ! WORK IN PROGRESS ! #

*Savannah* is a Windows application framework built on OpenGL & Dear ImGui.

## Features ##
- Window abstraction
- Texure load-unload
- GUI construction based on Dear ImGui

## Build notes ##
This framework is meant to be used with:
- C++ 17
- TDM-GCC 10.3
- OpenGL 3.3-4.1

Link static libraries:
+ -lopengl32
+ bin/libglad.a
+ bin/libglfw3dll.a
+ bin/libDear_ImGui.a
+ bin/libYaml_cpp.a
Define variables:
+ SAVANNAH_PLATFORM_WINDOWS 
+ SAVANNAH_DEBUGGER

## Developer's Tech Stack for Savannah ##
- C++ 17
- TDM GCC 10.3
- Dear ImGui 1.89.7 (Docking branch)
- color console
- GLFW
- GLAD
- Yaml-Cpp
- JSON for Modern C++ v3.11.2
- Utf-8 for Cpp
- spdlog

## Examples ##
Proforientator -- a tool to store, evaluate and analize user's skills.
<img src="https://raw.githubusercontent.com/Oscillograph/Savannah/main/examples/proforientator/screenshot.png" alt="Профориентатор" width="400"/>
