#include "../include/datatypes.h"
#include "../include/logger.h"

SkillRequirementSet::SkillRequirementSet()
{
}

SkillRequirementSet::~SkillRequirementSet()
{
	for (int i = 0; i < m_Requirements.size(); i++)
	{
		delete m_Requirements[i];
		m_Requirements[i] = nullptr;
	}
}

void SkillRequirementSet::AddRequirement(const SkillRequirement& req)
{
	SkillRequirement* requirement = new SkillRequirement();
	requirement->name = req.name;
	requirement->level = req.level;
	m_Requirements.push_back(requirement);
}

void SkillRequirementSet::AddRequirement(SkillRequirement* skill)
{
	m_Requirements.push_back(skill);
}

void SkillRequirementSet::RemoveRequirement(SkillRequirement* skill)
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

void SkillRequirementSet::RemoveRequirement(const std::string& name)
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

std::vector<SkillRequirement*>& SkillRequirementSet::GetRequirementsArray()
{
	return m_Requirements;
}

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
	for (auto m : m_SkillsWithoutGroup)
	{
		m = nullptr;
	}
	
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
	bool skillExists = false;
	for (auto m : m_SkillsRegistry)
	{
		if (m == skill)
		{
			skillExists = true;
			CONSOLE_LOG(skill->name, " : ", skill->level, " - exists in the registry.");
			break;
		}
	}
	if (!skillExists)
	{
		m_SkillsRegistry.push_back(skill);
		AddSkillToGroup(skill, skill->group);
	} else {
		CONSOLE_LOG("Couldn't add a skill \"", skill->name, "\": already exists in the registry.");
	}
}

void SkillRegistry::AddSkillToGroup(Skill* skill, const std::string& name)
{
	skill->group = name;
	
	if (skill->group != "Groupless")
	{
		if (m_GroupsRegistry.find(skill->group) == m_GroupsRegistry.end())
		{
			CONSOLE_LOG("There is no group called \"\"", skill->group);
			SkillGroup* group = new SkillGroup();
			group->name = skill->group;
			AddGroup(group);
		} 
		
		bool alreadyExists = false;
		for (auto m : m_GroupsRegistry[skill->group]->children)
		{
			if (m == skill)
			{
				alreadyExists = true;
			}
			break;
		}
		if (!alreadyExists)
		{
			m_GroupsRegistry[skill->group]->children.push_back(skill);
		} else {
			CONSOLE_LOG("Couldn't add a skill to group \"", skill->group, "\": already exists there;")
		}
	} else {
		m_SkillsWithoutGroup.push_back(skill);
	}
}

