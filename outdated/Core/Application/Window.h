#ifndef SAVANNAH_WINDOW_H
#define SAVANNAH_WINDOW_H

// this is an abstract representation of a window

#include <Savannah/Core.h>
#include <Savannah/Systems/Events/Event.h>

namespace Savannah {
	struct WindowProperties {
		std::string Title;
		unsigned int Width;
		unsigned int Height;
		
		WindowProperties(const std::string& title = "Blazing Serpent Engine",
			unsigned int width = 320 * 4,
			unsigned int height = 200 * 4)
		: Title(title), Width(width), Height(height)
		{}
	};
	
	// the window interface
	class SAVANNAH_API Window {
	public:
		using EventCallbackFn = std::function<void(Event&)>;
		
		virtual ~Window() {}
		virtual void OnUpdate(int code = 0) const = 0;
		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;
		
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;
		
		virtual void* GetNativeWindow() const = 0;
		
		virtual float GetTime() const = 0;
		virtual std::string GetTitle() const = 0;
		virtual void SetTitle(std::string newTitle) = 0;
		
		static Window* Create(WindowProperties& properties);
	};
}

#endif
