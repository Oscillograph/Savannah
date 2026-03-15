# ! WORK IN PROGRESS ! #

*Savannah* is a Windows/Linux application framework built on OpenGL & Dear ImGui.

## Features ##
- Window abstraction
- Texure load-unload
- GUI construction based on Dear ImGui

## Build notes ##
This framework is meant to be used as a static library with:
- C++ 17
- OpenGL 3.3+

commands:

```
cmake -S . -B build
cd build
make
make install
```

The resulting static library will be sent to the "out" directory in the source root.
Make sure you carry "data" folder from the source root along with your Savannah projects as this folder contains fonts and images necessary for the framework to run properly. Lack of this folder may result in program crash at startup when ImGui subsystem tries to load a font and fails to find it nearby.


## Developer's Tech Stack for Savannah ##
- C++ 17
- CMake
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
