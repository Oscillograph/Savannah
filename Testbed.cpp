#include "Savannah.h"

#include "Savannah/Vendor/nlohmann/json.hpp"

class Tree {
public:
	Tree(const std::string& description = "")
	{
		m_Contents = description.c_str();
		m_Branches = {};
		m_BranchMap = {};
	}
	
	// Access a branch by name
	inline Tree& operator [] (const std::string& name)
	{
		if (m_BranchMap.count(name.c_str()) == 0) // if it doesn't exist
		{
			m_BranchMap[name.c_str()] = m_BranchMap.size(); // register it in the branch map
			m_Branches.push_back( {name, Tree()} ); // create a new blank branch
		}
		return m_Branches[m_BranchMap[name.c_str()]].second; // return the branch
	}
	
	// Access a branch by number
	inline std::pair<std::string, Tree>& operator [] (const uint32_t& number)
	{
		if (m_Branches.size() > number)
		{
			return m_Branches[number];
		} else {
			return m_Branches[m_Branches.size()-1];
		}
	}
	
	inline Tree& operator=(const std::string& description)
	{
		if (description.size() > 0)
			m_Contents = description.c_str();
		return *this;
	}
	
	// get contents
	inline std::string& str() { return m_Contents; }
	inline const char* c_str() { return m_Contents.c_str(); }
	
	inline int CountChildren() { return m_Branches.size(); }
	
	// serialize into a string
	inline std::string Serialize(const std::string& root = "Default Tree Name", int level = 0)
	{
		std::stringstream text;
		const char* spaces = AddSpaces(level);
		text << spaces << root.c_str() << "\n";
		text << spaces << "[" << m_Contents.c_str() << "]\n";
		
		uint32_t childrenCount = CountChildren();
		if (childrenCount > 0)
		{
			text << spaces << "{\n";
			
			for (uint32_t i = 0; i < childrenCount; i++)
			{
				text << m_Branches[i].second.Serialize(m_Branches[i].first, level+1);
			}
			text << spaces << "}\n";
			
		}
		
		return text.str();
	}
	
	// serialize a node into a string
	inline nlohmann::json SerializeJSON(const std::string& root = "Default Tree Name", int level = 0)
	{
		nlohmann::json jsonData;
		jsonData["Name"] = root;
		jsonData["Description"] = m_Contents.c_str();
		
		uint32_t childrenCount = CountChildren();
		if (childrenCount > 0)
		{
			for (uint32_t i = 0; i < childrenCount; i++)
			{
				jsonData["Branches"][m_Branches[i].first] = m_Branches[i].second.SerializeJSON(m_Branches[i].first, level+1);
			}
		}
		
		return jsonData;
	}
	
	// reconstruct a Tree from a string
	inline void Deserialize(const std::string& source)
	{
		// read a character
		// if it is a '{' then
		//		read characters until "\n"
		//		trim spaces or specially chosen characters from the left
		//		collect a child and append it to the tree
		// 		repeat until a character is '}'
	}
	
	// reconstruct a Tree from a json object
	inline void DeserializeJSON(const nlohmann::json& j)
	{
		
	}
	
private:
	std::string m_Contents; // text content of a node - its description
	std::vector<std::pair<std::string, Tree>> m_Branches;
	std::unordered_map<std::string, size_t> m_BranchMap;
	
	// utility method
	inline const char* AddSpaces(int level = 0)
	{
		char symbol = '	';
		std::string spaces = "";
		for (int i = 0; i < level; i++)
		{
			spaces += symbol;
		}
		return spaces.c_str();
	}
};

