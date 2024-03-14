#include "./proforientator/include/proforientator.h"

namespace Savannah 
{
	Proforientator::Proforientator()
	{
		CONSOLE_GREEN("Savannah Framework initialization succesful.");
		SetWindowTitle("Профориентатор 1.0");
		
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
		
		m_CurrentMode = ProforientatorMode::Idle;
		CONSOLE_LOG("Enter Idle mode");
		
		m_TaskStack.push_back(ProforientatorTasks::LoadData);
		CONSOLE_LOG("Add a new Task: LoadData");

//		CONSOLE_GREY("Skill not aquaried");
//		CONSOLE_WHITE("Skill aquired.");
//		CONSOLE_CYAN("Skill is almost perfect.");
//		
//		CONSOLE_CAPTION_BLACK("- - - - - - - - - - - - - - - - - ------- - - - - - - - - - - - - - - - - - - -");
//		CONSOLE_CAPTION_BLACK("- - - - - - - - -  Job requirements  - - - - - - - - - -");
//		CONSOLE_RED("Skill doesn't match requirements.");
//		CONSOLE_YELLOW("Skill matches requirements.");
//		CONSOLE_GREEN("Skill excedes requirements.");
	}
	
	Proforientator::~Proforientator()
	{
		UnloadDatabase();
		
		delete m_NewSkill;
		m_NewSkill = nullptr;
		delete m_EditSkill;
		m_EditSkill = nullptr;
		m_SkillSelected = nullptr;
		
		m_SkillGroupSelected = nullptr;
		delete m_NewGroup;
		m_NewGroup = nullptr;
	}
	
	void Proforientator::Logic()
	{
		while (m_TaskStack.size() > 0)
		{
			m_CurrentTask = m_TaskStack.back();
			m_TaskStack.pop_back();
			
			switch (m_CurrentTask)
			{
			case ProforientatorTasks::NewSkill:
				{
					m_EditSkill = new Skill("Новый навык", "Groupless", 0);
					m_SkillSelected = m_EditSkill;
					m_CurrentMode = ProforientatorMode::NewSkill;
					CONSOLE_LOG("Enter NewSkill mode");
				}
				break;
			case ProforientatorTasks::CancelNewSkill:
				{
					delete m_NewSkill;
					m_NewSkill = nullptr;
					delete m_EditSkill;
					m_EditSkill = nullptr;
					m_SkillSelected = nullptr;
					m_CurrentMode = ProforientatorMode::Idle;
					CONSOLE_LOG("Enter Idle mode");
				}
				break;
			case ProforientatorTasks::AddSkill:
				{
					if (m_EditSkill != nullptr)
					{
						m_SkillsRegistry->AddSkill(m_EditSkill);
						m_SkillSelected = m_SkillsRegistry->FindSkill(m_EditSkill->name);
						m_EditSkill = nullptr;
						m_EditSkill = new Skill(m_SkillSelected);
						m_ChangesInDatabase = true;
						CONSOLE_LOG("Skill \"", m_EditSkill->name, "\" added to group \"", m_SkillSelected->group, "\".");
						m_CurrentMode = ProforientatorMode::EditSkill;
						CONSOLE_LOG("Enter EditSkill mode");
					} else {
						CONSOLE_LOG("Called Task AddSkill when m_EditSkill is nullptr.");
					}
				}
				break;
			case ProforientatorTasks::EditSkill:
				{
					if (m_EditSkill != nullptr)
					{
						m_SkillSelected->name = m_EditSkill->name;
						if (m_EditSkill->group != m_SkillSelected->group)
						{
							m_SkillsRegistry->RemoveSkillFromGroup(m_SkillSelected);
							m_SkillsRegistry->AddSkillToGroup(m_SkillSelected, m_EditSkill->group);
						}
						m_SkillSelected->level = m_EditSkill->level;
						m_SkillSelected->CopyRequirementsArray(m_EditSkill->GetRequirementsArray());
						m_ChangesInDatabase = true;
						CONSOLE_LOG("Skill \"", m_EditSkill->name, "\" from group \"", m_SkillSelected->group, "\" updated.");
						m_CurrentMode = ProforientatorMode::EditSkill;
						CONSOLE_LOG("Enter EditSkill mode");
					} else {
						CONSOLE_LOG("Called Task EditSkill when m_EditSkill is nullptr.");
					}
				}
				break;
			case ProforientatorTasks::DeleteSkill:
				{
					m_SkillsRegistry->RemoveSkill(m_SkillSelected);
					m_SkillSelected = nullptr;
					delete m_EditSkill;
					m_EditSkill = nullptr;
					m_ChangesInDatabase = true;
					CONSOLE_LOG("m_SkillSelected equals nullptr now.");
				}
				break;
			case ProforientatorTasks::Exit:
				{
					doExit = true;
				}
				break;
			case ProforientatorTasks::LoadData:
				{
					LoadDatabase(m_SkillsFile);
				}
				break;
			case ProforientatorTasks::ReloadData:
				{
					ReloadDatabase();
				}
				break;
			case ProforientatorTasks::SaveData:
				{
					SaveDatabase(m_SkillsFile);
				}
				break;
			case ProforientatorTasks::Idle:
				// do nothing
				break;
			default:
				break;
			}
		}
		m_CurrentTask = ProforientatorTasks::Idle;
	}
	
