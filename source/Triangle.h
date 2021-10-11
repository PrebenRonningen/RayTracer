#pragma once
#include "Object.h"
#include "MaterialManager.h"
#include <vector>

class Triangle : public Object
{
public:
	Triangle(const Elite::FPoint3 position, const std::vector<Elite::FPoint3> vertexList, Material* pMaterial, const CullMode cullmode = CullMode::BackFace);
	virtual ~Triangle()
	{
	};

	virtual bool Hit(const Ray& ray, HitRecord& hitRecord) const;

	void Update(const float deltaTime);

	virtual const char* GetType() const override
	{
		return "Triangle";
	};

	Elite::FPoint3 GetCenter() const
	{
		return m_Center;
	}
private:
	Material* m_pMaterial;
	std::vector<Elite::FPoint3> m_VertexList;
	std::vector<Elite::FPoint3> m_TransformedVertex;
	Elite::FPoint3 m_Position;
	Elite::FPoint3 m_Center;
	Elite::FVector3 m_Normal;
};