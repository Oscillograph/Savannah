#ifndef SAVANNAH_MAINAPP_H
#define SAVANNAH_MAINAPP_H

#include <savannah/core.h>

// Dear ImGui: standalone example application for GLFW + OpenGL 3, using programmable pipeline
// (GLFW is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan/Metal graphics context creation, etc.)
// If you are new to Dear ImGui, read documentation from the docs/ folder + read the top of imgui.cpp.
// Read online: https://github.com/ocornut/imgui/tree/master/docs

#ifndef IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#define IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#endif

#include <external/imgui/imgui.h>
#include <external/imgui/imgui_internal.h> // for advanced selectable flags
#include <external/imgui/misc/cpp/imgui_stdlib.h> // for strings

#include <external/imgui/backends/imgui_impl_glfw.h>
#include <external/imgui/backends/imgui_impl_opengl3.h>

// #include <savannah/utils/dearimgui-backend.h>
#include <savannah/platforms/opengl/opengl_window.h> // open, process and close windows on Windows
#include <savannah/platforms/opengl/opengl_texture.h> // probably, it's better to make a separate renderer module

// events
#include <savannah/systems/events/event_app.h>
#include <savannah/systems/events/event_key.h>
#include <savannah/systems/events/event_mouse.h>

#define GL_SILENCE_DEPRECATION

static void glfw_error_callback(int error, const char* description)
{
	fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

namespace Savannah {
	class App {
	public:
		App();
		virtual ~App();
		virtual void Init();
		virtual void PreSetup();
		virtual void SetupWindow();
		virtual void SetupResources();
		virtual void SetupGUI();
		virtual Window* GetWindow();
		virtual void ProcessInput();
		virtual void Logic();
		virtual void Render();
		virtual void GUIBegin();
		virtual void GUIEnd();
		virtual void GUIContent();
		virtual void ShutDown();
		virtual void SetFPS(float fps);
		virtual float GetFPS();
		virtual int GetIdleFrames();
		
		bool doExit = false;
		
	protected:
		ImFontConfig* font_config = nullptr;
		Window* m_Window = nullptr;
		WindowStack m_WindowStack;
		ImVec4 m_ClearColor = {0.0f, 0.0f, 0.0f, 0.0f};
		std::string m_WindowTitle = "Превед, мир!";
		const char* m_glsl_version = "#version 410";
		bool m_WindowMinimized = false;
		float FPS = SAVANNAH_FPS60;
		int m_IdleFrames = 0;
		
	protected:
		void SetWindowTitle(const std::string& title);
		
	protected:
		void OnEvent(Event& event);
		
		void UpdateWindowMinimizedStatus();
		void OnWindowMinimized();
		void OnWindowRestored();
		bool IsWindowMinimized();
		bool OnWindowResized(WindowResizeEvent& event);
		bool OnMouseClicked(MouseButtonPressed& event);
	};
	
	App* CreateApplication();
	
}

#endif
