// #include <datatypes.h>
#include <yaml_wrapper.h>
#include <logger.h>
#include <windows.h>
//#include <locale>

int main(int argc, char** argv) {
//	CONSOLE_CAPTION_BLACK("Hello, user!");
//	SetConsoleOutputCP(866);
//	SetConsoleCP(866);
//	setlocale(LC_ALL, "Russian");
//	SetConsoleOutputCP(1251);
	SetConsoleOutputCP(CP_UTF8);
//	system("chcp 65001");
	
	
	// load database
	std::string skillsFile = "../data/skillsDB.txt";
	Proforientator::SkillRegistry skillsRegistry;
	Proforientator::YamlWrapper YAMLWrapperObject;
	YAMLWrapperObject.UseSkillRegistry(&skillsRegistry);
	YAMLWrapperObject.LoadDocument(skillsFile);
	
	// Wait for input.
	
	// Process.
	
	// Display.
	CONSOLE_CAPTION_BLACK("- - - - - - - - - - - - - - - - - ------- - - - - - - - - - - - - - - - - - - -");
	CONSOLE_CAPTION_BLACK("- - - - - - - - - - - Skills map - - - - - - - - - - - -");
	std::string message = "";
	for (auto group = skillsRegistry.GetGroups().begin();
		 group != skillsRegistry.GetGroups().end();
		 group++)
	{
		for (auto skill = (*group).second->children.begin(); skill != (*group).second->children.end(); skill++)
		{
			message = (*skill)->name + ": ";
			
			// padding
			for (int i = message.size(); i < 30; i++)
			{
				message += " ";
			}
			
			// level
			for (int i = 0; i < 10; i++)
			{
				if (i < (*skill)->level)
				{
					message += "+";
				} else {
					message += ".";
				}
			}
			
			// actual dusplay
			if ((*skill)->level >= 8)
			{
				CONSOLE_CYAN(message);
			} else {
				if ((*skill)->level >= 2)
				{
					CONSOLE_WHITE(message);
				} else {
					if ((*skill)->level >= 0)
					{
						CONSOLE_GREY(message);
					}
				}
			}
		}
		
		system("pause");
	}
	CONSOLE_GREY("Skill not aquaried");
	CONSOLE_WHITE("Skill aquired.");
	CONSOLE_CYAN("Skill is almost perfect.");
	
	CONSOLE_CAPTION_BLACK("- - - - - - - - - - - - - - - - - ------- - - - - - - - - - - - - - - - - - - -");
	CONSOLE_CAPTION_BLACK("- - - - - - - - -  Job requirements  - - - - - - - - - -");
	CONSOLE_RED("Skill doesn't match requirements.");
	CONSOLE_YELLOW("Skill matches requirements.");
	CONSOLE_GREEN("Skill excedes requirements.");
	
	// Get back to waiting for input.
	
	// save database
	YAMLWrapperObject.SaveDocument(skillsFile);
	
	if (system(NULL))
	{
		std::string command;
		std::getline(std::cin, command);
		system(command.c_str());
	}
	
	return 0;
}
