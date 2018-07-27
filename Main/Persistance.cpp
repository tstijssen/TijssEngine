#include "Persistance.h"
#include <fstream>

using namespace Tijss::Persistant;

PersistantData *PersistantData::s_instance = 0;

PersistantData::PersistantData(std::string intFileName, std::string floatFileName, std::string stringFileName)
{
	m_IntFileName = intFileName;
	m_FloatFileName = floatFileName;
	m_StringFileName = stringFileName;

	std::string inKey;
	int inInt;
	float inFloat;
	std::string inString;

	std::ifstream intInStream(intFileName);
	while (intInStream >> inKey >> inInt)
	{
		m_StoredInts.insert(std::pair<std::string, int>(inKey, inInt));
	}

	std::ifstream floatInStream(floatFileName);
	while (floatInStream >> inKey >> inFloat)
	{
		m_StoredFloats.insert(std::pair<std::string, float>(inKey, inFloat));
	}

	std::ifstream stringInStream(stringFileName);
	while (stringInStream >> inKey >> inString)
	{
		m_StoredStrings.insert(std::pair<std::string, std::string>(inKey, inString));
	}

}

PersistantData::~PersistantData()
{
	std::ofstream intOutStream(m_IntFileName);

	for (auto it = m_StoredInts.begin(); it != m_StoredInts.end(); ++it)
	{
		intOutStream << (*it).first << ' ' << (*it).second << std::endl;
	}

	std::ofstream floatOutStream(m_FloatFileName);

	for (auto it = m_StoredFloats.begin(); it != m_StoredFloats.end(); ++it)
	{
		floatOutStream << (*it).first << ' ' << (*it).second << std::endl;
	}

	std::ofstream stringOutStream(m_StringFileName);

	for (auto it = m_StoredStrings.begin(); it != m_StoredStrings.end(); ++it)
	{
		stringOutStream << (*it).first << ' ' << (*it).second << std::endl;
	}

}

int PersistantData::GetInt(std::string key)
{
	std::map<std::string, int>::iterator result = m_StoredInts.find(key);
	if (result == m_StoredInts.end())
	{
		return 0;
	}
	
	return (*result).second;
}

float PersistantData::GetFloat(std::string key)
{
	std::map < std::string, float > ::iterator result = m_StoredFloats.find(key);
	if (result == m_StoredFloats.end())
	{
		return 0;
	}

	return (*result).second;
}
std::string PersistantData::GetString(std::string key)
{
	std::map < std::string, std::string > ::iterator result = m_StoredStrings.find(key);
	if (result == m_StoredStrings.end())
	{
		return "";
	}

	return (*result).second;
}

void PersistantData::SetInt(std::string key, int value)
{
	std::map<std::string, int>::iterator result = m_StoredInts.find(key);
	if (result == m_StoredInts.end())
	{
		m_StoredInts.insert(std::pair<std::string, int>(key, value));
		return;
	}

	(*result).second = value;
}
void PersistantData::SetFloat(std::string key, float value)
{
	std::map < std::string, float > ::iterator result = m_StoredFloats.find(key);

	if (result == m_StoredFloats.end())
	{
		m_StoredFloats.insert(std::pair < std::string, float >(key, value));
		return;
	}

	(*result).second = value;
}
void PersistantData::SetString(std::string key, std::string value)
{
	std::map < std::string, std::string > ::iterator result = m_StoredStrings.find(key);

	if (result == m_StoredStrings.end())
	{
		m_StoredStrings.insert(std::pair<std::string, std::string>(key, value));
		return;
	}
	
	(*result).second = value;
}