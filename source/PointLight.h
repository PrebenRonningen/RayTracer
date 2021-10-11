#pragma once
#include "Light.h"


class PointLight : public Light
{
public:

	PointLight(const Elite::FPoint3& position = Elite::FPoint3(0, 0, 0), const Elite::RGBColor& color = Elite::RGBColor(1, 1, 1), float intensity = 10);
	virtual ~PointLight() override;

	virtual Elite::FVector3 GetDirection(const HitRecord& hitRecord) const override;
	virtual Elite::RGBColor GetBiradiance(const Elite::FPoint3& position = Elite::FPoint3(0.f, 0.f, 0.f)) const override;
	virtual Elite::FPoint3 GetPosition() const override;

	PointLight(const PointLight& other) = delete;
	PointLight(PointLight&&) = delete;
	PointLight& operator=(const PointLight&) = delete;
	PointLight& operator=(PointLight&&) = delete;

	virtual const char* GetType() const override
	{
		return "Point Light";
	};

	void SetColor(const Elite::RGBColor& color)
	{
		m_Color = color;
	}
	Elite::RGBColor GetColor() const
	{
		return m_Color;
	};
private:
	Elite::RGBColor m_Color;
	Elite::FPoint3 m_Postion;
	float m_Intensity;
};

