#include "Plane.h"

Plane::Plane(const Elite::FPoint3& position, const Elite::FVector3& normal, Material* material)
	: m_pMaterial{ material }
	, m_Position{ position }
	, m_Normal{ normal }
{
	m_Cullmode = CullMode::BackFace;
}

Plane::~Plane()
{
	//delete m_pMaterial;
	//m_pMaterial = nullptr;
}

bool Plane::Hit(const Ray& ray, HitRecord& hitRecord) const
{
	Elite::FVector3 rayPlaneVector = m_Position - *ray.origin;
	
	float rayPlaneDotNormal = Elite::Dot(rayPlaneVector, m_Normal);
	
	if ( !hitRecord.Shadow )
	{
		if ( rayPlaneDotNormal > 0 )
		{
			return false;
		}
	}
	else
	{
		if ( Elite::Dot(rayPlaneVector, m_Normal) < 0 )
		{
			return false;
		}
	}
	
	float tempT = rayPlaneDotNormal / Elite::Dot(ray.direction, m_Normal);
	
	if ( ( tempT < ray.tMin ) || ( tempT > ray.tMax ) )
	{
		return false;
	}
	
	hitRecord.t = tempT;
	
	hitRecord.normal = m_Normal;
	hitRecord.point = *ray.origin + ( tempT * ray.direction ); //+ m_Normal * 0.0001f;
	hitRecord.material = m_pMaterial;
	
	return true;

}

const char* Plane::GetType() const
{
	return "Plane";
}

void Plane::Update(const float)
{
}
