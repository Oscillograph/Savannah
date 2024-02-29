#include <yaml_wrapper.h>
#include <logger.h>

namespace Proforientator
{
	YAML::Emitter& operator<<(YAML::Emitter& out, Skill* rhs){
//		out << YAML::Flow;
		out << YAML::BeginMap;
		out << YAML::Key << "Name" << YAML::Value << rhs->name.c_str();
		out << YAML::Key << "Level" << YAML::Value << rhs->level;
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
	
	void YamlWrapper::UseSkillRegistry(SkillRegistry* skillRegistry)
	{
		m_SkillRegistry = skillRegistry;
	}
	
	bool YamlWrapper::LoadDocument(const std::string& filename) // load an existing yaml document from a file in memory
	{
		m_Contents = FileIO::GetRawText(filename);
		YAML::Node data = YAML::Load(m_Contents);
		if (!data["Database"]){
			CONSOLE_LOG("No database node");
			return false;
		}
		
		
		// std::string skillName = data["Skill"].as<std::string>();
		
		YAML::Node skillsGroups = data["Database"];
		
		if (skillsGroups) {
			for (YAML::Node skillsGroup : skillsGroups) {
				std::string skillsGroupName = skillsGroup["SkillsGroup"].as<std::string>();
//				CONSOLE_LOG(skillsGroupName);
				SkillGroup* group = new SkillGroup();
				group->name = skillsGroupName;
				m_SkillRegistry->AddGroup(group);
				
				YAML::Node skills = skillsGroup["Skills"];
				for (YAML::Node skill : skills)
				{
					std::string name = skill["Name"].as<std::string>();
					uint32_t level = skill["Level"].as<int>();
//					CONSOLE_LOG(name, " : ", level);
					m_SkillRegistry->NewSkill(name, group->name, level);
				}
			}
		}
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
		out << YAML::EndSeq;
		out << YAML::EndMap;
		
		return FileIO::WriteRawText(filename, out.c_str());
	}
}
