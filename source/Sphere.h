#pragma once
#include "Object.h"
#include "MaterialManager.h"

class Sphere : public Object
{
public:
	Sphere(const Elite::FPoint3& position = Elite::FPoint3(0, 0, -100), Material* material = MaterialManager::GetInstance()->GetMaterialByName("Default_Lambert"), const float radius = 1.f);
	virtual ~Sphere() override;

	virtual bool Hit(const Ray& ray, HitRecord& hitRecord) const override;

	Sphere(const Sphere&) = delete;
	Sphere(Sphere&&) noexcept = delete;
	Sphere& operator=(const Sphere&) = delete;
	Sphere& operator=(Sphere&&) noexcept = delete;

	virtual const char* GetType() const override;
	virtual void Update(const float deltaTime);
private:
	Material* m_pMaterial;
	Elite::FPoint3 m_Position;
	float m_Radius;
};
