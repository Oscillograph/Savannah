#include "Savannah.h"

namespace Savannah {
	class TestBedApp : public Application {
	public:
		TestBedApp(){
			// PushLayer(new ExampleLayer());
			// PushLayer(new SceneViewer());
			
			// SetImGuiLayer(new EditorGuiLayer());
			// PushOverlay(GetImGuiLayer());
			
			// m_ImGuiLayerEnabled = true;
			// TODO: find out why m_WinTitle is overridden by the engine library
			// m_WinTitle = "BSE Editor";
			// m_Window->SetTitle(m_WinTitle);
			// m_Window->SetVSync(false);
			// m_Window->SetVSync(true);
			
			EngineData::FPS_deltaTime = SAVANNAH_FPS60;
			SAVANNAH_INFO("Hello, world!\n");
		}
		
		~TestBedApp(){
			SAVANNAH_INFO("Exit TestBed\n");
		}
		
	private:
		std::string m_WinTitle;
	};
	
	Application* CreateApplication() {
		return new TestBedApp();
	}
}
