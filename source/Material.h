#pragma once
#include "Ray.h"
#include "EMath.h"
#include "ERGBColor.h"


class Material
{
public:
	virtual Elite::RGBColor Shade(const HitRecord& hitRecord, const Elite::FVector3& viewDirection, const Elite::FVector3& lightDirection) const = 0;
	virtual ~Material() = default;
};

