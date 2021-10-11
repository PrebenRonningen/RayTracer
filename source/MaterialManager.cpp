#include "MaterialManager.h"
#include <regex>

void MaterialManager::AddMaterial( Material* pNewMaterial, const std::string& materialName)
{
	unsigned long long key = 0;
	if ( pNewMaterial != nullptr )
	{
		key = std::hash<std::string>()( materialName.c_str() );
	} else
	{
		printf("[INFO] Material Name: %s\n", materialName.c_str());

		std::cmatch matchName;
		std::regex r("([\\_a-zA-Z0-9]+)");

		std::regex_search(materialName.c_str(), matchName, r);

		key = std::hash<std::string>()( materialName.c_str() );
	}

	m_MaterialMap.emplace(std::pair<unsigned long long, Material*>(key, pNewMaterial));
}


Material* MaterialManager::GetMaterialByName(const std::string& materialName) const
{
	printf("[INFO] Material Name: %s\n", materialName.c_str());

	std::cmatch nameMatch;
	std::regex r("([\\_a-zA-Z0-9]+)");

	std::regex_search(materialName.c_str(), nameMatch, r);

	if ( nameMatch.size() < 2 )
	{
		printf("[ERROR] Invalide Name\n");
		return nullptr;
	}

	unsigned long long nameKey = std::hash<std::string>()( nameMatch[1].str().c_str() );

	std::map<unsigned long long, Material*>::const_iterator pos = m_MaterialMap.find(nameKey);

	if ( pos != m_MaterialMap.end() )
	{
		Material* pMaterial = pos->second;
		return pMaterial;
	} else
	{
		printf("[ERROR] No Material named \"%s\" found\n\n", materialName.c_str());
		return nullptr;
	}
}

void MaterialManager::Reset()
{
	for ( auto& t : m_MaterialMap )
	{
		delete t.second;
		t.second = nullptr;
	}
}
