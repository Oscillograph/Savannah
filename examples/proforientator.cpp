#include <include/savannah/savannah.h>
#include <examples/proforientator/include/yaml_wrapper.h>
#include <examples/proforientator/include/logger.h>

// Compiled Dear ImGui to a binary .a file and linked it to Savannah.
// Result: 40+ seconds to build -> 4-7 seconds to build.
// I am very glad.

namespace Savannah 
{
	class Proforientator : public App 
	{
	public:
		Proforientator()
		{
		}
		
		~Proforientator()
		{
		}
		
		void GUIContent() override 
		{
			bool p_open = true;
			static bool use_work_area = true;
			// static ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;
			static ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoSavedSettings;
		
			
			static bool animate = true;
			ImGui::Checkbox("Animate", &animate);
			
			static float values[90] = {};
			static int values_offset = 0;
			static double refresh_time = 0.0;
			if (!animate || refresh_time == 0.0)
				refresh_time = ImGui::GetTime();
			while (refresh_time < ImGui::GetTime()) // Create data at fixed 60 Hz rate for the demo
			{
				static float phase = 0.0f;
				values[values_offset] = cosf(phase);
				values_offset = (values_offset + 1) % IM_ARRAYSIZE(values);
				phase += 0.10f * values_offset;
				refresh_time += 1.0f / 60.0f;
			}
			
			// Plots can display overlay texts
			// (in this example, we will display an average value)
			{
				float average = 0.0f;
				for (int n = 0; n < IM_ARRAYSIZE(values); n++)
					average += values[n];
				average /= (float)IM_ARRAYSIZE(values);
				char overlay[32];
				sprintf(overlay, "avg %f", average);
				ImGui::PlotLines("Lines", values, IM_ARRAYSIZE(values), values_offset, overlay, -1.0f, 1.0f, ImVec2(0, 80.0f));
			}
		}
		
	private:
	};
	
	App* CreateApplication()
	{
		return new Proforientator();
	}
}
