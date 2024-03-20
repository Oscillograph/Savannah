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
		
		NewTask(ProforientatorTasks::LoadData);
		CONSOLE_LOG("Add a new Task: LoadData");
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
		delete m_EditGroup;
		m_EditGroup = nullptr;
		
		delete m_NewSkillsGroupIcon;
		m_NewSkillsGroupIcon = nullptr;
	}
	
	void Proforientator::SetupResources()
	{
		m_NewSkillsGroupIcon = new OpenGLTexture2D("../examples/proforientator/data/img/icon_add.png");
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
					m_CurrentMode = ProforientatorMode::Idle;
					CONSOLE_LOG("m_SkillSelected equals nullptr now.");
				}
				break;
			case ProforientatorTasks::AddSkillRequirement:
				{}
				break;
			case ProforientatorTasks::EditSkillRequirement:
				{}
				break;
			case ProforientatorTasks::RemoveSkillRequirement:
				{}
				break;
			case ProforientatorTasks::NewSkillGroup:
				{
					m_SkillGroupSelected = new SkillGroup();
					m_SkillGroupSelected->name = "Новая группа";
					m_SkillGroupSelected->children = {};
					CopySkillGroupSelectedToEditGroup();

					m_CurrentMode = ProforientatorMode::NewSkillGroup;
					CONSOLE_LOG("Enter NewSkillGroup mode");
				}
				break;
			case ProforientatorTasks::CancelNewSkillGroup:
				{
					delete m_SkillGroupSelected;
					m_SkillGroupSelected = nullptr;
					delete m_EditGroup;
					m_EditGroup = nullptr;

					m_CurrentMode = ProforientatorMode::Idle;
					CONSOLE_LOG("Enter Idle mode");
				}
				break;
			case ProforientatorTasks::AddSkillGroup:
				{
					if (m_SkillsRegistry->GetGroups().find(m_EditGroup->name) == m_SkillsRegistry->GetGroups().end())
					{
						// update the group name
						m_SkillGroupSelected->name = m_EditGroup->name;
						
						// update group names registry
						m_SkillsRegistry->GetGroupsNames().push_back(m_SkillGroupSelected->name);
						
						// update the map in the skill registry
						m_SkillsRegistry->GetGroups()[m_SkillGroupSelected->name] = m_SkillGroupSelected;
						
						// update the EditGroup object
						CopySkillGroupSelectedToEditGroup();
						
						m_ChangesInDatabase = true;
					} else {
						m_ErrorFlag = true;
						m_ErrorMessage = "Группа с таким именем уже существует!";
					}
				}
				break;
			case ProforientatorTasks::EditSkillGroup:
				{
					if (m_EditGroup != nullptr)
					{
						std::string oldName = m_SkillGroupSelected->name;

						// update group names registry
						CONSOLE_LOG("Update group names registry...");
						std::vector<std::string>& groupsNames = m_SkillsRegistry->GetGroupsNames();
						for (int i = 0; i < groupsNames.size(); i++)
						{
							if (groupsNames[i] == oldName)
							{
								groupsNames[i] = m_EditGroup->name;
								break;
							}
						}
						
						// update the map in the skill registry
						CONSOLE_LOG("Update the map in the skill registry...");
						std::map<std::string, SkillGroup*>& groupRegistry = m_SkillsRegistry->GetGroups();
						auto itg = groupRegistry.find(oldName);
						(*itg).second = nullptr;
						groupRegistry.erase(itg);
						groupRegistry[m_EditGroup->name] = m_SkillGroupSelected;
						
						// update group names in skills
						CONSOLE_LOG("Update group names in skills...");
						for (int i = 0; i < m_SkillGroupSelected->children.size(); i++)
						{
							m_SkillGroupSelected->children[i]->group = m_EditGroup->name;
						}
						
						// update the group name
						CONSOLE_LOG("Update the group name...");
						m_SkillGroupSelected->name = m_EditGroup->name;
						
						// update the EditGroup object
						CONSOLE_LOG("Update EditGroup object...");
						CopySkillGroupSelectedToEditGroup();
						
						m_ChangesInDatabase = true;
						CONSOLE_LOG("Skill group \"", oldName, "\" updated to \"", m_EditGroup->name, "\".");
						m_CurrentMode = ProforientatorMode::EditSkillGroup;
						CONSOLE_LOG("Enter EditGroup mode");
					} else {
						CONSOLE_LOG("Called Task EditSkillGroup when m_EditGroup is nullptr.");
					}
				}
				break;
			case ProforientatorTasks::DeleteSkillGroup:
				{
					// delete skills from the group
					std::vector<Skill*>& skills = m_SkillGroupSelected->children;
					Skill* skill = nullptr;
					for (auto it = skills.begin(); it != skills.end(); )
					{
						skill = *it;
						(*it)->group = "Groupless";
						m_SkillsRegistry->AddSkillToGroup(*it, "Groupless");
						*it = nullptr;
						it = skills.erase(it);
					}
					
					// delete from the group names registry
					std::vector<std::string>& names = m_SkillsRegistry->GetGroupsNames();
					for (auto it = names.begin(); it != names.end(); it++)
					{
						if ((*it) == m_SkillGroupSelected->name)
						{
							names.erase(it);
							break;
						}
					}
					
					// delete from the map in the skill registry
					std::map<std::string, SkillGroup*>& map = m_SkillsRegistry->GetGroups();
					auto itg = map.find(m_SkillGroupSelected->name);
					if (itg != map.end())
					{
						map.erase(itg);
					}
					
					// update the EditGroup object
					delete m_SkillGroupSelected;
					m_SkillGroupSelected = nullptr;
					delete m_EditGroup;
					m_EditGroup = nullptr;
					
					m_ChangesInDatabase = true;
					m_CurrentMode = ProforientatorMode::Idle;
				}
				break;
			case ProforientatorTasks::Idle:
				// do nothing
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
			case ProforientatorTasks::UpdateData:
				break;
			case ProforientatorTasks::SaveData:
				{
					SaveDatabase(m_SkillsFile);
				}
				break;
			case ProforientatorTasks::Exit:
				{
					doExit = true;
				}
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
					}
					
					ImGui::TableSetColumnIndex(i);
					if (ImGui::BeginTable("##NewSkillGroupButton", 1, 0, {TEXT_BASE_WIDTH * 30, TEXT_BASE_HEIGHT * 10}))
					{
						ImGui::TableNextRow();
						ImGui::TableSetColumnIndex(0);
						if (ImGui::ImageButton("##AddSkillGroup", (ImTextureID)m_NewSkillsGroupIcon->GetID(), {TEXT_BASE_WIDTH * 25, TEXT_BASE_HEIGHT * 10}, ImVec2(0, 0), ImVec2(1, 1)))
						{
							NewTask(ProforientatorTasks::NewSkillGroup);
							CONSOLE_LOG("Add a new Task: NewSkillGroup");
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
	
	void Proforientator::NewTask(ProforientatorTasks task)
	{
		m_ErrorFlag = false;
		m_ErrorMessage = "";
		m_TaskStack.push_back(task);
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
		m_SkillGroupSelected = nullptr;
		UnloadDatabase();
		LoadDatabase(m_SkillsFile);
		m_ChangesInDatabase = false;
	}
	
	void Proforientator::CopySkillSelectedToEditSkill()
	{
		delete m_EditSkill;
		m_EditSkill = new Skill(m_SkillSelected);
	}
	
	void Proforientator::CopySkillGroupSelectedToEditGroup()
	{
		delete m_EditGroup;
		m_EditGroup = new SkillGroup(m_SkillGroupSelected);
	}
	
	void Proforientator::ProcessSkillRequirements()
	{
		if (m_SkillSelected != nullptr)
		{
			m_SkillRequirementsMet.clear();
			m_SkillRequirementsNotMet.clear();
			SkillRequirement* currentRequirement = nullptr;
			
			for (int i = 0; i < m_SkillSelected->GetRequirementsArray().size(); i++)
			{
				currentRequirement = m_SkillSelected->GetRequirementsArray()[i];
				Skill* skillRequired = m_SkillsRegistry->FindSkill(currentRequirement->name);
				
				if (skillRequired != nullptr)
				{
					if (currentRequirement->level <= skillRequired->level)
					{
						m_SkillRequirementsMet.push_back(currentRequirement);
						continue;
					}
				}
				
				m_SkillRequirementsNotMet.push_back(currentRequirement);
			}
		}
		
		CONSOLE_LOG("Reqs Met: ", m_SkillRequirementsMet.size(), "; Reqs Not Met: ", m_SkillRequirementsNotMet.size());
	}
	
	void Proforientator::ShowMainMenu()
	{
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("Файл"))
			{
				if (ImGui::MenuItem("Перезагрузить БД")) 
				{ 
					NewTask(ProforientatorTasks::ReloadData);
					CONSOLE_LOG("Add a new Task: ReloadData");
				}
				ImGui::MenuItem(" ");
				if (ImGui::MenuItem("Выход")) 
				{
					NewTask(ProforientatorTasks::Exit);
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
		switch (m_CurrentMode)
		{
		case ProforientatorMode::EditSkill:
		case ProforientatorMode::NewSkill:
			{
				if (m_SkillSelected != nullptr)
				{
					ShowEditSkillTable();
					TextColoredSkillLevelDescription((int)m_EditSkill->level);
					ImGui::Text("");
					ShowSkillRequirementsTable();
				}
			}
			break;
		case ProforientatorMode::EditSkillRequirement:
			{
				if (m_SkillSelected != nullptr)
				{
					ShowEditSkillRequirementsTable();
				}
			}
			break;
		case ProforientatorMode::NewSkillGroup:
		case ProforientatorMode::EditSkillGroup:
			{
				if (m_SkillGroupSelected != nullptr)
				{
					ShowEditSkillGroupTable();
				}
			}
			break;
		default:
			break;
		}
	}
	
	void Proforientator::ShowSkillRequirementsTable()
	{
		if (ImGui::BeginTable("##Требования", 3, ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollY))
		{
			ImGui::TableSetupColumn("Полученные навыки##passed", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_WIDTH * 35);
			ImGui::TableSetupColumn("Нужны ещё##notpassed", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_WIDTH * 35);
			ImGui::TableSetupColumn("##empty", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_WIDTH * 10);
			ImGui::TableHeadersRow();
			
			ImGui::TableNextRow();
			std::string upToLevel = "";
			
			ImGui::TableSetColumnIndex(0); // requirements met
			for (int i = 0; i < m_SkillRequirementsMet.size(); i++)
			{
				std::string& reqName = m_SkillRequirementsMet[i]->name;
				uint32_t& reqLevel = m_SkillRequirementsMet[i]->level;
				Skill* skillStored = m_SkillsRegistry->FindSkill(reqName);
				
				sprintf((char*)upToLevel.c_str(), " (%d)", skillStored->level);
				TextColoredSkillName(skillStored);
				ImGui ::SameLine();
				ImGui::Text(upToLevel.c_str());
			}
			
			ImGui::TableSetColumnIndex(1); // requirements not met
			for (int i = 0; i < m_SkillRequirementsNotMet.size(); i++)
			{
				std::string& reqName = m_SkillRequirementsNotMet[i]->name;
				uint32_t& reqLevel = m_SkillRequirementsNotMet[i]->level;
				Skill* skillStored = m_SkillsRegistry->FindSkill(reqName);
				
				sprintf((char*)upToLevel.c_str(), " (%d)", reqLevel);
				
				if (skillStored != nullptr)
				{
					TextColoredSkillName(skillStored);
				} else {
					ImGui::TextColored({1.0f, 0.2f, 0.2f, 1.0f}, reqName.c_str());
				}
				ImGui ::SameLine();
				ImGui::Text(upToLevel.c_str());
			}
			
			ImGui::TableSetColumnIndex(2);
			if (ImGui::Button("Изменить"))
			{
				m_CurrentMode = ProforientatorMode::EditSkillRequirement;
				CONSOLE_LOG("Enter EditSkillRequirement mode");
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
				std::string buttonName = (m_CurrentMode == ProforientatorMode::NewSkill) ? "Отменить##" : "Удалить##";
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.8f, 0.3f, 0.3f, 1.0f});
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.8f, 0.6f, 0.4f, 1.0f});
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{1.0f, 0.7f, 0.7f, 1.0f});
				if (ImGui::Button(buttonName.c_str()))
				{
					NewTask(ProforientatorTasks::DeleteSkill);
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
						NewTask(ProforientatorTasks::EditSkill);
						CONSOLE_LOG("Add a new Task: EditSkill");
						break;
					case ProforientatorMode::NewSkill:
						NewTask(ProforientatorTasks::AddSkill);
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
		if (m_EditGroup != nullptr)
		{
			if (ImGui::BeginTable("##ПравитьГруппу", 3, ImGuiTableFlags_SizingFixedFit))
			{
				ImGui::TableSetupColumn("##skill", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_WIDTH * 10);
				ImGui::TableSetupColumn("##control", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_WIDTH * 59);
				ImGui::TableSetupColumn("##buttons", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_WIDTH * 11);
				
				{
					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);
					ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
					ImGui::Text("Группа: ");
					
					ImGui::TableSetColumnIndex(1);
					ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
					if (ImGui::InputText("###Name", &(m_EditGroup->name)))
					{
						m_ChangesInDatabase = true;
					}
					
					ImGui::TableSetColumnIndex(2);
					{
						std::string buttonLabel = "";
						switch (m_CurrentMode)
						{
						case ProforientatorMode::NewSkillGroup:
							buttonLabel = "Отмена##";
							break;
						case ProforientatorMode::EditSkillGroup:
							buttonLabel = "Удалить##";
							break;
						default:
							buttonLabel = "ОШИБКА!##";
						}

						ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
						ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.8f, 0.3f, 0.3f, 1.0f});
						ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.8f, 0.6f, 0.4f, 1.0f});
						ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{1.0f, 0.7f, 0.7f, 1.0f});
						if (ImGui::Button(buttonLabel.c_str()))
						{
							switch (m_CurrentMode)
							{
							case ProforientatorMode::NewSkillGroup:
								NewTask(ProforientatorTasks::CancelNewSkillGroup);
								CONSOLE_LOG("Add a new Task: CancelNewSkillGroup");
								break;
							case ProforientatorMode::EditSkillGroup:
								NewTask(ProforientatorTasks::DeleteSkillGroup);
								CONSOLE_LOG("Add a new Task: DeleteSkillGroup");
								break;
							default:
								// do nothing
								CONSOLE_LOG("Unknown behaviour!");
								break;
							}
						}
						ImGui::PopStyleColor(3);
					}
				}
				
				{
					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);