// It is important than skill name in requirement matches the skill name in the registry
Skill* SkillRegistry::FindSkill(const std::string& name)
{
	Skill* skill = nullptr;
	for (auto it = m_SkillsRegistry.begin(); it != m_SkillsRegistry.end(); it++)
	{
		if ((*it)->name.compare(name) == 0)
		{
			skill = *it;
			break;
		}
	}
	return skill;
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
			m_GroupsRegistry[skill->group]->children.erase(it);
			break;
		}
	}
	
	for (auto it = m_SkillsRegistry.begin(); it != m_SkillsRegistry.end(); it++)
	{
		if (*it == skill)
		{
			delete *it;
			// *it = nullptr;
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

void SkillRegistry::RemoveSkillFromGroup(Skill* skill)
{
	for (auto itg = m_GroupsRegistry[skill->group]->children.begin();
		itg != m_GroupsRegistry[skill->group]->children.end();
		itg++)
	{
		if ((*itg)->name == skill->name)
		{
			*itg = nullptr;
			skill->group = "Groupless";
			break;
		}
	}
}

void SkillRegistry::NewGroup(const std::string& name)
{
	bool groupAlreadyExists = false;
	
	for (int i = 0; i < m_GroupsNames.size(); i++)
	{
		if (m_GroupsNames[i] == name)
		{
			groupAlreadyExists = true;
			CONSOLE_LOG("Couldn't add a group \"", name, "\": already exists in the vector registry.");
			break;
		}
	}
	
	if (!groupAlreadyExists)
	{
		m_GroupsNames.push_back(name);
		
		if (m_GroupsRegistry.find(name) == m_GroupsRegistry.end())
		{
			CONSOLE_LOG("Couldn't add a group \"", name, "\": already exists in the map registry.");
		}
		
		if (!groupAlreadyExists)
		{
			m_GroupsRegistry[name] = new SkillGroup;
			m_GroupsRegistry[name]->name = name;
			return;
		}
	}
}

void SkillRegistry::AddGroup(SkillGroup* group)
{
	bool groupAlreadyExists = false;
	
	for (int i = 0; i < m_GroupsNames.size(); i++)
	{
		if (m_GroupsNames[i] == group->name)
		{
			groupAlreadyExists = true;
			CONSOLE_LOG("Couldn't add a group \"", group->name, "\": already exists in the vector registry.");
			break;
		}
	}
	
	if (!groupAlreadyExists)
	{
		m_GroupsNames.push_back(group->name);
		
		if (m_GroupsRegistry.find(group->name) != m_GroupsRegistry.end())
		{
			groupAlreadyExists = true;
			CONSOLE_LOG("Couldn't add a group \"", group->name, "\": already exists in the map registry.");
		}
		
		if (!groupAlreadyExists)
		{
			m_GroupsRegistry[group->name] = group;
			return;
		}
	}
}

void SkillRegistry::SortGroups()
{
	for (auto groupIt = m_GroupsRegistry.begin(); groupIt != m_GroupsRegistry.end(); groupIt++)
	{
		SkillGroup* group = (*groupIt).second;
		// sort skills in the group
		Sort(group->children);
	}
}

void SkillRegistry::SortGroup(SkillGroup* group)
{
	Sort(group->children);
}

void SkillRegistry::Sort(std::vector<Skill*>& source)
{
	  //////////////////////////////////////////////
	 // Merge Sort  ///////////////////////////////
	//////////////////////////////////////////////
	std::vector<Skill*> temp = source;
	
	// characteristics of intervals
	int length = 1; // it starts from 1 and grows to lengthMax
	int lengthMax = source.size();
	
	// left part of the interval
	int leftStart = 0;
	int leftIndex = 0;
	int leftEnd = 0;
	
	// right part of the interval
	int rightStart = 0;
	int rightIndex = 0;
	int rightEnd = 0;
	
	int tempIndex = 0; // where we put an item
	
	while (length < lengthMax)
	{
		tempIndex = 0; // reset the position of where we put items
		
		// build one interval and then move it across all the source
		leftStart = 0;
		leftEnd = leftStart + length - 1;
		rightStart = leftEnd + 1;
		rightEnd = leftStart + 2 * length - 1;
		
		while (leftStart < lengthMax) // only process when we have the left part
		{
			if (rightStart >= lengthMax)
			{
				break; // don't process the case where we have no right part
			}
			
			if (rightEnd >= lengthMax)
			{
				rightEnd = lengthMax - 1; // always fit the array size
			}
			
			// set up indexes in both parts of the interval...
			leftIndex = leftStart;
			rightIndex = rightStart;
			tempIndex = leftStart; // ...and the writing place as well
			
			// sort things in the interval
			while ((leftIndex <= leftEnd) && (rightIndex <= rightEnd)) // compare items
			{ 
				if (source[rightIndex]->level <= source[leftIndex]->level)
				{
					temp[tempIndex] = source[leftIndex];
					leftIndex++;
					tempIndex++;
				} else {
					temp[tempIndex] = source[rightIndex];
					rightIndex++;
					tempIndex++;
				}
			}
			
			// unprocessed part is considered sorted by default, and there are
			// two special situations possible:
			while (leftIndex <= leftEnd) // 1) no more items left on the right;
			{
				temp[tempIndex] = source[leftIndex];
				leftIndex++;
				tempIndex++;
			}
			
			while (rightIndex <= rightEnd) // 2) no more items left on the left
			{
				temp[tempIndex] = source[rightIndex];
				rightIndex++;
				tempIndex++;
			}
			
			// move the interval
			leftStart += 2*length;
			leftEnd += 2*length;
			rightStart += 2*length;
			rightEnd += 2*length;
		}
		
		// update the source
		for (int i = 0; i < source.size(); i++)
		{
			source[i] = temp[i];
		}
		
		length = length*2; // increase the interval length
	}
	
	// clean the temporary array
	for (int i = 0; i < temp.size(); i++)
	{
		temp[i] = nullptr;
	}
}

std::map<std::string, SkillGroup*>& SkillRegistry::GetGroups()
{
	return m_GroupsRegistry;
}

SkillGroup* SkillRegistry::GetGroupByName(const std::string& groupName)
{
	return m_GroupsRegistry[groupName];
}

std::vector<std::string>& SkillRegistry::GetGroupsNames()
{
	return m_GroupsNames;
}

std::vector<Skill*>& SkillRegistry::GetGrouplessSkills()
{
	return m_SkillsWithoutGroup;
}

std::string SkillRegistry::GetRequirements(Skill* skill)
{
	std::string out = "";
	if (skill != nullptr)
	{
		int requirementsTotal = (skill->GetRequirementsArray()).size();
		for (int i = 0; i < requirementsTotal; i++)
		{
			out += (skill->GetRequirementsArray())[i]->name;
			if (i < (requirementsTotal-1))
			{
				out += ", ";
			}
		}
	}
	return out;
}

Job::Job(const std::vector<SkillRequirement*>& reqs) 
{
	m_Requirements = reqs;
};

Job::~Job()
{
	for (auto it = m_Requirements.begin(); it != m_Requirements.end(); it++)
	{
		delete *it;
		*it = nullptr;
	}
}
