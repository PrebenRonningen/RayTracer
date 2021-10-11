#include "Light.h"
size_t Light::m_AmountOfLights = 0;
Light::Light()
	: m_ID{ m_AmountOfLights++ }
{
}