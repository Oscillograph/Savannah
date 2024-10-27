#include "./implot-test/include/implot_test.h"

namespace Savannah 
{
	ImPlotTest::ImPlotTest()
	{
		SAVANNAH_CONSOLE_GREEN("Savannah Framework initialization succesful.");
		SetWindowTitle("ImPlotTest 1.0");
	}
	
	ImPlotTest::~ImPlotTest()
	{
//		context = nullptr;
	}

	void ImPlotTest::SetupResources() 
	{
	}
	
	void ImPlotTest::Logic() 
	{
	}
	
	void ImPlotTest::GUIContent() 
	{
		bool p_open = true;
		static bool use_work_area = true;
		// static ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;
		static ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoSavedSettings;
		
		TEXT_BASE_WIDTH = ImGui::CalcTextSize("A").x;
		TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();
		
		// ====================================================================================
		// The Application starts here
		// ------------------------------------------------------------------------------------
		ShowMainMenu();
		
		// We demonstrate using the full viewport area or the work area (without menu-bars, task-bars etc.)
		// Based on your use case you may want one or the other.
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(use_work_area ? viewport->WorkPos : viewport->Pos);
		ImGui::SetNextWindowSize(use_work_area ? viewport->WorkSize: viewport->Size);
		// ------------------------------------------------------------------------------------
		
		if (ImGui::Begin("Example: Fullscreen window", &p_open, flags))
		{
//			int   bar_data[11] = {1, 2, 4, 2, 1, 0, -1, 2, 7, 9, 8};
//			
//			if (ImPlot::BeginPlot("My Plot")) {
//				ImPlot::PlotBars("My Bar Plot", bar_data, 11);
//				ImPlot::EndPlot();
//			}
			
			{
				static float values1[7][7]  = {{0.8f, 2.4f, 2.5f, 3.9f, 0.0f, 4.0f, 0.0f},
					{2.4f, 0.0f, 4.0f, 1.0f, 2.7f, 0.0f, 0.0f},
					{1.1f, 2.4f, 0.8f, 4.3f, 1.9f, 4.4f, 0.0f},
					{0.6f, 0.0f, 0.3f, 0.0f, 3.1f, 0.0f, 0.0f},
					{0.7f, 1.7f, 0.6f, 2.6f, 2.2f, 6.2f, 0.0f},
					{1.3f, 1.2f, 0.0f, 0.0f, 0.0f, 3.2f, 5.1f},
					{0.1f, 2.0f, 0.0f, 1.4f, 0.0f, 1.9f, 6.3f}};
				static float scale_min       = 0;
				static float scale_max       = 6.3f;
				static const char* xlabels[] = {"C1","C2","C3","C4","C5","C6","C7"};
				static const char* ylabels[] = {"R1","R2","R3","R4","R5","R6","R7"};
				
				static ImPlotColormap map = ImPlotColormap_Viridis;
				
				ImPlot::GetColormapName(map);
				
				if (ImPlot::ColormapButton(ImPlot::GetColormapName(map), ImVec2(225,0), map)) {
					map = (map + 1) % ImPlot::GetColormapCount();

					// We bust the color cache of our plots so that item colors will
					// resample the new colormap in the event that they have already
					// been created. See documentation in implot.h.
					ImPlot::BustColorCache("##Heatmap1");
					ImPlot::BustColorCache("##Heatmap2");
				}
				
				ImGui::SameLine();
				ImGui::LabelText("##Colormap Index", "%s", "Change Colormap");
				ImGui::SetNextItemWidth(225);
				ImGui::DragFloatRange2("Min / Max",&scale_min, &scale_max, 0.01f, -20, 20);
				
				static ImPlotHeatmapFlags hm_flags = 0;
				
				ImGui::CheckboxFlags("Column Major", (unsigned int*)&hm_flags, ImPlotHeatmapFlags_ColMajor);
				
				static ImPlotAxisFlags axes_flags = ImPlotAxisFlags_Lock | ImPlotAxisFlags_NoGridLines | ImPlotAxisFlags_NoTickMarks;
				
				ImPlot::PushColormap(map);
				
				const int sizeX = 240;
				const int sizeY = 240;
				static double values2[sizeX*sizeY];
				srand((unsigned int)(ImGui::GetTime()*1000000));
				for (int i = 0; i < sizeX*sizeY; ++i)
					values2[i] = ImPlot::RandomRange(0.0,1.0);
				
				if (ImPlot::BeginPlot("##Heatmap2",ImVec2(800,800))) {
					ImPlot::SetupAxes(nullptr, nullptr, ImPlotAxisFlags_NoDecorations, ImPlotAxisFlags_NoDecorations);
					ImPlot::SetupAxesLimits(-1,1,-1,1);
					ImPlot::PlotHeatmap("heat1",values2,sizeX,sizeY,0,1,nullptr);
//					ImPlot::PlotHeatmap("heat2",values2,sizeX,sizeY,0,1,nullptr, ImPlotPoint(-1,-1), ImPlotPoint(0,0));
					ImPlot::EndPlot();
				}
				
				ImPlot::PopColormap();
			}
			
			ImGui::End();
		}
		// ====================================================================================
	}
	
	void ImPlotTest::ShowMainMenu()
	{
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("Меню"))
			{
				if (ImGui::MenuItem("Выход")) 
				{
					SAVANNAH_CONSOLE_LOG("Add a new Task: Exit");
					doExit = true;
				}
				ImGui::EndMenu();
			}
			
			
			ImGui::EndMainMenuBar();
		}
	}	

	App* CreateApplication()
	{
		ImPlotTest* app = new ImPlotTest();
		app->SetFPS(SAVANNAH_FPS60);
		return app;
	}
}
