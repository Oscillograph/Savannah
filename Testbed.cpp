#include "Savannah.h"

namespace Savannah {
	class Testbed : public App {
	public:
		Testbed(){
			
		}
		
		~Testbed(){
			
		}
		
		void GUIContent() override {
			bool p_open = true;
			static bool use_work_area = true;
			// static ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;
			static ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoSavedSettings;
			
			// We demonstrate using the full viewport area or the work area (without menu-bars, task-bars etc.)
			// Based on your use case you may want one or the other.
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			// viewport->WorkPos.x -= 50;
			// viewport->WorkPos.y -= 50;
			// viewport->WorkSize.y += 50;
			ImGui::SetNextWindowPos(use_work_area ? viewport->WorkPos : viewport->Pos);
			ImGui::SetNextWindowSize(use_work_area ? viewport->WorkSize: viewport->Size);
			
			if (ImGui::Begin("Example: Fullscreen window", &p_open, flags))
			{
				ImGui::Checkbox("Use work area instead of main area", &use_work_area);
				ImGui::SameLine();
				// HelpMarker("Main Area = entire viewport,\nWork Area = entire viewport minus sections used by the main menu bars, task bars etc.\n\nEnable the main-menu bar in Examples menu to see the difference.");
				
				ImGui::CheckboxFlags("ImGuiWindowFlags_NoBackground", &flags, ImGuiWindowFlags_NoBackground);
				ImGui::CheckboxFlags("ImGuiWindowFlags_NoDecoration", &flags, ImGuiWindowFlags_NoDecoration);
				ImGui::Indent();
				ImGui::CheckboxFlags("ImGuiWindowFlags_NoTitleBar", &flags, ImGuiWindowFlags_NoTitleBar);
				ImGui::CheckboxFlags("ImGuiWindowFlags_NoCollapse", &flags, ImGuiWindowFlags_NoCollapse);
				ImGui::CheckboxFlags("ImGuiWindowFlags_NoScrollbar", &flags, ImGuiWindowFlags_NoScrollbar);
				ImGui::Unindent();
			}
			ImGui::End();
			
			
			if (ImGui::BeginMainMenuBar())
			{
				if (ImGui::BeginMenu("File"))
				{
					// ShowExampleMenuFile();
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu("Edit"))
				{
					if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
					if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
					ImGui::Separator();
					if (ImGui::MenuItem("Cut", "CTRL+X")) {}
					if (ImGui::MenuItem("Copy", "CTRL+C")) {}
					if (ImGui::MenuItem("Paste", "CTRL+V")) {}
					ImGui::EndMenu();
				}
				ImGui::EndMainMenuBar();
			}
		}
	};
	
	App* CreateApplication(){
		return new Testbed();
	}
}

/*
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
*/
