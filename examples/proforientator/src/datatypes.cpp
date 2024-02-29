#include <datatypes.h>
#include <logger.h>

namespace Proforientator
{
	Skill::Skill(const std::string& _name, const std::string& _group, uint32_t _level)
		: name(_name), group(_group), level(_level) 
	{
	};
	
	SkillGroup::SkillGroup()
	{
	}
	
	SkillGroup::~SkillGroup()
	{
		for (auto m : children)
		{
			m = nullptr;
		}
	}
	
	SkillRegistry::SkillRegistry()
	{
	}
	
	SkillRegistry::~SkillRegistry()
	{
		for (auto m : m_SkillsRegistry)
		{
			delete m;
			m = nullptr;
		}
		
		for (auto m : m_GroupsRegistry)
		{
			delete m.second;
			m.second = nullptr;
		}
	}
	
	Skill* SkillRegistry::NewSkill(const std::string& name, const std::string& group, uint32_t level)
	{
		Skill* skill = new Skill(name, group, level);
		AddSkill(skill);
		return skill;
	}
	
	void SkillRegistry::AddSkill(Skill* skill)
	{
		m_SkillsRegistry.push_back(skill);
		
		if (m_GroupsRegistry.find(skill->group) == m_GroupsRegistry.end())
		{
			m_GroupsRegistry[skill->group] = new SkillGroup();
			m_GroupsRegistry[skill->group]->name = skill->group;
		} else {
			m_GroupsRegistry[skill->group]->children.push_back(skill);
		}
	}
	
	void SkillRegistry::RemoveSkill(Skill* skill)
	{
		for (auto it = m_GroupsRegistry[skill->group]->children.begin();
			it != m_GroupsRegistry[skill->group]->children.end();
			it++)
		{
			if (*it == skill)
			{
				*it = nullptr;
				break;
			}
		}
		
		for (auto it = m_SkillsRegistry.begin(); it != m_SkillsRegistry.end(); it++)
		{
			if (*it == skill)
			{
				*it = nullptr;
				m_SkillsRegistry.erase(it);
				break;
			}
		}
	}
	
	void SkillRegistry::RemoveSkill(const std::string& name)
	{
		std::string group = "";
		for (auto it = m_SkillsRegistry.begin(); it != m_SkillsRegistry.end(); it++)
		{
			if ((*it)->name == name)
			{
				group = (*it)->group;
				for (auto itg = m_GroupsRegistry[group]->children.begin();
					itg != m_GroupsRegistry[group]->children.end();
					itg++)
				{
					if ((*itg)->name == name)
					{
						*itg = nullptr;
						break;
					}
				}
			}
			*it = nullptr;
			m_SkillsRegistry.erase(it);
			break;
		}
	}
	
	void SkillRegistry::AddGroup(SkillGroup* group)
	{
		if (m_GroupsRegistry.find(group->name) == m_GroupsRegistry.end())
		{
			m_GroupsRegistry[group->name] = new SkillGroup;
			m_GroupsRegistry[group->name]->name = group->name;
		} else {
			CONSOLE_LOG("Couldn't add a group - it already existed in the registry.");
		}
	}
	
	std::unordered_map<std::string, SkillGroup*>& SkillRegistry::GetGroups()
	{
		return m_GroupsRegistry;
	}
	
	Job::Job(const std::vector<Skill*>& reqs)
		: m_Requirements(reqs) 
	{
	};
	
	Job::~Job()
	{
		for (auto it = m_Requirements.begin(); it != m_Requirements.end(); it++)
		{
			delete *it;
			*it = nullptr;
		}
	}
	
	void Job::AddSkill(Skill* skill)
	{
		m_Requirements.push_back(skill);
	}
	
	void Job::RemoveSkill(Skill* skill)
	{
		for (auto it = m_Requirements.begin(); it != m_Requirements.end(); it++)
		{
			if (*it == skill)
			{
				delete *it;
				m_Requirements.erase(it);
				break;
			}
		}
	}
	
	void Job::RemoveSkill(const std::string& name)
	{
		for (auto it = m_Requirements.begin(); it != m_Requirements.end(); it++)
		{
			if ((*it)->name.c_str() == name.c_str())
			{
				delete *it;
				m_Requirements.erase(it);
				break;
			}
		}
	}
}
