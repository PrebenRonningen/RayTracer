#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(const Elite::FVector3& direction, const Elite::RGBColor& color, float intensity)
	: m_Direction{direction}
	, m_Color{color}
	, m_Intensity{intensity}
{
}

DirectionalLight::~DirectionalLight()
{
}

Elite::FVector3 DirectionalLight::GetDirection(const HitRecord&) const
{
	return -m_Direction;
}

Elite::RGBColor DirectionalLight::GetBiradiance(const Elite::FPoint3&) const
{
	return m_Color * m_Intensity;
}

Elite::FPoint3 DirectionalLight::GetPosition() const
{
	return Elite::FPoint3(-m_Direction * FLT_MAX);
	//return Elite::FPoint3(0, 0, 0);
}
