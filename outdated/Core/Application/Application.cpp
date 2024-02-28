#include <Savannah/Core/Application/Application.h>

namespace Savannah {
	Application* Application::m_Instance = nullptr;
	
	Application::Application() {
		m_Instance = this;
		
		BSE_TRACE("Trying to create an app window");
		m_Window = Window::Create(*(new WindowProperties()));
	}
	
	Application::~Application(){
		
	}
	
	void Application::Run(){
		while(m_Running){
			m_Running = false;
		}
	}
}
