#pragma once
#include "Material.h"

class Material_Lambert : public Material
{
public:
	Material_Lambert(const Elite::RGBColor& diffuseColor, float diffuseReflectance);

	Material_Lambert(const Material_Lambert& other) = delete;
	Material_Lambert(Material_Lambert&&) = delete;
	Material_Lambert& operator=(const Material_Lambert&) = delete;
	Material_Lambert& operator=(Material_Lambert&&) = delete;

	virtual Elite::RGBColor Shade(const HitRecord& hitRecord, const Elite::FVector3& viewDirection, const Elite::FVector3& lightDirection) const override;

private:
	Elite::RGBColor m_DiffuseColor;
	float m_DiffuseReflectance;
};

