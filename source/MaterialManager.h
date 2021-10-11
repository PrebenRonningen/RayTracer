#pragma once
#include "Singleton.h"
#include <string>
#include <vector>
#include <map>

#include "Material.h"

class MaterialManager : public Singleton< MaterialManager>
{
public:
	~MaterialManager()
	{
		Reset();
	}

	void AddMaterial( Material* pNewMaterial, const std::string& materialName);

	Material* GetMaterialByName(const std::string& materialName) const;

private:
	virtual void Reset() override;

	std::map<unsigned long long, Material*> m_MaterialMap; // MaterialNameKey and MaterialID

};

