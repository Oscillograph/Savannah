#ifndef PROFORIENTATOR_DATATYPES_H
#define PROFORIENTATOR_DATATYPES_H

#include "./common.h"
#include <map>
#include <unordered_map>


struct SkillRequirement
{
	std::string name = "";
	uint32_t level = 0;
};

struct SkillRequirementSet
{
	SkillRequirementSet();
	~SkillRequirementSet();
	void AddRequirement(const SkillRequirement& req = {"None", 0});
	void AddRequirement(SkillRequirement* skill);
	void RemoveRequirement(SkillRequirement* skill);
	void RemoveRequirement(const std::string& name);
	
	std::vector<SkillRequirement*>& GetRequirementsArray();
	
protected:
	std::vector<SkillRequirement*> m_Requirements = {};
};

struct Skill : SkillRequirementSet
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
	Skill* FindSkill(const std::string& name); // a skill name in requirement must be equal to a skill name in the registry
	void RemoveSkill(Skill* skill);
	void RemoveSkill(const std::string& name);
	void AddGroup(SkillGroup* group);
	void SortGroups();
	void SortGroup(SkillGroup* group);
	std::string GetRequirements(Skill* skill);
	
	std::map<std::string, SkillGroup*>& GetGroups();
	
private:
	std::map<std::string, SkillGroup*> m_GroupsRegistry = {};
	std::vector<Skill*> m_SkillsRegistry = {};
	
	void Sort(std::vector<Skill*>& source);
};

struct Job : SkillRequirementSet
{
public:
	Job();
	Job(const std::vector<SkillRequirement*>& reqs);
	~Job();
};

#endif