namespace Savannah {
	class Testbed : public App {
	public:
		Testbed(){
			StoryClever["Содержание"] = "Характеристика сюжета и ГГ, проблематика, тема и пр.";
			StoryClever["Содержание"]["Действие"] = "";
			StoryClever["Содержание"]["Действие"]["Приключения"] = "";
			StoryClever["Содержание"]["Действие"]["Приключения"]["Природа"] = "";
			StoryClever["Содержание"]["Действие"]["Приключения"]["Лабиринт"] = "";
			StoryClever["Содержание"]["Действие"]["Приключения"]["Чудовище"] = "";
			StoryClever["Содержание"]["Действие"]["Приключения"]["Судный день"] = "";
			StoryClever["Содержание"]["Действие"]["Дуэль"] = "";
			StoryClever["Содержание"]["Действие"]["Дуэль"]["Жертва охоты"] = "";
			StoryClever["Содержание"]["Действие"]["Дуэль"]["Столкновение"] = "";
			StoryClever["Содержание"]["Действие"]["Дуэль"]["Месть"] = "";
			StoryClever["Содержание"]["Действие"]["Дуэль"]["Беспринципность"] = "";
			StoryClever["Содержание"]["Действие"]["Часы"] = "";
			StoryClever["Содержание"]["Действие"]["Часы"]["Выкуп"] = "";
			StoryClever["Содержание"]["Действие"]["Часы"]["Обратный отсчёт"] = "";
			StoryClever["Содержание"]["Действие"]["Часы"]["Воздержание"] = "";
			StoryClever["Содержание"]["Действие"]["Часы"]["Судьба"] = "";
			StoryClever["Содержание"]["Действие"]["Эпик"] = "";
			StoryClever["Содержание"]["Действие"]["Эпик"]["Восстание"] = "";
			StoryClever["Содержание"]["Действие"]["Эпик"]["Заговор"] = "";
			StoryClever["Содержание"]["Действие"]["Эпик"]["Самосуд"] = "";
			StoryClever["Содержание"]["Действие"]["Эпик"]["Спаситель"] = "";
			StoryClever["Содержание"]["Ужасы"] = "";
			StoryClever["Содержание"]["Ужасы"]["Жуткое"] = "";
			StoryClever["Содержание"]["Ужасы"]["Сверхъестественное"] = "";
			StoryClever["Содержание"]["Ужасы"]["Неопределённое"] = "";
			StoryClever["Содержание"]["Преступление"] = "";
			StoryClever["Содержание"]["Преступление"]["Капер"] = "";
			StoryClever["Содержание"]["Преступление"]["Зал суда"] = "";
			StoryClever["Содержание"]["Преступление"]["Разведка / Шпионаж"] = "";
			StoryClever["Содержание"]["Преступление"]["Газета"] = "";
			StoryClever["Содержание"]["Преступление"]["Тюрьма"] = "";
			StoryClever["Содержание"]["Преступление"]["Организация преступления"] = "";
			StoryClever["Содержание"]["Преступление"]["Тайна убийства"] = "";
			StoryClever["Содержание"]["Преступление"]["Тайна убийства"]["Уютный детектив"] = "";
			StoryClever["Содержание"]["Преступление"]["Тайна убийства"]["Детектив-профессионал"] = "";
			StoryClever["Содержание"]["Преступление"]["Тайна убийства"]["Исторический"] = "";
			StoryClever["Содержание"]["Преступление"]["Тайна убийства"]["Полицейская история"] = "";
			StoryClever["Содержание"]["Преступление"]["Тайна убийства"]["Паранормальное"] = "";
			StoryClever["Содержание"]["Преступление"]["Тайна убийства"]["Нуар / Крутой детектив"] = "";
			StoryClever["Содержание"]["Вестерн"] = "";
			StoryClever["Содержание"]["Вестерн"]["Классический"] = "";
			StoryClever["Содержание"]["Вестерн"]["Переходный"] = "";
			StoryClever["Содержание"]["Вестерн"]["Месть"] = "";
			StoryClever["Содержание"]["Вестерн"]["Профессия"] = "";
			StoryClever["Содержание"]["Война"] = "";
			StoryClever["Содержание"]["Война"]["Пропаганда"] = "";
			StoryClever["Содержание"]["Война"]["Братство"] = "";
			StoryClever["Содержание"]["Триллер"] = "";
			StoryClever["Содержание"]["Триллер"]["Закон"] = "";
			StoryClever["Содержание"]["Триллер"]["Военный"] = "";
			StoryClever["Содержание"]["Триллер"]["Серийный убийца"] = "";
			StoryClever["Содержание"]["Триллер"]["Медицинский"] = "";
			StoryClever["Содержание"]["Триллер"]["Разведка / Шпионаж"] = "";
			StoryClever["Содержание"]["Триллер"]["Хичкок"] = "";
			StoryClever["Содержание"]["Триллер"]["Журналистский"] = "";
			StoryClever["Содержание"]["Триллер"]["Политический"] = "";
			StoryClever["Содержание"]["Триллер"]["Финансовый"] = "";
			StoryClever["Содержание"]["Триллер"]["Женщина в беде"] = "";
			StoryClever["Содержание"]["Триллер"]["Ребёнок в беде"] = "";
			StoryClever["Содержание"]["Общество"] = "";
			StoryClever["Содержание"]["Общество"]["Домашнее"] = "";
			StoryClever["Содержание"]["Общество"]["Женское"] = "";
			StoryClever["Содержание"]["Общество"]["Политическое"] = "";
			StoryClever["Содержание"]["Общество"]["Биографическое"] = "";
			StoryClever["Содержание"]["Общество"]["Историческое"] = "";
			StoryClever["Содержание"]["Любовь"] = "";
			StoryClever["Содержание"]["Любовь"]["Ухаживание"] = "";
			StoryClever["Содержание"]["Любовь"]["Свадьба"] = "";
			StoryClever["Содержание"]["Любовь"]["Одержимость"] = "";
			StoryClever["Содержание"]["Представление"] = "";
			StoryClever["Содержание"]["Представление"]["Спорт"] = "";
			StoryClever["Содержание"]["Представление"]["Искусство"] = "";
			StoryClever["Содержание"]["Представление"]["Музыка"] = "";
			StoryClever["Содержание"]["Представление"]["Бизнес"] = "";
			StoryClever["Содержание"]["Состояние"] = "";
			StoryClever["Содержание"]["Состояние"]["Сентименты (трогательное / чувственное)"] = "";
			StoryClever["Содержание"]["Состояние"]["Пафос"] = "";
			StoryClever["Содержание"]["Состояние"]["Трагедия"] = "";
			StoryClever["Содержание"]["Состояние"]["Восхищение"] = "";
			StoryClever["Содержание"]["Мировоззрение"] = "";
			StoryClever["Содержание"]["Мировоззрение"]["Образование"] = "";
			StoryClever["Содержание"]["Мировоззрение"]["Открытие"] = "";
			StoryClever["Содержание"]["Мировоззрение"]["Взросление"] = "";
			StoryClever["Содержание"]["Мировоззрение"]["Прозрение"] = "";
			StoryClever["Содержание"]["Мораль"] = "";
			StoryClever["Содержание"]["Мораль"]["Наказание"] = "";
			StoryClever["Содержание"]["Мораль"]["Искупление"] = "";
			StoryClever["Содержание"]["Мораль"]["Испытание"] = "";
			StoryClever["Содержание"]["Мораль"]["Испытание"]["Триумф"] = "";
			StoryClever["Содержание"]["Мораль"]["Испытание"]["Поражение"] = "";
			
			StoryClever["Длительность"] = "";
			StoryClever["Длительность"]["Короткая"] = "";
			StoryClever["Длительность"]["Средняя"] = "";
			StoryClever["Длительность"]["Длинная"] = "";
			
			StoryClever["Структура"] = "";
			StoryClever["Структура"]["Минисюжет"] = "";
			StoryClever["Структура"]["Архисюжет"] = "";
			StoryClever["Структура"]["Антисюжет"] = "";
			
			StoryClever["Реальность (художественный мир)"] = "";
			StoryClever["Реальность (художественный мир)"]["Реализм"] = "";
			StoryClever["Реальность (художественный мир)"]["Фактуализм"] = "";
			StoryClever["Реальность (художественный мир)"]["Фантастика"] = "";
			StoryClever["Реальность (художественный мир)"]["Фантастика"]["Социальная"] = "";
			StoryClever["Реальность (художественный мир)"]["Фантастика"]["Волшебная"] = "";
			StoryClever["Реальность (художественный мир)"]["Фантастика"]["Научная фантастика"] = "";
			StoryClever["Реальность (художественный мир)"]["Фантастика"]["Научная фантастика"]["Альтернативная история"] = "";
			StoryClever["Реальность (художественный мир)"]["Фантастика"]["Научная фантастика"]["Киберпанк"] = "";
			StoryClever["Реальность (художественный мир)"]["Фантастика"]["Научная фантастика"]["Твёрдая наука"] = "";
			StoryClever["Реальность (художественный мир)"]["Фантастика"]["Научная фантастика"]["Мягкая наука"] = "";
			StoryClever["Реальность (художественный мир)"]["Фантастика"]["Научная фантастика"]["Военная"] = "";
			StoryClever["Реальность (художественный мир)"]["Фантастика"]["Научная фантастика"]["Романтическая"] = "";
			StoryClever["Реальность (художественный мир)"]["Фантастика"]["Научная фантастика"]["Космическая опера"] = "";
			StoryClever["Реальность (художественный мир)"]["Фантастика"]["Научная фантастика"]["Постапокалипсис / Дистопия"] = "";
			StoryClever["Реальность (художественный мир)"]["Абсурдизм"] = "";
			
			StoryClever["Стиль"] = "";
			StoryClever["Стиль"]["Драма"] = "";
			StoryClever["Стиль"]["Комедия"] = "";
			StoryClever["Стиль"]["Документалистика"] = "";
			StoryClever["Стиль"]["Литературный"] = "";
			StoryClever["Стиль"]["Литературный"]["Поэзия"] = "";
			StoryClever["Стиль"]["Литературный"]["Минимализм"] = "";
			StoryClever["Стиль"]["Литературный"]["Мета"] = "";
			StoryClever["Стиль"]["Литературный"]["Пост-модерн"] = "";
			StoryClever["Стиль"]["Кино"] = "";
			StoryClever["Стиль"]["Театр"] = "";
			StoryClever["Стиль"]["Мюзикл"] = "";
			StoryClever["Стиль"]["Дневник"] = "";
			StoryClever["Стиль"]["Танцы"] = "";
			StoryClever["Стиль"]["Мультипликация"] = "";
			
			m_InputText = new std::string((StoryClever.SerializeJSON("История")).dump(4));
		}
		