//					ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
//					ImGui::Text("Порядок: ");
					
					ImGui::TableSetColumnIndex(1);
//					ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
//					if (ImGui::InputText("###Order", &(m_EditGroup->name)))
//					{
//						m_ChangesInDatabase = true;
//					}
					
					ImGui::TableSetColumnIndex(2);
					{
						ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
						ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.3f, 0.8f, 0.3f, 1.0f});
						ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.6f, 0.8f, 0.4f, 1.0f});
						ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.7f, 1.0f, 0.7f, 1.0f});
						if (ImGui::Button("Сохранить##"))
						{
							switch (m_CurrentMode)
							{
							case ProforientatorMode::NewSkillGroup:
								NewTask(ProforientatorTasks::AddSkillGroup);
								CONSOLE_LOG("Add a new Task: AddSkillGroup");
								break;
							case ProforientatorMode::EditSkillGroup:
								NewTask(ProforientatorTasks::EditSkillGroup);
								CONSOLE_LOG("Add a new Task: EditSkillGroup");
								break;
							default:
								// do nothing
								CONSOLE_LOG("Unknown behaviour!");
								break;
							}
						}
						ImGui::PopStyleColor(3);
					}
				}
				ImGui::EndTable();
			}
		}
	}
	
	void Proforientator::ShowEditSkillRequirementsTable()
	{
		if (m_SkillSelected != nullptr)
		{
			if (ImGui::BeginTable("##EditSkillRequirementsTable", 3, 0))
			{
				ImGui::TableSetupColumn("##Left", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_WIDTH * 35);
				ImGui::TableSetupColumn("##Middle", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_WIDTH * 10);
				ImGui::TableSetupColumn("##Right", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_WIDTH * 35);
				
				ImGui::TableNextRow(); // top row
				{
					ImGui::TableSetColumnIndex(0); // add a new requirement
					{
						ImGui::InputText("##NewRequirement", &(m_SkillRequirementNew.name));
					}
					ImGui::TableSetColumnIndex(1); // confirm button
					{
						ImGui::Button("< Добавить");
					}
					ImGui::TableSetColumnIndex(2); // blank
				}
				
				ImGui::TableNextRow(); // content row
				{
					ImGui::TableSetColumnIndex(0); // requirements set
					{
						ImGui::BeginTable("##RequirementsSet", 2, 0);
						bool selected = false;
						std::string buf = "";
//						CONSOLE_LOG(m_SkillSelected->GetRequirementsArray().size());
						for (int i = 0; i < m_SkillSelected->GetRequirementsArray().size(); i++)
						{
							std::string& reqName = m_SkillSelected->GetRequirementsArray()[i]->name;
							uint32_t& reqLevel = m_SkillSelected->GetRequirementsArray()[i]->level;
							selected = (reqName == m_SkillRequirementSelectedName) ? true : false;
							
							ImGui::TableNextRow();
							ImGui::TableSetColumnIndex(0);
							if (ImGui::Selectable(reqName.c_str(), &selected, ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_SelectOnNav))
							{
								m_SkillRequirementSelectedName = reqName;
							}
							
							ImGui::TableSetColumnIndex(1);
							buf = "";
							for (int i = 0; i < 10; i++)
							{
								if (i < reqLevel)
								{
									sprintf((char*)(buf.c_str()), "%s+", buf.c_str());
								} else {
									sprintf((char*)(buf.c_str()), "%s ", buf.c_str());
								}
							}
							if (ImGui::Selectable(buf.c_str(), &selected, ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_SelectOnNav))
							{
								m_SkillRequirementSelectedName = reqName;
							}
						}
						ImGui::EndTable();
					}
					ImGui::TableSetColumnIndex(1); // control buttons (<< , >> , LevelSlider)
					ImGui::TableSetColumnIndex(2); // all skills
				}
				
				ImGui::EndTable();
			}
		}
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
					ProcessSkillRequirements();
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
					ProcessSkillRequirements();
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
					NewTask(ProforientatorTasks::NewSkill);
				}
			}
			if (!groupless)
			{
				ImGui::SameLine();
				{
					std::string buttonName = "Править группу##" + group->name;
					if (ImGui::Button(buttonName.c_str(), {TEXT_BASE_WIDTH * 14, TEXT_BASE_HEIGHT * 2}))
					{
//						CONSOLE_LOG("Previous m_SkillGroupSelected value: ", ((m_SkillGroupSelected == nullptr) ? "nullptr" : m_SkillGroupSelected->name));
						m_SkillGroupSelected = group;
						
						m_CurrentMode = ProforientatorMode::EditSkillGroup;
						CONSOLE_LOG("Enter EditSkillGroup mode");
						CopySkillGroupSelectedToEditGroup();
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
