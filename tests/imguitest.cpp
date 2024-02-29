#include <include/savannah/savannah.h>

namespace Savannah 
{
	class Testbed : public App 
	{
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
			
			ImGui::ShowDemoWindow();
		}
		
	private:
	};
	
	App* CreateApplication(){
		return new Testbed();
	}
}