		~Testbed(){
			delete m_InputText;
		}
		
		void GUIContent() override {
			bool p_open = true;
			static bool use_work_area = true;
			// static ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;
			static ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoSavedSettings;
			
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
			
			
			// ***********************************************************************************
			// 		Генератор жанрового описания будущего шедевра (см. инфографику Ш.Койна) 
			// ***********************************************************************************
			if (m_InputText == nullptr)
				m_InputText = new std::string();
			
			if (ImGui::Begin("Example: Fullscreen window", &p_open, flags))
			{
				ImGui::SetNextItemWidth(viewport->WorkSize.x - 10.0f);
				ImGui::InputTextMultiline("##Текстовое поле", m_InputText, {0.0f, 400});
				// ImGui::SameLine();
				// ImGui::SetNextItemWidth(400.0f);
				// ImGui::InputTextMultiline("##Другое поле", m_InputText);
				
				// Generate random genre
				if (ImGui::Button("Сгенерировать жанр"))
				{
					m_InputText->clear();
					srand((uint32_t)(glfwGetTime() * 10000));
					
					for (uint32_t i = 0; i < StoryClever.CountChildren(); i++)
					{
						*m_InputText += StoryClever[i].first;
						*m_InputText += ": ";
						
						int r = rand() % StoryClever[StoryClever[i].first].CountChildren();
						auto branch = StoryClever[StoryClever[i].first][r];
						
						while (r != -1)
						{
							*m_InputText += branch.first;
							if (branch.second.CountChildren() > 0)
							{
								*m_InputText += " > ";
								r = rand() % branch.second.CountChildren();
								branch = branch.second[r];
							} else {
								*m_InputText += "\n";
								r = -1;
							}
						}
					}
				}
			}
			ImGui::End();
			// ****************************************************************************
		}
		
	private:
		std::string* m_InputText = nullptr;
		Tree StoryClever;
	};
	
	App* CreateApplication(){
		return new Testbed();
	}
}
