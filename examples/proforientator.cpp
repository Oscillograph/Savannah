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
			
			m_LevelDescription.push_back("незнаком"); // 0
			m_LevelDescription.push_back("слышал"); // 1
			m_LevelDescription.push_back("знаком"); // 2
			m_LevelDescription.push_back("есть пет-проекты"); // 3
			m_LevelDescription.push_back("разбираюсь в чужих проектах"); // 4
			m_LevelDescription.push_back("широкий кругозор"); // 5
			m_LevelDescription.push_back("могу работать в команде как участник"); // 6
			m_LevelDescription.push_back("могу создавать новые решения"); // 7
			m_LevelDescription.push_back("могу работать в команде как эксперт"); // 8
			m_LevelDescription.push_back("могу руководить работой команды"); // 9
			m_LevelDescription.push_back("БОГ"); // 10
			
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
			m_YAMLWrapperObject->SaveDocument(m_SkillsFile);
			delete m_SkillsRegistry;
			delete m_YAMLWrapperObject;
		}
		
		void GUIContent() override 
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
				if (ImGui::BeginTable("Top Table", 2, ImGuiTableFlags_ScrollY, {TEXT_BASE_WIDTH * 160, TEXT_BASE_HEIGHT * 11}))
				{
					ImGui::TableNextColumn();
					ShowLogo();
					ImGui::TableNextColumn();
					ShowContent();
					ImGui::EndTable();
				}
				