	void Proforientator::GUIContent() 
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
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ShowLogo();
				ImGui::TableSetColumnIndex(1);
				ShowContent();
//					CONSOLE_LOG("Content shown");
				ImGui::EndTable();
			}
			
//				ImGui::ShowStackToolWindow();
			
			{
				if (ImGui::BeginTable("Middle Table", (m_SkillsRegistry->GetGroupsNames().size() + 2), ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollX, {TEXT_BASE_WIDTH * 140, TEXT_BASE_HEIGHT * 15}))
				{
					std::string columnName = "";
					for (int i = 0; i <= m_SkillsRegistry->GetGroupsNames().size(); i++)
					{
						sprintf((char*)columnName.c_str(), "##Group%d", i);
						ImGui::TableSetupColumn(columnName.c_str(), ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_WIDTH * 35);
					}
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
					
					ImGui::TableSetColumnIndex(i);
					if (ImGui::BeginTable("##NewSkillGroupButton", 1, 0, {TEXT_BASE_WIDTH * 30, TEXT_BASE_HEIGHT * 10}))
					{
						ImGui::TableNextRow();
						ImGui::TableSetColumnIndex(0);
						if (ImGui::Button("+", {TEXT_BASE_WIDTH * 30, TEXT_BASE_HEIGHT * 10}))
						{
							m_CurrentMode = ProforientatorMode::NewSkillGroup;
							CONSOLE_LOG("Enter NewSkillGroup mode");
						}
						ImGui::EndTable();
					}
					
//						CONSOLE_LOG("General skills tables shown.");
					ImGui::TableSetColumnIndex(i + 1);
					ShowSkillsTable("Groupless");
//						CONSOLE_LOG("Groupless skills table shown.");
					ImGui::EndTable();
				}
			}
			
			ImGui::End();
		}
		// ====================================================================================
	}
	
	void Proforientator::LoadDatabase(const std::string& file)
	{
		m_SkillsFile = file;
		m_SkillsRegistry = new SkillRegistry();
		m_YAMLWrapperObject = new YamlWrapper();
		m_YAMLWrapperObject->UseSkillRegistry(m_SkillsRegistry);
		m_YAMLWrapperObject->LoadDocument(file);
		m_SkillsRegistry->SortGroups();
	}
	
	void Proforientator::SaveDatabase(const std::string& file)
	{
		m_YAMLWrapperObject->SaveDocument(file);
	}
	
	void Proforientator::UnloadDatabase()
	{
		if (m_ChangesInDatabase)
		{
			SaveDatabase(m_SkillsFile);
			CONSOLE_LOG("Changes saved.");
		} else {
			CONSOLE_LOG("No changes.");
		}
		delete m_SkillsRegistry;
		delete m_YAMLWrapperObject;
	}
	
	void Proforientator::ReloadDatabase()
	{
		m_SkillSelected = nullptr;
		UnloadDatabase();
		LoadDatabase(m_SkillsFile);
		m_ChangesInDatabase = false;
	}
	
	void Proforientator::CopySkillSelectedToEditSkill()
	{
		delete m_EditSkill;
		m_EditSkill = new Skill(m_SkillSelected);
	}
	
	void Proforientator::ShowMainMenu()
	{
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("Файл"))
			{
				if (ImGui::MenuItem("Перезагрузить БД")) 
				{ 
					m_TaskStack.push_back(ProforientatorTasks::ReloadData);
					CONSOLE_LOG("Add a new Task: ReloadData");
				}
				ImGui::MenuItem(" ");
				if (ImGui::MenuItem("Выход")) 
				{
					m_TaskStack.push_back(ProforientatorTasks::Exit);
					CONSOLE_LOG("Add a new Task: Exit");
				}
				ImGui::EndMenu();
			}
			
			ImGui::EndMainMenuBar();
		}
	}
	
	void Proforientator::ShowLogo()
	{
		// A beautiful sinusoid
		{
			static bool animate = true;
			
			static float values[90] = {};
			static int values_offset = 0;
			static double refresh_time = 0.0;
			if (!animate || refresh_time == 0.0)
				refresh_time = ImGui::GetTime();
			while (refresh_time < ImGui::GetTime()) // Create data at fixed GetFPS() rate for the demo
			{
				static float phase = 0.0f;
				values[values_offset] = cosf(phase);
				values_offset = (values_offset + 1) % IM_ARRAYSIZE(values);
				phase += 0.10f * values_offset;
				refresh_time += GetFPS();
			}
			
			// Plots can display overlay texts // TAKEN FROM imgui_demo.cpp
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
	
	void Proforientator::ShowContent()
	{
		std::string raw = R"(<-- Анимированная синусоида!

)";
		ImGui::Text(raw.c_str());
		if (m_SkillSelected != nullptr)
		{
			if ((m_CurrentMode == ProforientatorMode::EditSkill) ||
				(m_CurrentMode == ProforientatorMode::NewSkill))
			{
				ShowEditSkillTable();
				TextColoredSkillLevelDescription((int)m_EditSkill->level);
//				std::string requirements = m_EditSkill->GetRequirements();
				ImGui::Text("");
				ShowSkillRequirementsTable();
			}
			
			if ((m_CurrentMode == ProforientatorMode::NewSkillGroup) ||
				(m_CurrentMode == ProforientatorMode::EditSkillGroup))
			{
				ShowEditSkillGroupTable();
			}
		}
	}
	
	void Proforientator::ShowSkillRequirementsTable()
	{
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
	
	void Proforientator::ShowEditSkillTable()
	{
		std::string columnsID = "###EditSkill";
		
		if (ImGui::BeginTable((columnsID.c_str()), 3, ImGuiTableFlags_SizingFixedFit))
		{
			ImGui::TableSetupColumn("##skill", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_WIDTH * 10);
			ImGui::TableSetupColumn("##control", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_WIDTH * 59);
			ImGui::TableSetupColumn("##buttons", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_WIDTH * 11);
			ImGui::TableHeadersRow();
			
			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);
			ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
			std::string editSkillName = "Навык: ";
			ImGui::Text(editSkillName.c_str());
			
			ImGui::TableSetColumnIndex(1);
			ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
			if (ImGui::InputText("###Name", &(m_EditSkill->name)))
			{
				m_ChangesInDatabase = true;
			}
			
			ImGui::TableSetColumnIndex(2);
			{
				ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
				std::string buttonName = "Удалить##";
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.8f, 0.3f, 0.3f, 1.0f});
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.8f, 0.6f, 0.4f, 1.0f});
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{1.0f, 0.7f, 0.7f, 1.0f});
				if (ImGui::Button(buttonName.c_str()))
				{
					m_TaskStack.push_back(ProforientatorTasks::DeleteSkill);
					CONSOLE_LOG("Add a new Task: DeleteSkill");
				}
				ImGui::PopStyleColor(3);
			}
			
			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);
			ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
			std::string editSkillGroup = "Группа: ";
			ImGui::Text(editSkillGroup.c_str());
			
			ImGui::TableSetColumnIndex(1);
			{
				ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
				std::string groupsToSelect = "";
				int comboSelected = -1;
				
				for (int i = 0; i <= m_SkillsRegistry->GetGroupsNames().size(); i++)
				{
					if (i < m_SkillsRegistry->GetGroupsNames().size())
					{
						if (m_SkillsRegistry->GetGroupsNames()[i].compare(m_EditSkill->group) == 0)
						{
							comboSelected = i;
						}
						groupsToSelect.append(((i == 0) ? m_SkillsRegistry->GetGroupsNames()[i] : ('\0' + m_SkillsRegistry->GetGroupsNames()[i])));
					} else {
						groupsToSelect.append('\0' + std::string("Groupless"));
					}
				}
				if (comboSelected == -1)
				{
					comboSelected = m_SkillsRegistry->GetGroupsNames().size();
				}
				
				if (ImGui::Combo("###Group", &comboSelected, groupsToSelect.c_str()))
				{
					m_EditSkill->group = (comboSelected == m_SkillsRegistry->GetGroupsNames().size()) ? "Groupless" : m_SkillsRegistry->GetGroupsNames()[comboSelected];
				}
			}
			
			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);
			int level = (int)(m_EditSkill->level);
			ImGui::Text("Уровень: ");
			
			ImGui::TableSetColumnIndex(1);
			if (ImGui::SliderInt("###Level", &level, 0, 10))
			{
				if (m_CurrentMode == ProforientatorMode::EditSkill)
				{
					m_ChangesInDatabase = true;
				}
				m_EditSkill->level = (uint32_t)level;
			}
			
			ImGui::TableSetColumnIndex(2);
			{
				ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
				std::string buttonName = "Сохранить##";
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.3f, 0.8f, 0.3f, 1.0f});
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.4f, 0.8f, 0.6f, 1.0f});
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.7f, 1.0f, 0.7f, 1.0f});
				if (ImGui::Button(buttonName.c_str()))
				{
					switch (m_CurrentMode)
					{
					case ProforientatorMode::EditSkill:
						m_TaskStack.push_back(ProforientatorTasks::EditSkill);
						CONSOLE_LOG("Add a new Task: EditSkill");
						break;
					case ProforientatorMode::NewSkill:
						m_TaskStack.push_back(ProforientatorTasks::AddSkill);
						CONSOLE_LOG("Add a new Task: AddSkill");
						break;
					default:
						CONSOLE_LOG("EditSkill mode active, but m_CurrentMode is not set to it.");
					}
				}
				ImGui::PopStyleColor(3);
			}
			
			ImGui::EndTable();
		}
	}
	
	void Proforientator::ShowEditSkillGroupTable()
	{
	}
	
	void Proforientator::ShowEditSkillRequirementsTable()
	{
	}
	
	void Proforientator::ShowSkillsTable(const std::string& groupName)
	{
		ImGui::TextColored({1.0f, 0.7f, 0.5f, 1.0f}, (groupName.compare("Groupless") == 0) ? "Без группы" : groupName.c_str());
		if (ImGui::BeginTable(groupName.c_str(), 2, ImGuiTableFlags_ScrollY, {TEXT_BASE_WIDTH * 30, TEXT_BASE_HEIGHT * 10}))
		{
			SkillGroup* group = nullptr;
			bool groupless = false;
			
			std::vector<Skill*> skillsCollection;
			if (groupName.compare("Groupless") == 0)
			{
				groupless = true;
				skillsCollection = m_SkillsRegistry->GetGrouplessSkills();
			} else {
				group = (m_SkillsRegistry->GetGroups())[groupName];
				skillsCollection = group->children;
			}
			
//				CONSOLE_LOG("Skills table \"", groupName, " \" size: ", skillsCollection.size());
			for (auto skillsIterator = skillsCollection.begin(); skillsIterator != skillsCollection.end(); skillsIterator++)
			{
				std::string nameID = (*skillsIterator)->name;
				bool selected = false;
				if ((*skillsIterator) == m_SkillSelected)
				{
					selected = true;
				} else {
					selected = false;
				}
				
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::PushID(nameID.c_str());
				if (ImGui::Selectable(nameID.c_str(), &selected, ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_SelectOnNav))
				{
					m_SkillSelected = *skillsIterator;
					m_CurrentMode = ProforientatorMode::EditSkill;
					CONSOLE_LOG("Enter EditSkill mode");
					CopySkillSelectedToEditSkill();
				}
				ImGui::Spacing();
				ImGui::TableSetColumnIndex(1);
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
					m_CurrentMode = ProforientatorMode::EditSkill;
					CONSOLE_LOG("Enter EditSkill mode");
					CopySkillSelectedToEditSkill();
				}
				
				ImGui::PopID();
			}
			
			
			ImGui::EndTable();
			
			ImGui::Text("");
			{
				std::string buttonName = "Новый навык##" + ((group == nullptr) ? "Groupless" : group->name);
				if (ImGui::Button(buttonName.c_str(), {TEXT_BASE_WIDTH * 14, TEXT_BASE_HEIGHT * 2}))
				{
					CONSOLE_LOG("Add a new Task: NewSkill");
					m_TaskStack.push_back(ProforientatorTasks::NewSkill);
				}
			}
			if (!groupless)
			{
				ImGui::SameLine();
				{
					std::string buttonName = "Править группу##" + ((group == nullptr) ? "Groupless" : group->name);
					if (ImGui::Button(buttonName.c_str(), {TEXT_BASE_WIDTH * 14, TEXT_BASE_HEIGHT * 2}))
					{
						m_CurrentMode = ProforientatorMode::EditSkillGroup;
						CONSOLE_LOG("Enter EditSkillGroup mode");
					}
				}
			}
		}
	}
	
	void Proforientator::TextColoredSkillName(Skill* skill)
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
	
	void Proforientator::TextColoredSkillLevelDescription(int level)
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

	App* CreateApplication()
	{
		Proforientator* app = new Proforientator();
		app->SetFPS(SAVANNAH_FPS60);
		return app;
	}
}
