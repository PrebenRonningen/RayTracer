#pragma once
#include "Ray.h"
#include "EMath.h"
#include "ERGBColor.h"

class Light
{
public:	
	Light();
	virtual ~Light(){};

	Light(const Light& other) = delete;
	Light(Light&&) = delete;
	Light& operator=(const Light&) = delete;
	Light& operator=(Light&&) = delete;

	virtual Elite::FVector3 GetDirection(const HitRecord& hitRecord = HitRecord{}) const = 0;
	virtual Elite::RGBColor GetBiradiance(const Elite::FPoint3& position = Elite::FPoint3(0.f, 0.f, 0.f)) const = 0;
	virtual Elite::FPoint3 GetPosition() const = 0;

	virtual const char* GetType() const
	{
		return "Light";
	};
	inline const size_t GetID() const
	{
		return m_ID;
	};
	inline void SetIsEnabled(const bool state)
	{
		m_IsEnabled = state;
	}
	inline bool GetIsEnabled() const
	{
		return m_IsEnabled;
	};
	inline bool GetIsOn() const
	{
		return m_IsOn;
	};
	inline bool GetIsCastingShadow() const
	{
		return m_IsCastingShadow;
	};
	inline void ToggleLight()
	{
		m_IsOn = !m_IsOn;
	};
	inline void ToggleShadows()
	{
		m_IsCastingShadow = !m_IsCastingShadow;
	};
	inline void ToggleIsEnabled()
	{
		m_IsEnabled = !m_IsEnabled;
	}

protected:
	static size_t m_AmountOfLights;
	size_t m_ID{};
	bool m_IsEnabled{true};
	bool m_IsOn{true};
	bool m_IsCastingShadow{true};
};