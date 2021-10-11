#include "Sphere.h"



Sphere::Sphere(const Elite::FPoint3& position, Material* material, const float radius)
	: m_pMaterial(material)
	, m_Position(position)
	, m_Radius(radius)
{
}

Sphere::~Sphere()
{

}

bool Sphere::Hit(const Ray& ray, HitRecord& hitRecord) const
{
	Elite::FVector3 DistanceToSphere = m_Position - *ray.origin;

	float squareRadius = m_Radius * m_Radius;
	bool isRayInsideSphere = (Elite::Dot(DistanceToSphere, DistanceToSphere) - squareRadius) < 0;

	if ( isRayInsideSphere ) return false;
	
	float tca = Elite::Dot(DistanceToSphere, ray.direction);

	float squareOD = Elite::Dot(DistanceToSphere, DistanceToSphere) - (tca * tca);

	if ( squareRadius < squareOD ) return false;

	float squareThc = squareRadius - squareOD;

	float thc = sqrtf(squareThc);

	float t0 = tca - thc;

	if ( t0 < 0 ) return false;

	hitRecord.material = m_pMaterial;
	hitRecord.point = *ray.origin + ( t0 * ray.direction );
	hitRecord.normal = ( hitRecord.point - m_Position ) / m_Radius;
	hitRecord.t = t0;

	return true;
}

const char* Sphere::GetType() const
{
	return "Sphere";
}

void Sphere::Update(const float)
{
}
