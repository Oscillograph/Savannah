#include <Savannah/Core/Application/Application.h>

namespace Savannah {
	Application* Application::m_Instance = nullptr;
	
	Application::Application() {
		m_Instance = this;
	}
	
	Application::~Application(){
		
	}
	
	void Application::Run(){
		while(m_Running){
			m_Running = false;
		}
	}
}
