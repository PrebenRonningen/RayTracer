#include "Material_LambertPhong.h"

#include "BRDF.h"

Material_LambertPhong::Material_LambertPhong(const Elite::RGBColor& diffuseColor, float diffuseReflectance, float specularReflectance, float phongExponent)
	: m_DiffuseColor{ diffuseColor }
	, m_DiffuseReflectance{diffuseReflectance}
	, m_SpeculareReflectance{specularReflectance}
	, m_PhongExponent{phongExponent}
{
}

Elite::RGBColor Material_LambertPhong::Shade(const HitRecord& hitRecord, const Elite::FVector3& cameraDirection, const Elite::FVector3& lightDirection) const
{
	return BRDF::Lambert(m_DiffuseReflectance, m_DiffuseColor) + 
		   BRDF::Phong(m_SpeculareReflectance, m_PhongExponent, cameraDirection, lightDirection, hitRecord.normal);
}
