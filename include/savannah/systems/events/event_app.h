#ifndef BSE_APPEVENT_H
#define BSE_APPEVENT_H

#include <savannah/systems/events/event.h>

namespace Savannah 
{
	// WINDOW
	class SAVANNAH_API WindowResizeEvent : public Event 
	{
	public:
		WindowResizeEvent(unsigned int width, unsigned int height)
			: m_Width(width), m_Height(height) {}
		
		inline unsigned int GetWidth() const { return m_Width; }
		inline unsigned int GetHeight() const { return m_Height; }
		
		std::string ToString() const override 
		{
			std::string out = "";
			sprintf((char*)out.c_str(), "Окно изменило размер: %d, %d", m_Width, m_Height);
			return out;
		}
		
		EVENT_CLASS_TYPE(WindowResize)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	private:
		unsigned int m_Width, m_Height;
	};
	
	class SAVANNAH_API WindowCloseEvent : public Event 
	{
	public:
		WindowCloseEvent() {}
		
		EVENT_CLASS_TYPE(WindowClose)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};
	
	// APPLICATION
	class SAVANNAH_API AppTickEvent : public Event {
	public:
		AppTickEvent() {}
		
		EVENT_CLASS_TYPE(AppTick)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};
	
	class SAVANNAH_API AppUpdateEvent : public Event {
	public:
		AppUpdateEvent() {}
		
		EVENT_CLASS_TYPE(AppUpdate)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};
	
	class SAVANNAH_API AppRenderEvent : public Event {
	public:
		AppRenderEvent() {}
		
		EVENT_CLASS_TYPE(AppRender)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};
}

#endif
