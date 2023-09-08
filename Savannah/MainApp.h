#ifndef SAVANNAH_MAINAPP_H
#define SAVANNAH_MAINAPP_H

#include <Savannah/Core.h>

// Dear ImGui: standalone example application for GLFW + OpenGL 3, using programmable pipeline
// (GLFW is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan/Metal graphics context creation, etc.)
// If you are new to Dear ImGui, read documentation from the docs/ folder + read the top of imgui.cpp.
// Read online: https://github.com/ocornut/imgui/tree/master/docs

#include <stdio.h>

#include <Savannah/Vendor/Glad/include/glad/glad.h>
#include <Savannah/Vendor/glfw/glfw3.h> // Will drag system OpenGL headers

#ifndef IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#define IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#endif

#include <Savannah/Vendor/imgui/imgui.h>
#include <Savannah/Vendor/imgui/misc/cpp/imgui_stdlib.h> // for strings

#include <Savannah/Vendor/imgui/backends/imgui_impl_glfw.h>
#include <Savannah/Vendor/imgui/backends/imgui_impl_opengl3.h>

// #include <Savannah/Utils/DearImGuiBackend.h>

#define GL_SILENCE_DEPRECATION

static void glfw_error_callback(int error, const char* description)
{
	fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

namespace Savannah {
	class App {
	public:
		App(){}
		virtual ~App(){}
		
		virtual void Init(){
			SetupWindow();
			SetupGUI();
		}
		
		virtual void SetupWindow(){
			glfwSetErrorCallback(glfw_error_callback);
			if (!glfwInit())
				return;
			
			// Decide GL+GLSL versions
			// GL 3.0 + GLSL 130
			// const char* m_glsl_version = "#version 410";
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
			//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
			//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
			
			// Create window with graphics context
			// glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
			m_Window = glfwCreateWindow(1280, 720, "Превед, мир!", nullptr, nullptr);
			if (m_Window == nullptr)
				return;
			glfwMakeContextCurrent(m_Window);
			gladLoadGL();
			glfwSwapInterval(1); // Enable vsync
		}
		
		virtual void SetupGUI(){
			// Setup Dear ImGui context
			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO(); // (void)io;
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
			io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
			
			io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
			io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
			
			// Setup Dear ImGui style
			ImGui::StyleColorsDark();
			//ImGui::StyleColorsLight();
			
			// Setup Platform/Renderer backends
			ImGui_ImplOpenGL3_Init(m_glsl_version);
			ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
			
			// load font
			if (font_config == nullptr){
				font_config = new ImFontConfig();
			}
			font_config->OversampleH = 1; //or 2 is the same
			font_config->OversampleV = 1;
			font_config->PixelSnapH = 1;
			io.Fonts->AddFontFromFileTTF("./Savannah/Assets/Fonts/tahoma.ttf", 18.0f, font_config, io.Fonts->GetGlyphRangesCyrillic());
			;
		}
		
		virtual GLFWwindow* GetWindow(){
			return m_Window;
		}
		
		virtual void ProcessInput(){
			// Poll and handle events (inputs, window resize, etc.)
			// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
			// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
			// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
			// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
			glfwPollEvents();
		}
		
		virtual void Render(){
			GUIBegin();
			GUIContent();
			GUIEnd();
		}
		
		virtual void GUIBegin(){
			// Start the Dear ImGui frame
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
		}
		
		virtual void GUIEnd(){
			// Rendering
			ImGui::Render();
			int display_w, display_h;
			glfwGetFramebufferSize(m_Window, &display_w, &display_h);
			// io.DisplaySize = ImVec2((float)display_w, (float)display_h);
			glViewport(0, 0, (GLsizei)display_w, (GLsizei)display_h);
			glClearColor(m_ClearColor.x * m_ClearColor.w, m_ClearColor.y * m_ClearColor.w, m_ClearColor.z * m_ClearColor.w, m_ClearColor.w);
			glClear(GL_COLOR_BUFFER_BIT);
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
			
			ImGuiIO& io = ImGui::GetIO();
			if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable){
				GLFWwindow* window_backup_context = glfwGetCurrentContext();
				ImGui::UpdatePlatformWindows();
				ImGui::RenderPlatformWindowsDefault();
				glfwMakeContextCurrent(window_backup_context);
			}
			
			// 
			glfwSwapBuffers(m_Window);
		}
		
		virtual void GUIContent(){
			// Our state
			bool show_demo_window = true;
			bool show_another_window = false;
			m_ClearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
			
			// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
			if (show_demo_window)
				ImGui::ShowDemoWindow(&show_demo_window);
			// 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
			{
				ImGuiIO& io = ImGui::GetIO();
				static float f = 0.0f;
				static int counter = 0;
				
				ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
				
				ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
				ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
				ImGui::Checkbox("Another Window", &show_another_window);
				
				ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
				ImGui::ColorEdit3("clear color", (float*)&m_ClearColor); // Edit 3 floats representing a color
				
				if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
					counter++;
				ImGui::SameLine();
				ImGui::Text("counter = %d", counter);
				
				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
				ImGui::End();
			}
			
			// 3. Show another simple window.
			if (show_another_window)
			{
				ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
				ImGui::Text("Hello from another window!");
				if (ImGui::Button("Close Me"))
					show_another_window = false;
				ImGui::End();
			}
		}
		
		virtual void ShutDown(){
			// Cleanup
			ImGui_ImplOpenGL3_Shutdown();
			ImGui_ImplGlfw_Shutdown();
			ImGui::DestroyContext();
			
			glfwDestroyWindow(m_Window);
			glfwTerminate();
		}
		
		virtual float GetFPS(){ return FPS; }
		
		bool doExit = false;
		
	private:
		ImFontConfig* font_config = nullptr;
		GLFWwindow* m_Window = nullptr;
		ImVec4 m_ClearColor = {0.0f, 0.0f, 0.0f, 0.0f};
		const char* m_glsl_version = "#version 410";
		float FPS = SAVANNAH_FPS60;
	};
	
	App* CreateApplication();
	
}

#endif