//				ImGui::ShowStackToolWindow();
				
				{
					if (ImGui::BeginTable("Middle Table", 4, ImGuiTableFlags_RowBg, {TEXT_BASE_WIDTH * 140, TEXT_BASE_HEIGHT * 12}))
					{
						ImGui::TableNextColumn();
						ShowSkillsTable("Технологии");
						ImGui::TableNextColumn();
						ShowSkillsTable("Инструменты");
						ImGui::TableNextColumn();
						ShowSkillsTable("Запчасти");
						ImGui::TableNextColumn();
						ShowSkillsTable("Знания");
						ImGui::EndTable();
					}
					/*
					ImGui::Columns(4);
					ImGui::SetColumnWidth(0, TEXT_BASE_WIDTH * 40);
					ImGui::SetColumnWidth(1, TEXT_BASE_WIDTH * 40);
					ImGui::SetColumnWidth(2, TEXT_BASE_WIDTH * 40);
					ImGui::SetColumnWidth(3, TEXT_BASE_WIDTH * 40);
					
					ShowSkillsTable("Технологии");
					ImGui::NextColumn();
					ShowSkillsTable("Инструменты");
					ImGui::NextColumn();
					ShowSkillsTable("Запчасти");
					ImGui::NextColumn();
					ShowSkillsTable("Знания");
					
					ImGui::NextColumn();
					*/
				}
				
				ImGui::End();
			}
			// ====================================================================================
		}
		
	private:
		std::string m_SkillsFile = "";
		std::vector<std::string> m_LevelDescription = {};
		SkillRegistry* m_SkillsRegistry = nullptr;
		Skill* m_SkillSelected = nullptr;
		YamlWrapper* m_YAMLWrapperObject = nullptr;
		
		float TEXT_BASE_WIDTH = 0.0f;
		float TEXT_BASE_HEIGHT = 0.0f;
		
		void ShowMainMenu()
		{
			if (ImGui::BeginMainMenuBar())
			{
				if (ImGui::BeginMenu("Файл"))
				{
					// ShowExampleMenuFile();
					if (ImGui::MenuItem("Выход")) 
					{ 
						doExit = true; 
					}
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
		}
		
		void ShowLogo()
		{
			// A beautiful sinusoid
			{
				static bool animate = true;
				// ImGui::Checkbox("Animate", &animate);
				
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
					// float average = 0.0f;
					// for (int n = 0; n < IM_ARRAYSIZE(values); n++)
					//	average += values[n];
					// average /= (float)IM_ARRAYSIZE(values);
					char overlay[32];
					// sprintf(overlay, "avg %f", average);
					ImGui::PlotLines(
						"###Lines", 
						values, 
						IM_ARRAYSIZE(values), 
						values_offset, 
						overlay, 
						-1.0f, 
						1.0f, 
						ImVec2(TEXT_BASE_WIDTH*60, TEXT_BASE_HEIGHT*10)
						);
				}
			}
		}
		
		void ShowContent()
		{
			std::string raw = R"(Если чуточку разобраться, то Dear ImGui оказывается очень даже ручной.
Строительство интерфейса напоминает конструирование таблиц-на-лету, и
ощущения чем-то похожи на сайтостроительство тех времён, когда в ходу
были преимущественно таблицы.

<-- А синусоида-то анимированная!

)";
			ImGui::Text(raw.c_str());
			if (m_SkillSelected != nullptr)
			{
				std::string selected = "Выбран навык: ";
				ImGui::Text(selected.c_str());
				ImGui::SameLine();
				ImGui::InputText("###Name", &(m_SkillSelected->name));
				
				int level = (int)(m_SkillSelected->level);
				ImGui::Text("Уровень: ");
				ImGui::SameLine();
				ImGui::SliderInt("###Level", &level, 0, 10);
				m_SkillSelected->level = (uint32_t)level;
				ImVec4 color;
				if (level < 2)
				{ // red
					color = {1.0f, 0.4f, 0.4f, 1.0f};
				} else {
					if (level < 4)
					{ // orange
						color = {1.0f, 0.7f, 0.5f, 1.0f};
					} else {
						if (level < 6)
						{ // yellow
							color = {1.0f, 1.0f, 0.5f, 1.0f};
						} else {
							if (level < 10)
							{ // green
								color = {0.6f, 1.0f, 0.5f, 1.0f};
							} else {
								if (level == 10)
								{ // magenta
									color = {1.0f, 0.5f, 1.0f, 1.0f};
								}
							}
						}
					}
				}
				ImGui::TextColored(color, m_LevelDescription[level].c_str());
			}
		}
		
		void ShowSkillsTable(const std::string& groupName)
		{
			ImGui::TextColored({1.0f, 0.7f, 0.5f, 1.0f}, groupName.c_str());
			if (ImGui::BeginTable(groupName.c_str(), 2, ImGuiTableFlags_ScrollY, {TEXT_BASE_WIDTH * 30, TEXT_BASE_HEIGHT * 10}))
			{
				// PushStyleCompact();
				SkillGroup* group = (m_SkillsRegistry->GetGroups())[groupName];
				for (auto skillsIterator = group->children.begin(); skillsIterator != group->children.end(); skillsIterator++)
				{
					std::string nameID = (*skillsIterator)->name;
					ImGui::TableNextColumn();
					bool selected = false;
					if ((*skillsIterator) == m_SkillSelected)
					{
						selected = true;
					} else {
						selected = false;
					}
					ImGui::PushID(nameID.c_str());
					if (ImGui::Selectable(nameID.c_str(), &selected, ImGuiSelectableFlags_SpanAllColumns))
					{
						m_SkillSelected = *skillsIterator;
					}
					ImGui::Spacing();
					ImGui::TableNextColumn();
					std::string valueID = (*skillsIterator)->name;
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
					if (ImGui::Selectable(value.c_str(), &selected, ImGuiSelectableFlags_SpanAllColumns))
					{
						m_SkillSelected = *skillsIterator;
					}
					ImGui::PopID();
				}
				ImGui::EndTable();
				
				ImGui::Text("");
				std::string buttonName = "Новый навык##" + group->name;
				if (ImGui::Button(buttonName.c_str(), {TEXT_BASE_WIDTH * 30, TEXT_BASE_HEIGHT * 2}))
				{
					m_SkillSelected = m_SkillsRegistry->NewSkill("Новый навык", group->name, 0);
				}
			}
		}
	};
	
	App* CreateApplication()
	{
		return new Proforientator();
	}
}
