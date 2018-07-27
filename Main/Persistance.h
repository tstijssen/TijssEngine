#pragma once
#include <map>
#include <string>

namespace Tijss
{
	namespace Persistant
	{
		class PersistantData
		{
		private:
			static PersistantData *s_instance;
			PersistantData(std::string intFileName, std::string floatFileName, std::string stringFileName);
		public:
			// disallow copying, only one file system
			PersistantData(PersistantData const&) = delete;
			void operator=(PersistantData const&) = delete;

			static PersistantData *instance(std::string intFileName = "PersistentData\\IntData.txt",
											std::string floatFileName = "PersistentData\\FloatData.txt",
											std::string stringFileName = "PersistentData\\StringData.txt")
			{
				if (!s_instance)
					s_instance = new PersistantData(intFileName, floatFileName, stringFileName);
				return s_instance;
			}
			
			~PersistantData();

			int GetInt(std::string key);
			float GetFloat(std::string key);
			std::string GetString(std::string key);

			void SetInt(std::string key, int value);
			void SetFloat(std::string key, float value);
			void SetString(std::string key, std::string value);

			// erase all int data
			bool EraseInt();
			// find and erase int
			bool EraseInt(std::string key);

			// erase all string data
			bool EraseString();
			// find and erase string
			bool EraseString(std::string key);

			// erase all float data
			bool EraseFloat();
			// find and erase float
			bool EraseFloat(std::string key);

			// erase all persistant data
			bool EraseAll();
		private:
			std::map<std::string, int> m_StoredInts;
			std::map<std::string, float> m_StoredFloats;
			std::map<std::string, std::string> m_StoredStrings;

			std::string m_IntFileName;
			std::string m_FloatFileName;
			std::string m_StringFileName;
		};
	}
}