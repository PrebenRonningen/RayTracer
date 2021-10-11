#pragma once
#include "Material.h"
class Material_LambertCookTorrance : public Material
{
public:
	Material_LambertCookTorrance(const Elite::RGBColor& diffuseColor, const Elite::RGBColor& albido, float roughness, bool metalness, float fresnelIntensity = 5.f);

	Material_LambertCookTorrance(const Material_LambertCookTorrance& other) = delete;
	Material_LambertCookTorrance(Material_LambertCookTorrance&&) = delete;
	Material_LambertCookTorrance& operator=(const Material_LambertCookTorrance&) = delete;
	Material_LambertCookTorrance& operator=(Material_LambertCookTorrance&&) = delete;

	virtual Elite::RGBColor Shade(const HitRecord& hitRecord, const Elite::FVector3& viewDirection, const Elite::FVector3& lightDirection) const override;

private:
	Elite::RGBColor m_DiffuseColor;
	Elite::RGBColor m_AlbidoColor;
	float m_Roughness;
	float m_FresnelIntensity;
	bool m_IsMetal;
};

