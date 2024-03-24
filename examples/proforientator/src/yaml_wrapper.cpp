#include <savannah/core.h>
#include "../include/yaml_wrapper.h"

YAML::Emitter& operator<<(YAML::Emitter& out, Skill* rhs){
//		out << YAML::Flow;
	out << YAML::BeginMap;
	out << YAML::Key << "Name" << YAML::Value << rhs->name.c_str();
	out << YAML::Key << "Level" << YAML::Value << rhs->level;
	
	out << YAML::Key << "Requirements" << YAML::Value;
	out << YAML::BeginSeq;
	for (int i = 0; i < rhs->GetRequirementsArray().size(); i++)
	{
		out << YAML::BeginMap;
		out << YAML::Key << "Name" << YAML::Value << rhs->GetRequirementsArray()[i]->name;
		out << YAML::Key << "Level" << YAML::Value << rhs->GetRequirementsArray()[i]->level;
		out << YAML::EndMap;
	}
	out << YAML::EndSeq;
	
	out << YAML::EndMap;
	return out;
}

YAML::Emitter& operator<<(YAML::Emitter& out, SkillGroup* rhs)
{
//		out << YAML::Flow;
	out << YAML::BeginMap;
	out << YAML::Key << "SkillsGroup" << YAML::Value << rhs->name;
	out << YAML::Key << "Skills" << YAML::Value;
	
	out << YAML::BeginSeq;
	for (auto it = rhs->children.begin(); it != rhs->children.end(); it++)
	{
		out << *it;
	}
	out << YAML::EndSeq;
	out << YAML::EndMap;
	
	return out;
}

YAML::Emitter& operator<<(YAML::Emitter& out, std::vector<Skill*>& rhs)
{
//		out << YAML::Flow;
	out << YAML::BeginMap;
	out << YAML::Key << "SkillsGroup" << YAML::Value << "Groupless";
	out << YAML::Key << "Skills" << YAML::Value;
	
	out << YAML::BeginSeq;
	for (auto it = rhs.begin(); it != rhs.end(); it++)
	{
		out << *it;
	}
	out << YAML::EndSeq;
	out << YAML::EndMap;
	
	return out;
}

void YamlWrapper::UseSkillRegistry(SkillRegistry* skillRegistry)
{
	m_SkillRegistry = skillRegistry;
}

bool YamlWrapper::LoadDocument(const std::string& filename) // load an existing yaml document from a file in memory
{
	bool success = false;
	m_Contents = FileIO::GetRawText(filename);
	YAML::Node data = YAML::Load(m_Contents);
	if (!data["Database"]){
		SAVANNAH_CONSOLE_LOG("No database node");
		return success;
	}
	
	
	// std::string skillName = data["Skill"].as<std::string>();
	
	YAML::Node skillsGroups = data["Database"];
	
	if (skillsGroups) {
		for (YAML::Node skillsGroup : skillsGroups) {
			std::string skillsGroupName = skillsGroup["SkillsGroup"].as<std::string>();
//			CONSOLE_LOG(skillsGroupName);
			if (skillsGroupName.compare("Groupless") != 0)
			{
				SkillGroup* group = new SkillGroup();
				group->name = skillsGroupName;
				m_SkillRegistry->AddGroup(group);
			}
			
			YAML::Node skillNodes = skillsGroup["Skills"];
			for (YAML::Node skillNode : skillNodes)
			{
				std::string name = skillNode["Name"].as<std::string>();
				uint32_t level = skillNode["Level"].as<int>();
//				CONSOLE_LOG(name, " : ", level);
				Skill* skill = m_SkillRegistry->NewSkill(name, skillsGroupName, level);
				
				YAML::Node requirementNodes = skillNode["Requirements"];
				for (YAML::Node requirementNode : requirementNodes)
				{
					std::string reqName = requirementNode["Name"].as<std::string>();
					uint32_t reqLevel = requirementNode["Level"].as<int>();
					
					skill->AddRequirement({reqName, reqLevel});
				}
			}
		}
//		CONSOLE_TEAL("File parsed.");
		success = true;
		// break; // WHYYYYYYYYYYYYYYY?!!!!!!!!!?!?!?!?!
	}
	return success;
}

bool YamlWrapper::SaveDocument(const std::string& filename) // save a yaml document in a file
{
	YAML::Emitter out;
	out << YAML::BeginMap;
	out << YAML::Key << "Database" << YAML::Value;
	out << YAML::BeginSeq;
	for (auto it = m_SkillRegistry->GetGroups().begin(); it != m_SkillRegistry->GetGroups().end(); it++)
	{
		out << (*it).second;
	};
	out << m_SkillRegistry->GetGrouplessSkills();
	out << YAML::EndSeq;
	out << YAML::EndMap;
	
	return FileIO::WriteRawText(filename, out.c_str());
}
