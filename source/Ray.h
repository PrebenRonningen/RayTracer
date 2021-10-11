#pragma once
#include "EMath.h"
#include "EVector.h"
class Material;

struct Ray
{
	Elite::FPoint3* origin = nullptr;
	Elite::FVector3 direction = {0,0,1};

	float tMin = { 0.00001f };
	float tMax = { FLT_MAX };
};

struct HitRecord
{
	float t = {-0.0001f};
	float closest = { FLT_MAX };

	Elite::FPoint3 point = {};
	Elite::FVector3 normal = {};
	Material* material{};
	bool Shadow{};
};