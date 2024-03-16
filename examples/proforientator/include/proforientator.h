#include <include/savannah/savannah.h>

#include "./yaml_wrapper.h"
#include "./logger.h"

// basically, commands to process
enum class ProforientatorTasks : int
{
	NewSkill					= 0, // [+] create a new skill and prepare to edit it
	CancelNewSkill				= 1, // [+] cancel the new skill editing
	AddSkill 					= 2, // [+] add the new skill to skills registry
	EditSkill 					= 3, // [+] edit a skill in registry
	DeleteSkill 				= 4, // [+] delete a skill from registry
	
//	NewSkillRequirement			= 5, // NOT USED
//	CancelNewSkillRequirement	= 6, // NOT USED
	AddSkillRequirement 		= 7, // add a requirement to skill requirements set
	EditSkillRequirement 		= 8, // change level of a requirement
	RemoveSkillRequirement 		= 9, // remove a requirement from a skill requirements set (we do not delete anything here!)
	
	NewSkillGroup				= 10, // create a new skill group and prepare to edit it
	CancelNewSkillGroup			= 11, // cancel the new skill group editing
	AddSkillGroup 				= 12, // add a skill group to registry
	EditSkillGroup 				= 13, // edit a skill group in registry
	DeleteSkillGroup 			= 14, // delete a skill group from registry
	
	Idle						= 15, // [+] wait for user anything
	
	LoadData					= 16, // [+] load a database
	ReloadData					= 17, // [+] unload the database and load it back again
	UpdateData					= 18, // when should it be done?
	SaveData					= 19, // [+] save the database
	
	Exit						= 20  // [+]
};

// basically, states which define what to draw on screen
enum class ProforientatorMode : int
{
	NewSkill					= 0, // [+]
	EditSkill					= 1, // [+]
	
//	NewSkillRequirement			= 2, // NOT USED
	EditSkillRequirement		= 3, //
	
	NewSkillGroup				= 4, // 
	EditSkillGroup				= 5, // 
	
	NewJob						= 6, // 
	EditJob						= 7, // 
	AdviseJob					= 8, //
	
	Idle						= 9 // [+]
};

namespace Savannah 
{
	class Proforientator : public App 
	{
	public:
		Proforientator();
		~Proforientator();
		
		// void PreSetup() override;
		void SetupResources() override;
		void Logic() override;
		void GUIContent() override;
		
	private:
		std::string m_SkillsFile = "../examples/proforientator/data/skillsDB.txt";
		std::vector<std::string> m_LevelDescription = {};
		SkillRegistry* m_SkillsRegistry = nullptr;
		Skill* m_SkillSelected = nullptr; // selection from one of skills tables
		Skill* m_SkillRequirementSelected = nullptr; // selected in EditRequirements mode
		Skill* m_NewSkill = nullptr; // created in NewSkill mode
		Skill* m_EditSkill = nullptr; // created in EditSkill mode to separate from the selected one
		SkillGroup* m_SkillGroupSelected = nullptr; // specifically selected to be edited or deleted
		SkillGroup* m_NewGroup = nullptr; // the one being created or not to be saved at all
		SkillGroup* m_EditGroup = nullptr; // created in EditSkill mode to separate from the selected one
		YamlWrapper* m_YAMLWrapperObject = nullptr;
		bool m_ChangesInDatabase = false;
		ProforientatorMode m_CurrentMode = ProforientatorMode::Idle;
		ProforientatorTasks m_CurrentTask = ProforientatorTasks::Idle;
		std::vector<ProforientatorTasks> m_TaskStack = {};
		float TEXT_BASE_WIDTH = 0.0f;
		float TEXT_BASE_HEIGHT = 0.0f;
		
		// media resources
		OpenGLTexture2D* m_NewSkillsGroupIcon = nullptr;
		
		void LoadDatabase(const std::string& file);
		void SaveDatabase(const std::string& file);
		void UnloadDatabase();
		void ReloadDatabase();
		void CopySkillSelectedToEditSkill();
		void CopySkillGroupSelectedToEditGroup();
		void ShowMainMenu();
		void ShowLogo();
		void ShowContent();
		void ShowSkillRequirementsTable();
		void ShowEditSkillTable();
		void ShowEditSkillGroupTable();
		void ShowEditSkillRequirementsTable();
		void ShowSkillsTable(const std::string& groupName);
		void TextColoredSkillName(Skill* skill);
		void TextColoredSkillLevelDescription(int level);
	};
}
