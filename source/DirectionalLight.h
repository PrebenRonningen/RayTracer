#pragma once
#include "Light.h"

class DirectionalLight : public Light
{
public:
	DirectionalLight(const Elite::FVector3& direction = Elite::FVector3(0, -1, 0), const Elite::RGBColor& color = Elite::RGBColor(1, 1, 1), float intensity = 1.0f);
	virtual ~DirectionalLight() override;

	virtual Elite::FVector3 GetDirection(const HitRecord& hitRecord = HitRecord{}) const override;
	virtual Elite::RGBColor GetBiradiance(const Elite::FPoint3& position = Elite::FPoint3(0.f, 0.f, 0.f)) const override;
	virtual Elite::FPoint3 GetPosition() const override;

	DirectionalLight(const DirectionalLight& other) = delete;
	DirectionalLight(DirectionalLight&&) = delete;
	DirectionalLight& operator=(const DirectionalLight&) = delete;
	DirectionalLight& operator=(DirectionalLight&&) = delete;

	virtual const char* GetType() const override
	{
		return "Directional Light";
	};

	void SetColor(const Elite::RGBColor& color)
	{
		m_Color = color;
	}
	Elite::RGBColor GetColor() const
	{
		return m_Color;
	};

	void SetDirection(const Elite::FVector3& direction)
	{
		m_Direction = direction;
	}

	Elite::FVector3 GetDirection() const
	{
		return m_Direction;
	};


private:
	Elite::FVector3 m_Direction;
	Elite::RGBColor m_Color;
	float m_Intensity;
};

