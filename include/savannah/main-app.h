#ifndef SAVANNAH_MAINAPP_H
#define SAVANNAH_MAINAPP_H

#include <savannah/core.h>

// Dear ImGui: standalone example application for GLFW + OpenGL 3, using programmable pipeline
// (GLFW is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan/Metal graphics context creation, etc.)
// If you are new to Dear ImGui, read documentation from the docs/ folder + read the top of imgui.cpp.
// Read online: https://github.com/ocornut/imgui/tree/master/docs

#include <stdio.h>

#include <external/glad/include/glad/glad.h>
#include <external/glfw/glfw3.h> // Will drag system OpenGL headers

#ifndef IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#define IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#endif

#include <external/imgui/imgui.h>
#include <external/imgui/misc/cpp/imgui_stdlib.h> // for strings

#include <external/imgui/backends/imgui_impl_glfw.h>
#include <external/imgui/backends/imgui_impl_opengl3.h>

// #include <savannah/utils/dearimgui-backend.h>

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
		virtual void SetupGUI();
		virtual GLFWwindow* GetWindow();
		virtual void ProcessInput();
		virtual void Render();
		virtual void GUIBegin();
		virtual void GUIEnd();
		virtual void GUIContent();
		virtual void ShutDown();
		virtual float GetFPS();
		
		bool doExit = false;
		
	protected:
		ImFontConfig* font_config = nullptr;
		GLFWwindow* m_Window = nullptr;
		ImVec4 m_ClearColor = {0.0f, 0.0f, 0.0f, 0.0f};
		std::string m_WindowTitle = "Превед, мир!";
		const char* m_glsl_version = "#version 410";
		float FPS = SAVANNAH_FPS60;
		
	protected:
		void SetWindowTitle(const std::string& title);
	};
	
	App* CreateApplication();
	
}

#endif
