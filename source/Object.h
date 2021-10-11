#pragma once
#include "Ray.h"
#include "EMath.h"

enum class CullMode
{
	None = 0,
	BackFace = 1,
	FrontFace = 2
};

class Object
{
public:
	Object();
	virtual ~Object()
	{
	};

	virtual bool Hit(const Ray& ray, HitRecord& hitRecord) const = 0;
	
	virtual void Update(const float deltaTime) = 0;
	virtual const char* GetType() const
	{
		return "Object";
	};
	virtual const size_t GetID() const
	{
		return m_ID;
	};

	virtual void CycleCullMode()
	{
		int nextCullmode = int(m_Cullmode) + 1;
		nextCullmode %= int(CullMode::FrontFace) + 1;
		m_Cullmode = CullMode(nextCullmode);
	};

protected:
	static size_t m_AmountOfObjects;
	const size_t m_ID;
	CullMode m_Cullmode{ CullMode::None };
	bool m_IsEnabled{true};
};
