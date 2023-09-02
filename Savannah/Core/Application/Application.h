#ifndef SAVANNAH_APPLICATION_H
#define SAVANNAH_APPLICATION_H

#include <Savannah/Core.h>

// #include <utils/Profiler.h>

#include <Savannah/Core/Application/Window.h>
// #include <Savannah/Input.h>
// #include <Savannah/KeyCodes.h>
// #include <Savannah/Systems/Events/AppEvent.h>
// #include <Savannah/Systems/Events/KeyEvent.h>
// #include <Savannah/Systems/Events/MouseEvent.h>
// #include <Savannah/Application/LayerStack.h>
// #include <Savannah/Systems/GUI/ImGuiLayer.h>


#ifdef SAVANNAH_PLATFORM_WINDOWS
	#include <Savannah/Platforms/Windows/WindowsWindow.h>
	// 	#include <platforms/windows/WindowsInput.cpp>
#endif
/*
#include <renderer/FrameBuffer.h>
#include <renderer/Shader.h>
#include <renderer/Buffer.h>
#include <renderer/VertexArray.h>
#include <renderer/Texture.h>
#include <systems/Texture2DSpriteSheet.h>

#include <renderer/Renderer.h>
#include <renderer/Renderer2D.h>
#include <renderer/OrthographicCamera.h>
#include <systems/OrthographicCameraController.h>
*/

// #include <Savannah/Systems/Scene/Scene.h>
// #include <Savannah/Systems/Scene/SceneSerializer.h>
// #include <Savanahh/Systems/ECS/Entity.h>
// #include <Savannah/Systems/ECS/Components.h>

// #include <Savannah/EngineData.h>

namespace Savannah {
	class SAVANNAH_API Application {
	public:
		Application();
		virtual ~Application();
		
		// void OnEvent(Event& e);
		
		void Run();
		
		// void PushLayer(Layer* layer);
		// void PushOverlay(Layer* overlay);
		// void PopLayer(Layer* layer);
		// void PopOverlay(Layer* overlay);
		
		inline void Close() { m_Running = false; }
		
		// inline Window* GetWindow() { return m_Window; }
		inline static Application* Get() { return m_Instance; }
		inline static void Set(Application* App) { delete m_Instance; m_Instance = App; }
		// inline void SetImGuiLayer(Layer* layer){ m_ImGuiLayer = layer; }
		// inline Layer* GetImGuiLayer(){ return m_ImGuiLayer; }
		
	protected:
		// bool OnWindowClose(WindowCloseEvent& e);
		// bool OnWindowResize(WindowResizeEvent& e);
		// bool OnKeyPressed(KeyPressedEvent& e);
		
		// Window* m_Window;
		bool m_Running = true;
		bool m_Minimized = false;
		// LayerStack m_LayerStack;
		// Layer* m_ImGuiLayer = nullptr;
		bool m_ImGuiLayerEnabled = true;
		float m_LastFrameTime = 0.0f;
		float m_FrameTime = 0.0f;
		
	private:
		static Application* m_Instance;
	};
	
	// to be defined in client app
	Application* CreateApplication();
}

#endif
