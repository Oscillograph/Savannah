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
			LoadDatabase("../examples/proforientator/data/skillsDB.txt");
			
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
			UnloadDatabase();
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
				if (ImGui::BeginTable("Top Table", 2, ImGuiTableFlags_ScrollY | ImGuiTableFlags_SizingFixedFit, {TEXT_BASE_WIDTH * 160, TEXT_BASE_HEIGHT * 16}))
				{
					ImGui::TableSetupColumn("##LeftHalf", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_WIDTH * 60);
					ImGui::TableSetupColumn("##RightHalf", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_WIDTH * 80);
//					ImGui::TableHeadersRow();
					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);
					ShowLogo();
					ImGui::TableSetColumnIndex(1);
					ShowContent();
					ImGui::EndTable();
				}
				
//				ImGui::ShowStackToolWindow();
				
				{
					if (ImGui::BeginTable("Middle Table", 4, ImGuiTableFlags_RowBg, {TEXT_BASE_WIDTH * 140, TEXT_BASE_HEIGHT * 12}))
					{
						ImGui::TableSetupColumn("##Group1", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_WIDTH * 35);
						ImGui::TableSetupColumn("##Group2", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_WIDTH * 35);
						ImGui::TableSetupColumn("##Group3", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_WIDTH * 35);
						ImGui::TableSetupColumn("##Group4", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_WIDTH * 35);
						ImGui::TableNextRow();
						
						int i = 0;
						std::map<std::string, SkillGroup*>& groups = m_SkillsRegistry->GetGroups();
						for (auto it = groups.begin();
							it != groups.end();
							it++)
						{
							ImGui::TableSetColumnIndex(i);
							ShowSkillsTable((*it).first);
							i++;
							if (i == 4)
							{
								break;
							}
						}
						ImGui::EndTable();
					}
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
		bool m_ChangesInDatabase = false;
		
		float TEXT_BASE_WIDTH = 0.0f;
		float TEXT_BASE_HEIGHT = 0.0f;
		
		void LoadDatabase(const std::string& file)
		{
			m_SkillsFile = file;
			m_SkillsRegistry = new SkillRegistry();
			m_YAMLWrapperObject = new YamlWrapper();
			m_YAMLWrapperObject->UseSkillRegistry(m_SkillsRegistry);
			m_YAMLWrapperObject->LoadDocument(file);
			m_SkillsRegistry->SortGroups();
		}
		
		void UnloadDatabase()
		{
			if (m_ChangesInDatabase)
			{
				m_YAMLWrapperObject->SaveDocument(m_SkillsFile);
				CONSOLE_LOG("Changes saved.");
			} else {
				CONSOLE_LOG("No changes.");
			}
			delete m_SkillsRegistry;
			delete m_YAMLWrapperObject;
		}
		
		void ReloadDatabase()
		{
			m_SkillSelected = nullptr;
			UnloadDatabase();
			LoadDatabase(m_SkillsFile);
			m_ChangesInDatabase = false;
		}
		
		void ShowMainMenu()
		{
			if (ImGui::BeginMainMenuBar())
			{
				if (ImGui::BeginMenu("Файл"))
				{
					// ShowExampleMenuFile();
					if (ImGui::MenuItem("Перезагрузить БД")) 
					{ 
						ReloadDatabase();
					}
					//ImGui::Separator();
					ImGui::MenuItem(" ");
					if (ImGui::MenuItem("Выход")) 
					{ 
						doExit = true; 
					}
					ImGui::EndMenu();
				}
				/*if (ImGui::BeginMenu("Правка"))
				{
					if (ImGui::MenuItem("Отменить", "CTRL+Z")) {}
					if (ImGui::MenuItem("Повторить", "CTRL+Y", false, false)) {}  // Disabled item
					ImGui::Separator();
					if (ImGui::MenuItem("Вырезать", "CTRL+X")) {}
					if (ImGui::MenuItem("Копировать", "CTRL+C")) {}
					if (ImGui::MenuItem("Вставить", "CTRL+V")) {}
					ImGui::EndMenu();
				}*/
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
					// char overlay[32];
					// sprintf(overlay, "avg %f", average);
					ImGui::PlotLines(
						"###Lines", 
						values, 
						IM_ARRAYSIZE(values), 
						values_offset, 
						// overlay,
						NULL,
						-1.0f, 
						1.0f, 
						ImVec2(TEXT_BASE_WIDTH*60, TEXT_BASE_HEIGHT*10)
						);
					
					ImGui::Text("");
					std::string raw = R"(Профориентатор -- учёт, оценка и анализ компетенций пользователя.
Навыки разделены по группам и собраны в таблички.
Чтобы править информацию о навыке, его нужно выбрать мышкой.
Ползунком отмечается уровень мастерства с от оценкой от 0 до 10.)";
					ImGui::Text(raw.c_str());
				}
			}
		}
		
		void ShowContent()
		{
			std::string raw = R"(<-- Анимированная синусоида!

)";
			ImGui::Text(raw.c_str());
			if (m_SkillSelected != nullptr)
			{
				std::string columnsID = "###EditSkill";
				
				if (ImGui::BeginTable((columnsID.c_str()), 3, ImGuiTableFlags_SizingFixedFit))
				{
					ImGui::TableSetupColumn("##skill", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_WIDTH * 10);
//					ImGui::TableSetupColumn("##control", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_WIDTH * 40);
//					ImGui::TableSetupColumn("##buttons", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_WIDTH * 10);
//					ImGui::TableSetupColumn("##skill");
					ImGui::TableSetupColumn("##control", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_WIDTH * 59);
					ImGui::TableSetupColumn("##buttons", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_WIDTH * 11);
					ImGui::TableHeadersRow();
					
					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);
					ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
					std::string selected = "Навык: ";
					ImGui::Text(selected.c_str());
					
					ImGui::TableSetColumnIndex(1);
					ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
//					ImGui::SetNextItemWidth(TEXT_BASE_WIDTH * 39);
					if (ImGui::InputText("###Name", &(m_SkillSelected->name)))
					{
						m_ChangesInDatabase = true;
//						ImGui::SetKeyboardFocusHere(-1);
					}
					
					ImGui::TableSetColumnIndex(2);
					ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
					std::string buttonName = "Удалить##" + m_SkillSelected->group + m_SkillSelected->name;
					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.8f, 0.3f, 0.3f, 1.0f});
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.8f, 0.6f, 0.4f, 1.0f});
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{1.0f, 0.7f, 0.7f, 1.0f});
					if (ImGui::Button(buttonName.c_str()))
					{
						m_ChangesInDatabase = true;
						m_SkillsRegistry->RemoveSkill(m_SkillSelected);
						m_SkillSelected = nullptr;
						CONSOLE_LOG("m_SkillSelected equals nullptr now.");
					}
					ImGui::PopStyleColor(3);
					
					if (m_SkillSelected != nullptr)
					{
						ImGui::TableNextRow();
						ImGui::TableSetColumnIndex(0);
						int level = (int)(m_SkillSelected->level);
						ImGui::Text("Уровень: ");
						
						ImGui::TableSetColumnIndex(1);
//						ImGui::SetNextItemWidth(TEXT_BASE_WIDTH * 39);
						if (ImGui::SliderInt("###Level", &level, 0, 10))
						{
							m_ChangesInDatabase = true;
							m_SkillSelected->level = (uint32_t)level;
						}
						
						ImGui::TableSetColumnIndex(2);
					}
					ImGui::EndTable();
				}
				
				if (m_SkillSelected != nullptr)
				{	
					TextColoredSkillLevelDescription((int)m_SkillSelected->level);
					std::string requirements = m_SkillsRegistry->GetRequirements(m_SkillSelected);
					ImGui::Text("");
//					ImGui::Text((std::string("Связанные навыки: ") + requirements).c_str());
					
					if (ImGui::BeginTable("##Требования", 2, ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollY))
					{
						ImGui::TableSetupColumn("Полученные навыки##passed", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_WIDTH * 40);
						ImGui::TableSetupColumn("Нужны ещё##notpassed", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_WIDTH * 40);
						ImGui::TableHeadersRow();
						ImGui::TableNextRow();
						
						std::string reqName = "";
						std::string upToLevel = "";
						int reqLevel = 0;
						for (int i = 0; i < m_SkillSelected->GetRequirementsArray().size(); i++)
						{
							reqName = m_SkillSelected->GetRequirementsArray()[i]->name;
							reqLevel = m_SkillSelected->GetRequirementsArray()[i]->level;
							
							int column = 1;
							Skill* skillStored = m_SkillsRegistry->FindSkill(reqName);
							if (skillStored != nullptr)
							{
								if (skillStored->level >= reqLevel)
								{
									sprintf((char*)upToLevel.c_str(), " (%d)", skillStored->level);
									column = 0;
								} else {
									sprintf((char*)upToLevel.c_str(), " (%d)", reqLevel);
								}
								ImGui::TableSetColumnIndex(column);
								TextColoredSkillName(skillStored);
								ImGui::SameLine();
								ImGui::Text(upToLevel.c_str());
							} else {
								ImGui::TableSetColumnIndex(column);
								ImGui::TextColored({1.0f, 0.2f, 0.2f, 1.0f}, reqName.c_str());
								ImGui::SameLine();
								ImGui::Text(upToLevel.c_str());
							}
						}
						
						ImGui::EndTable();
					}
				}
			}
		}
		
		void ShowSkillsTable(const std::string& groupName)
		{
			ImGui::TextColored({1.0f, 0.7f, 0.5f, 1.0f}, groupName.c_str());
			if (ImGui::BeginTable(groupName.c_str(), 2, ImGuiTableFlags_ScrollY, {TEXT_BASE_WIDTH * 30, TEXT_BASE_HEIGHT * 10}))
			{
				// PushStyleCompact();
				SkillGroup* group = (m_SkillsRegistry->GetGroups())[groupName];
//				m_SkillsRegistry->SortGroup(group);
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
					if (ImGui::Selectable(nameID.c_str(), &selected, ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_SelectOnNav))
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
					if (ImGui::Selectable(value.c_str(), &selected, ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_SelectOnNav))
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
					m_ChangesInDatabase = true;
					m_SkillSelected = m_SkillsRegistry->NewSkill("Новый навык", group->name, 0);
				}
			}
		}
		
		void TextColoredSkillName(Skill* skill)
		{
			if (skill != nullptr)
			{
				int level = (int)(skill->level);
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
				ImGui::TextColored(color, skill->name.c_str());
			}
		}
		
		void TextColoredSkillLevelDescription(int level)
		{
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
	};
	
	App* CreateApplication()
	{
		Proforientator* app = new Proforientator();
		app->SetFPS(SAVANNAH_FPS60);
		return app;
	}
}
