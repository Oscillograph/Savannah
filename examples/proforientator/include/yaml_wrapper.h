#include <external/yaml_cpp/include/yaml-cpp/yaml.h>
// #include <common.h>
#include <examples/proforientator/include/datatypes.h>
#include <examples/proforientator/include/fileio.h>

// most of these are yet examples of what should be done

namespace YAML {
	template<>
	struct convert<Proforientator::Skill*> {
		static Node encode(const Proforientator::Skill* rhs) {
			Node node;
			node.push_back(rhs->name.c_str());
			node.push_back(rhs->group.c_str());
			node.push_back(rhs->level);
			return node;
		}
		
		static bool decode(const Node& node, Proforientator::Skill* rhs) {
			if (!node.IsSequence() || (node.size() != 3))
				return false;
			rhs->name = node[0].as<std::string>();
			rhs->group = node[1].as<std::string>();
			rhs->level = node[2].as<int>();
			return true;
		}
	};
}


namespace Proforientator
{
	YAML::Emitter& operator<<(YAML::Emitter& out, Skill* rhs);
	YAML::Emitter& operator<<(YAML::Emitter& out, SkillGroup* rhs);
	
	class YamlWrapper
	{
	public:
		void UseSkillRegistry(SkillRegistry* skillRegistry);
		
		// YAML is just a string
		
		// basic workflow
//		void NewDocument(); // create a new yaml document file
		bool LoadDocument(const std::string& filename); // load an existing yaml document from a file in memory
		
//		void OpenDocument(); // open a yaml document in memory to read or to write
//		void StartDocument(); // start the writing of yaml document
//		void FinishDocument(); // finish the writing of yaml document
//		void CloseDocument(); // close a yaml document in memory
		bool SaveDocument(const std::string& filename); // save a yaml document in a file
		
//		void DisplayDocument(); // display a yaml document as formatted text
		

		
	private:
		std::string m_Contents = "";
		SkillRegistry* m_SkillRegistry = nullptr;
	};
}
