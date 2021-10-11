#pragma once
#include "Object.h"
#include "MaterialManager.h"

class Plane :  public Object
{
public:
	Plane(const Elite::FPoint3& position = Elite::FPoint3(0, 0, 0), const Elite::FVector3& normal = Elite::FVector3(0, 0, 0), Material* material = MaterialManager::GetInstance()->GetMaterialByName("Default_Lambert"));
	virtual ~Plane() override;

	virtual bool Hit(const Ray& ray, HitRecord& hitRecord) const override;

	Plane(const Plane&) = delete;
	Plane(Plane&&) noexcept = delete;
	Plane& operator=(const Plane&) = delete;
	Plane& operator=(Plane&&) noexcept = delete;

	virtual const char* GetType() const override;
	virtual void Update(const float deltaTime);

private:
	Material* m_pMaterial;
	Elite::FPoint3 m_Position;
	Elite::FVector3 m_Normal;
};

