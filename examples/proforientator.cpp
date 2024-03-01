#include <include/savannah/savannah.h>
// #include "./proforientator/include/datatypes.h"
#include "./proforientator/include/yaml_wrapper.h"
#include "./proforientator/include/logger.h"

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
			SetWindowTitle("Профориентатор 1.0");
			
			// load database
			m_SkillsFile = "../examples/proforientator/data/skillsDB.txt";
			m_SkillsRegistry = new SkillRegistry();
			m_YAMLWrapperObject = new YamlWrapper();
			m_YAMLWrapperObject->UseSkillRegistry(m_SkillsRegistry);
			m_YAMLWrapperObject->LoadDocument(m_SkillsFile);
			
			CONSOLE_GREY("Skill not aquaried");
			CONSOLE_WHITE("Skill aquired.");
			CONSOLE_CYAN("Skill is almost perfect.");
			
			CONSOLE_CAPTION_BLACK("- - - - - - - - - - - - - - - - - ------- - - - - - - - - - - - - - - - - - - -");
			CONSOLE_CAPTION_BLACK("- - - - - - - - -  Job requirements  - - - - - - - - - -");
			CONSOLE_RED("Skill doesn't match requirements.");
			CONSOLE_YELLOW("Skill matches requirements.");
			CONSOLE_GREEN("Skill excedes requirements.");
		}
		
		~Proforientator()
		{
			delete m_SkillsRegistry;
			delete m_YAMLWrapperObject;
		}
		
		void GUIContent() override 
		{
			bool p_open = true;
			static bool use_work_area = true;
			// static ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;
			static ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoSavedSettings;
			
			const float TEXT_BASE_WIDTH = ImGui::CalcTextSize("A").x;
			const float TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();
			
			// ====================================================================================
			// The Application starts here
			// ------------------------------------------------------------------------------------
			if (ImGui::BeginMainMenuBar())
			{
				if (ImGui::BeginMenu("Файл"))
				{
					// ShowExampleMenuFile();
					if (ImGui::MenuItem("Выход")) { doExit = true; }
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu("Правка"))
				{
					if (ImGui::MenuItem("Отменить", "CTRL+Z")) {}
					if (ImGui::MenuItem("Повторить", "CTRL+Y", false, false)) {}  // Disabled item
					ImGui::Separator();
					if (ImGui::MenuItem("Вырезать", "CTRL+X")) {}
					if (ImGui::MenuItem("Копировать", "CTRL+C")) {}
					if (ImGui::MenuItem("Вставить", "CTRL+V")) {}
					ImGui::EndMenu();
				}
				ImGui::EndMainMenuBar();
			}
			
			// We demonstrate using the full viewport area or the work area (without menu-bars, task-bars etc.)
			// Based on your use case you may want one or the other.
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(use_work_area ? viewport->WorkPos : viewport->Pos);
			ImGui::SetNextWindowSize(use_work_area ? viewport->WorkSize: viewport->Size);
			// ------------------------------------------------------------------------------------
			
			if (ImGui::Begin("Example: Fullscreen window", &p_open, flags))
			{
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
					ImGui::PlotLines(
						"На самом деле, подпись справа -- это неудобно, потому что текст уезжает далеко.", 
						values, 
						IM_ARRAYSIZE(values), 
						values_offset, 
						overlay, 
						-1.0f, 
						1.0f, 
						ImVec2(TEXT_BASE_WIDTH*60, TEXT_BASE_HEIGHT*10)
						);
				}
				
//				ImGui::ShowStackToolWindow();
				
				{
					ImGui::Columns(4);
					ImGui::SetColumnWidth(0, TEXT_BASE_WIDTH * 32);
					ImGui::SetColumnWidth(1, TEXT_BASE_WIDTH * 32);
					ImGui::SetColumnWidth(2, TEXT_BASE_WIDTH * 32);
					ImGui::SetColumnWidth(3, TEXT_BASE_WIDTH * 32);
					ImGui::TextColored({1.0f, 0.7f, 0.5f, 1.0f}, "Технологии");
					if (ImGui::BeginTable("Технологии", 2, ImGuiTableFlags_ScrollY, {TEXT_BASE_WIDTH * 30, TEXT_BASE_HEIGHT * 10}))
					{
						// PushStyleCompact();
						SkillGroup* group = (m_SkillsRegistry->GetGroups())["Технологии"];
						for (auto skillsIterator = group->children.begin(); skillsIterator != group->children.end(); skillsIterator++)
						{
							std::string nameID = (*skillsIterator)->name;
							ImGui::TableNextColumn();
							ImGui::PushID(nameID.c_str());
							ImGui::Text("%s", nameID.c_str());
							ImGui::Spacing();
							ImGui::TableNextColumn();
							std::string valueID = (*skillsIterator)->name;
							// ImGui::PushID(valueID.c_str());
							std::string value = "";
							for (int i = 0; i < 10; i++)
							{
								if (i < (*skillsIterator)->level)
								{
									value += "+";
								} else {
									value += " ";
								}
							}
							ImGui::Text("%s", value.c_str());
							ImGui::PopID();
						}
						// PopStyleCompact();
						ImGui::EndTable();
					}
					
					ImGui::NextColumn();
					ImGui::TextColored({1.0f, 0.7f, 0.5f, 1.0f}, "Инструменты");
					if (ImGui::BeginTable("Инструменты", 2, ImGuiTableFlags_ScrollY, {TEXT_BASE_WIDTH * 30, TEXT_BASE_HEIGHT * 10}))
					{
						// PushStyleCompact();
						SkillGroup* group = (m_SkillsRegistry->GetGroups())["Инструменты"];
						for (auto skillsIterator = group->children.begin(); skillsIterator != group->children.end(); skillsIterator++)
						{
							std::string nameID = (*skillsIterator)->name;
							ImGui::TableNextColumn();
							ImGui::PushID(nameID.c_str());
							ImGui::Text("%s", nameID.c_str());
							ImGui::Spacing();
							ImGui::TableNextColumn();
							std::string valueID = (*skillsIterator)->name;
							// ImGui::PushID(valueID.c_str());
							std::string value = "";
							for (int i = 0; i < 10; i++)
							{
								if (i < (*skillsIterator)->level)
								{
									value += "+";
								} else {
									value += " ";
								}
							}
							ImGui::Text("%s", value.c_str());
							ImGui::PopID();
						}
						// PopStyleCompact();
						ImGui::EndTable();
					}
					
					ImGui::NextColumn();
					ImGui::TextColored({1.0f, 0.7f, 0.5f, 1.0f}, "Запчасти");
					if (ImGui::BeginTable("Запчасти", 2, ImGuiTableFlags_ScrollY, {TEXT_BASE_WIDTH * 30, TEXT_BASE_HEIGHT * 10}))
					{
						// PushStyleCompact();
						SkillGroup* group = (m_SkillsRegistry->GetGroups())["Запчасти"];
						for (auto skillsIterator = group->children.begin(); skillsIterator != group->children.end(); skillsIterator++)
						{
							std::string nameID = (*skillsIterator)->name;
							ImGui::TableNextColumn();
							ImGui::PushID(nameID.c_str());
							ImGui::Text("%s", nameID.c_str());
							ImGui::Spacing();
							ImGui::TableNextColumn();
							std::string valueID = (*skillsIterator)->name;
							// ImGui::PushID(valueID.c_str());
							std::string value = "";
							for (int i = 0; i < 10; i++)
							{
								if (i < (*skillsIterator)->level)
								{
									value += "+";
								} else {
									value += " ";
								}
							}
							ImGui::Text("%s", value.c_str());
							ImGui::PopID();
						}
						// PopStyleCompact();
						ImGui::EndTable();
					}
					
					ImGui::NextColumn();
					ImGui::TextColored({1.0f, 0.7f, 0.5f, 1.0f}, "Знания");
					if (ImGui::BeginTable("Знания", 2, ImGuiTableFlags_ScrollY, {TEXT_BASE_WIDTH * 30, TEXT_BASE_HEIGHT * 10}))
					{
						// PushStyleCompact();
						SkillGroup* group = (m_SkillsRegistry->GetGroups())["Знания"];
						for (auto skillsIterator = group->children.begin(); skillsIterator != group->children.end(); skillsIterator++)
						{
							std::string nameID = (*skillsIterator)->name;
							ImGui::TableNextColumn();
							ImGui::PushID(nameID.c_str());
							ImGui::Text("%s", nameID.c_str());
							ImGui::Spacing();
							ImGui::TableNextColumn();
							std::string valueID = (*skillsIterator)->name;
							// ImGui::PushID(valueID.c_str());
							std::string value = "";
							for (int i = 0; i < 10; i++)
							{
								if (i < (*skillsIterator)->level)
								{
									value += "+";
								} else {
									value += " ";
								}
							}
							ImGui::Text("%s", value.c_str());
							ImGui::PopID();
						}
						// PopStyleCompact();
						ImGui::EndTable();
					}
					
					ImGui::NextColumn();
				}
				
				ImGui::End();
			}
			// ====================================================================================
		}
		
	private:
		std::string m_SkillsFile = "";
		SkillRegistry* m_SkillsRegistry = nullptr;
		YamlWrapper* m_YAMLWrapperObject = nullptr;
	};
	
	App* CreateApplication()
	{
		return new Proforientator();
	}
}
