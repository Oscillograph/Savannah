#ifndef PROFORIENTATOR_DATATYPES_H
#define PROFORIENTATOR_DATATYPES_H

#include "./common.h"
#include <unordered_map>

struct Skill
{
	Skill(const std::string& _name = "", const std::string& _group = "", uint32_t _level = 0);
	
	std::string name = "";
	std::string group = "";
	uint32_t level = 0;
};

struct SkillGroup
{
	SkillGroup();
	~SkillGroup();
	
	std::string name = "";
	std::vector<Skill*> children = {};
};

struct SkillRegistry
{
	SkillRegistry();
	~SkillRegistry();
	Skill* NewSkill(const std::string& name = "", const std::string& group = "", uint32_t level = 0);
	void AddSkill(Skill* skill);
	void RemoveSkill(Skill* skill);
	void RemoveSkill(const std::string& name);
	void AddGroup(SkillGroup* group);
	void SortGroups();
	void SortGroup(SkillGroup* group);
	
	std::unordered_map<std::string, SkillGroup*>& GetGroups();
	
private:
	std::unordered_map<std::string, SkillGroup*> m_GroupsRegistry = {};
	std::vector<Skill*> m_SkillsRegistry = {};
	
	void Sort(std::vector<Skill*>& source);
};

struct Job
{
public:
	Job();
	Job(const std::vector<Skill*>& reqs);
	~Job();
	void AddSkill(Skill* skill);
	void RemoveSkill(Skill* skill);
	void RemoveSkill(const std::string& name);
	
private:
	std::vector<Skill*> m_Requirements = {};
};

#endif
