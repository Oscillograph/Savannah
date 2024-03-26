#ifndef SAVANNAH_WINDOW_H
#define SAVANNAH_WINDOW_H

#include <savannah/core.h>
#include <savannah/systems/events/event.h>

namespace Savannah
{
	struct WindowProperties
	{
		std::string title = "Default Window Title";
		unsigned int width = 1200;
		unsigned int height = 768;
		bool vsync = true;
		
		WindowProperties(const std::string& _title = "Default Window Title", 
			unsigned int _width = 320 * 4, 
			unsigned int _height = 200 * 4, 
			bool _vsync = true)
			: title(_title), width(_width), height(_height), vsync(_vsync) 
		{}
	};
	
	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;
		
		virtual ~Window() {}
		
		WindowProperties GetProperties() { return m_Properties; }
		virtual std::string GetTitle() { return m_Properties.title; }
		virtual unsigned int GetWidth() { return m_Properties.width; }
		virtual unsigned int GetHeight() { return m_Properties.height; }
		virtual bool IsVSync() { return m_Properties.vsync; }
		
		virtual float GetTime() {}
		
		// to be virtual
		virtual void* GetNativeWindow() {} // get GLFWwindow pointer
		virtual void OnUpdate(int code = 0) {}
		virtual void SetVSync(bool vsync) {}
		virtual void SetTitle(const std::string& title) {}
		virtual void SetEventCallback(const EventCallbackFn& callback) {}
		
		static Window* Create(const WindowProperties& properties); // defined in window implementation
	private:
		WindowProperties m_Properties;
	};
	
	class WindowStack
	{
	public:
		WindowStack() = default;
		~WindowStack() {};
		
		void Push(Window*) {};
		void Pop(Window*) {};
		
	private:
		std::vector<Window*> m_Windows = {};
	};
	
}

#endif
