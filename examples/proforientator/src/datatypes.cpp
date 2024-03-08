#include "../include/datatypes.h"
#include "../include/logger.h"

SkillRequirementSet::SkillRequirementSet()
{
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
