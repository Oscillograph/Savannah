#ifndef SAVANNAH_WINDOWSWINDOW_H
#define SAVANNAH_WINDOWSWINDOW_H

#include <Savannah/Core.h>
// #include <BSE/log.h>
#include <Savannah/Application/Window.h>

// #include <renderer/GraphicsContext.h>
// #include <platforms/opengl/OpenGLContext.h>


namespace Savannah {
	class WindowsWindow : public Window {
	public:
		using EventCallbackFn = std::function<void(Event&)>;
		
		WindowsWindow(const WindowProperties& properties);
		~WindowsWindow();
		
		void OnUpdate(int code = 0) const override;
		
		inline unsigned int GetWidth() const override { return m_Data.Width; }
		inline unsigned int GetHeight() const override { return m_Data.Height; }
		inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;
		
		inline void* GetNativeWindow() const override { return m_Window; }
		
		inline float GetTime() const override { return (float)glfwGetTime(); }
		
		inline std::string GetTitle() const override { return m_Data.Title; };
		inline void SetTitle(std::string newTitle) override { 
			m_Data.Title = newTitle;
			glfwSetWindowTitle((GLFWwindow*)GetNativeWindow(), newTitle.c_str());
		};
	private:
		void Init(const WindowProperties& properties);
		void Shutdown();
		
		struct WindowData {
			std::string Title;
			unsigned int Width, Height;
			bool VSync;
			EventCallbackFn EventCallback;
			
			WindowData(const std::string& title = "Blazing Serpent Engine",
				//unsigned int width = 1280,
				//unsigned int height = 720,
				unsigned int width = 640,
				unsigned int height = 360,
				bool vsync = true,
				EventCallbackFn eventCallback = Callback_dummy)
			: Title(title), Width(width), Height(height), VSync(vsync), EventCallback(eventCallback)
			{}
		};
		
		GLFWwindow* m_Window;
		GraphicsContext* m_GraphicsContext;
		
		WindowData m_Data;
	};
}

#endif
