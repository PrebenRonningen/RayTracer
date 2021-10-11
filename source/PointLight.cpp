#include "PointLight.h"

PointLight::PointLight(const Elite::FPoint3& position, const Elite::RGBColor& color, const float intensity)
	: m_Postion{position}
	, m_Color{color}
	, m_Intensity{intensity}
{
}

PointLight::~PointLight()
{
}

Elite::FVector3 PointLight::GetDirection(const HitRecord& hitRecord) const
{
	Elite::FVector3 direction = Elite::GetNormalized(m_Postion - hitRecord.point);
	return direction;
}

Elite::RGBColor PointLight::GetBiradiance(const Elite::FPoint3& position) const
{
	float biradiance = Elite::Dot(m_Postion - position, m_Postion - position);
	return m_Color * (m_Intensity / biradiance );
}

Elite::FPoint3 PointLight::GetPosition() const
{
	return m_Postion;
}
