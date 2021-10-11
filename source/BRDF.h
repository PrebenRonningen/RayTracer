#pragma once
#include "EMath.h"
#include "ERGBColor.h"

namespace BRDF
{
	Elite::RGBColor Lambert(float diffuseReflectance, const Elite::RGBColor& diffuseColor);

	Elite::RGBColor Phong(float specularReflectanceFactor, float phongExponent, const Elite::FVector3& cameraViewDirection, const Elite::FVector3& lightInDirection, const Elite::FVector3& surfaceNormal);

	Elite::RGBColor Schlick(const Elite::RGBColor& albidoColor, const float fresnelIntensity, const Elite::FVector3& viewDirection, const Elite::FVector3& halfVector, bool metalness );
	float TrowbridgeRitzGGX(const Elite::FVector3& surfaceNormal, const Elite::FVector3& halfVector, const float roughness);
	float SchlickGGX(const Elite::FVector3& surfaceNormal, const Elite::FVector3& viewDirection, const float roughness);
	float SchlickSmith(const Elite::FVector3& surfaceNormal, const Elite::FVector3& viewDirection, const Elite::FVector3& lightDirection, const float roughness);

	Elite::RGBColor CookTorrance(const Elite::RGBColor& albidoColor, const float roughness, const float fresnelIntensity, const Elite::FVector3& surfaceNormal, const Elite::FVector3& viewDirection, const Elite::FVector3& lightDirection, const Elite::FVector3& halfVector, const bool metalness);
};