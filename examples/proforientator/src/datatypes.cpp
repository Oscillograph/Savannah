#include "../include/datatypes.h"
#include "../include/logger.h"

SkillRequirement::SkillRequirement(const std::string& _name, uint32_t _level)
	: name(_name), level(_level)
{
}

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

SkillRequirement* SkillRequirementSet::GetRequirementByName(const std::string& name)
{
	SkillRequirement* result = nullptr;
	for (int i = 0; i < m_Requirements.size(); i++)
	{
		if (m_Requirements[i]->name == name)
		{
			result = m_Requirements[i];
			break;
		}
	}
	return result;
}

void SkillRequirementSet::SortRequirementsByLevel()
{
	//////////////////////////////////////////////
	// Merge Sort  ///////////////////////////////
	//////////////////////////////////////////////
	std::vector<SkillRequirement*> temp = m_Requirements;
	
	// characteristics of intervals
	int length = 1; // it starts from 1 and grows to lengthMax
	int lengthMax = m_Requirements.size();
	
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
				if (m_Requirements[rightIndex]->level <= m_Requirements[leftIndex]->level)
				{
					temp[tempIndex] = m_Requirements[leftIndex];
					leftIndex++;
					tempIndex++;
				} else {
					temp[tempIndex] = m_Requirements[rightIndex];
					rightIndex++;
					tempIndex++;
				}
			}
			
			// unprocessed part is considered sorted by default, and there are
			// two special situations possible:
			while (leftIndex <= leftEnd) // 1) no more items left on the right;
			{
				temp[tempIndex] = m_Requirements[leftIndex];
				leftIndex++;
				tempIndex++;
			}
			
			while (rightIndex <= rightEnd) // 2) no more items left on the left
			{
				temp[tempIndex] = m_Requirements[rightIndex];
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
		for (int i = 0; i < m_Requirements.size(); i++)
		{
			m_Requirements[i] = temp[i];
		}
		
		length = length*2; // increase the interval length
	}
	
	// clean the temporary array
	for (int i = 0; i < temp.size(); i++)
	{
		temp[i] = nullptr;
	}
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
		if ((*it)->name == name)
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

void SkillRequirementSet::CopyRequirementsArray(const std::vector<SkillRequirement*>& reqSet)
{
	for (int i = 0; i < reqSet.size(); i++)
	{
		AddRequirement({reqSet[i]->name, reqSet[i]->level});
	}
}

Skill::Skill(const std::string& _name, const std::string& _group, uint32_t _level)
	: name(_name), group(_group), level(_level) 
{
};

Skill::Skill(Skill* skill)
	: name(skill->name), group(skill->group), level(skill->level)
{
	for (int i = 0; i < skill->GetRequirementsArray().size(); i++)
	{
		AddRequirement({skill->GetRequirementsArray()[i]->name, skill->GetRequirementsArray()[i]->level});
	}
}

std::string Skill::GetRequirements()
{
	std::string out = "";
	
	int requirementsTotal = GetRequirementsArray().size();
	for (int i = 0; i < requirementsTotal; i++)
	{
		out += GetRequirementsArray()[i]->name;
		if (i < (requirementsTotal-1))
		{
			out += ", ";
		}
	}
	
	return out;
}

SkillGroup::SkillGroup()
{
}

SkillGroup::SkillGroup(SkillGroup* group)
	: name(group->name)
{
	for (int i = 0; i < group->children.size(); i++)
	{
		children.push_back(group->children[i]);
	}
}

SkillGroup::~SkillGroup()
{
	for (auto m : children)
	{
		m = nullptr;
	}
}

void SkillGroup::RemoveSkill(Skill* skill)
{
	for (auto it = children.begin(); it != children.end(); it++)
	{
		if (*it == skill)
		{
			skill->group = "Groupless";
			*it = nullptr;
			children.erase(it);
			break;
		}
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
//		CONSOLE_LOG("Added a skill to the group.");
	} else {
		CONSOLE_LOG("Couldn't add a skill \"", skill->name, "\": already exists in the registry.");
	}
}

void SkillRegistry::AddSkillToGroup(Skill* skill, const std::string& name)
{
	skill->group = name;
	
	bool alreadyExists = false;
	
	if (skill->group.compare("Groupless") != 0)
	{
		if (m_GroupsRegistry.find(skill->group) == m_GroupsRegistry.end())
		{
			CONSOLE_LOG("There is no group called \"\"", skill->group, ",..");
			SkillGroup* group = new SkillGroup();
			group->name = skill->group;
			AddGroup(group);
			CONSOLE_LOG("...so it has been created.");
		} 
		
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
			CONSOLE_LOG("Added a skill to group \"", skill->group, "\".");
		} else {
			CONSOLE_LOG("Couldn't add a skill to group \"", skill->group, "\": already exists there;");
		}
	} else {
		for (auto m : m_SkillsWithoutGroup)
		{
			if (m == skill)
			{
				alreadyExists = true;
				break;
			}
		}
		
		if (!alreadyExists)
		{
			m_SkillsWithoutGroup.push_back(skill);
			CONSOLE_LOG("Added a skill to group \"", skill->group, "\".");
		} else {
			CONSOLE_LOG("Couldn't add a skill to group \"", skill->group, "\": already exists there;");
		}
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
	if (skill->group.compare("Groupless") != 0)
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
	} else {
		for (auto it = GetGrouplessSkills().begin(); it != GetGrouplessSkills().end(); it++)
		{
			if (*it == skill)
			{
				*it = nullptr;
				GetGrouplessSkills().erase(it);
				break;
			}
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
			if ((*it)->group.compare("Groupless") == 0)
			{
				for (auto it = GetGrouplessSkills().begin(); it != GetGrouplessSkills().end(); it++)
				{
					if ((*it)->name.compare(name) == 0)
					{
						*it = nullptr;
						GetGrouplessSkills().erase(it);
						CONSOLE_LOG("Removed a skill from the \"Groupless\" group.");
						break;
					}
				}
			} else {
				group = (*it)->group;
				m_GroupsRegistry[group]->RemoveSkill((*it));
				CONSOLE_LOG("Removed a skill from the group.");
			}
		}
		delete *it;
		*it = nullptr;
		m_SkillsRegistry.erase(it);
		CONSOLE_LOG("Removed a skill from the registry.");
		break;
	}
}

void SkillRegistry::RemoveSkillFromGroup(Skill* skill)
{
	if (skill->group.compare("Groupless") == 0)
	{
		for (auto it = GetGrouplessSkills().begin(); it != GetGrouplessSkills().end(); it++)
		{
			if (*it == skill)
			{
				*it = nullptr;
				GetGrouplessSkills().erase(it);
				CONSOLE_LOG("Removed a skill from the \"Groupless\" group.");
				break;
			}
		}
	} else {
		m_GroupsRegistry[skill->group]->RemoveSkill(skill);
		CONSOLE_LOG("Removed a skill from the group.");
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
		
		// and sort requirements in each skill
		for (int i = 0; i < group->children.size(); i++)
		{
			group->children[i]->SortRequirementsByLevel();
		}
	}
	Sort(m_SkillsWithoutGroup);
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

std::vector<Skill*>& SkillRegistry::GetAllSkills()
{
	return m_SkillsRegistry;
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
