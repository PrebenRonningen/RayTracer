#include "BRDF.h"

Elite::RGBColor BRDF::Lambert(float diffuseReflectance, const Elite::RGBColor& diffuseColor)
{
	return diffuseColor * diffuseReflectance;
}

Elite::RGBColor BRDF::Phong(float specularReflectanceFactor, float phongExponent, const Elite::FVector3& cameraViewDirection, const Elite::FVector3& lightInDirection, const Elite::FVector3& surfaceNormal)
{
	Elite::FVector3 reflect = lightInDirection - ( 2 * Elite::Dot(surfaceNormal, lightInDirection) * surfaceNormal );

	float cosA = Elite::Dot(reflect, cameraViewDirection);
	float phongSpecularReflection = specularReflectanceFactor * powf(cosA, phongExponent);

	return Elite::RGBColor(phongSpecularReflection, phongSpecularReflection, phongSpecularReflection);
}

Elite::RGBColor BRDF::Schlick(const Elite::RGBColor& albidoColor, const float fresnelIntensity, const Elite::FVector3& viewDirection, const Elite::FVector3& halfVector, bool metalness)
{

	Elite::RGBColor f0 = ( metalness == false) ? Elite::RGBColor(0.04f, 0.04f, 0.04f) : albidoColor;

	Elite::RGBColor finalSlick = f0 + ( Elite::RGBColor(1, 1, 1) - f0 ) * powf(1 - Elite::Dot(halfVector, viewDirection), fresnelIntensity);

	return finalSlick;
}

float BRDF::TrowbridgeRitzGGX(const Elite::FVector3& surfaceNormal, const Elite::FVector3& halfVector, const float roughness)
{
	float squareA = powf(powf(roughness, 2),2);
	float trowbridge = squareA;
	
	trowbridge /= float(E_PI) * powf(( powf(Elite::Dot(surfaceNormal, halfVector), 2) * ( squareA - 1 ) + 1 ), 2);

	return trowbridge;
}

float BRDF::SchlickGGX(const Elite::FVector3& surfaceNormal, const Elite::FVector3& viewDirection, const float remappedRoughness)
{
	float normalDotView = Elite::Dot(surfaceNormal, viewDirection);

	float schlickGGX = normalDotView/((normalDotView * ( 1 - remappedRoughness )) + remappedRoughness);
	return schlickGGX;
}

float BRDF::SchlickSmith(const Elite::FVector3& surfaceNormal, const Elite::FVector3& viewDirection, const Elite::FVector3& lightDirection, const float roughness)
{
	float remappedRoughness = powf(powf(roughness, 2) + 1, 2) / 8.f;
	return SchlickGGX(surfaceNormal, -viewDirection, remappedRoughness) * SchlickGGX(surfaceNormal, lightDirection, remappedRoughness);
}

Elite::RGBColor BRDF::CookTorrance(const Elite::RGBColor& albidoColor, const float roughness, const float fresnelIntensity, const Elite::FVector3& surfaceNormal, const Elite::FVector3& viewDirection, const Elite::FVector3& lightDirection, const Elite::FVector3& halfVector, const bool metalness)
{

	Elite::RGBColor F = Schlick(albidoColor, fresnelIntensity, viewDirection, halfVector, metalness);
	float D = TrowbridgeRitzGGX(surfaceNormal, halfVector, roughness);
	float G = SchlickSmith(surfaceNormal, viewDirection, lightDirection, roughness);

	Elite::RGBColor specular = ( F * D * G ) / ( 4.f * ( Elite::Dot(-viewDirection, surfaceNormal) * Elite::Dot(lightDirection, surfaceNormal) ) );
	return specular;
}