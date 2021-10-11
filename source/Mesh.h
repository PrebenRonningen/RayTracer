#pragma once
#include "Object.h"
#include <vector>
#include <string>
#include "Triangle.h"

class Mesh : public Object
{
public:
	Mesh(const Elite::FPoint3 position, const char* filePath, Material* pMaterial, const CullMode cullmode = CullMode::BackFace);
	Mesh(const Elite::FPoint3 position, std::vector<Elite::FPoint3> vertexBuffer, std::vector<unsigned int> idxBuffer, Material* pMaterial, const CullMode cullmode = CullMode::BackFace);

	virtual ~Mesh()
	{
		for ( Triangle* t : m_Triangles )
		{
			delete t;
			t = nullptr;
		}
		m_Triangles.clear();
	};

	virtual bool Hit(const Ray& ray, HitRecord& hitRecord) const;

	virtual void Update(const float deltaTime) override;
	virtual void CycleCullMode() override;
	virtual const char* GetType() const override
	{
		return "Mesh";
	};

private:
	void ParseFile(const char* filePath);

	Material* m_pMaterial;
	std::vector<Elite::FPoint3> m_VertexBuffer;
	std::vector<Triangle*> m_Triangles;
	std::vector<unsigned int> m_IndexBuffer;

	Elite::FPoint3 m_Position;
	Elite::FPoint3 m_Center;
	CullMode m_Cullmode;
};

