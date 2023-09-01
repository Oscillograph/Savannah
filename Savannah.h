#ifndef SAVANNAH_H
#define SAVANNAH_H

#include <Savannah/Core.h>
#include <Savannah/Core/Application/Application.h>
// #include <Savannah/Core/KeyCodes.h>

// Savannah systems
// - events system
// #include <Savannah/Systems/Events/Event.h>
// #include <Savannah/Systems/Events/AppEvent.h>
// #include <Savannah/Systems/Events/KeyEvent.h>
// #include <Savannah/Systems/Events/MouseEvent.h>

// - scenes
// #include <Savannah/Systems/Scene/Scene.h>
// #include <Savannah/Systems/Scene/SceneSerializer.h>

// - ecs
// #include <Savannah/Systems/ECS/Entity.h>
// #include <Savannah/Systems/ECS/Components.h>

// - utils
// #include <Savannah/Utils/Profiler.h>
// #include <Savannah/Utils/PlatformUtils.h> // modal windows

// entry point
#include <Savannah/MainLoop.h> // need to be included in Client main file

// Various Application data stored by Engine for Client App to use 
#include <Savannah/EngineData.h> // must be included last

#endif
