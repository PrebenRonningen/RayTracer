#pragma once
#include "Material.h"

class Material_LambertPhong : public Material
{
public:

	Material_LambertPhong(const Elite::RGBColor& diffuseColor, float diffuseReflectance,float specularReflectance, float phongExponent);

	Material_LambertPhong(const Material_LambertPhong& other) = delete;
	Material_LambertPhong(Material_LambertPhong&&) = delete;
	Material_LambertPhong& operator=(const Material_LambertPhong&) = delete;
	Material_LambertPhong& operator=(Material_LambertPhong&&) = delete;

	virtual Elite::RGBColor Shade(const HitRecord& hitRecord, const Elite::FVector3& viewDirection, const Elite::FVector3& lightDirection) const override;

private:
	Elite::RGBColor m_DiffuseColor;
	float m_DiffuseReflectance;
	float m_SpeculareReflectance;
	float m_PhongExponent;
};

