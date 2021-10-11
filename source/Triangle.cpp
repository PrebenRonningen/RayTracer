#include "Triangle.h"
#include "EMath.h"


Triangle::Triangle(const Elite::FPoint3 position, const std::vector<Elite::FPoint3> vertexList, Material* pMaterial, const CullMode cullmode)
	: m_pMaterial{pMaterial}
	, m_VertexList{vertexList}
	, m_Position{position}
{
	m_TransformedVertex = m_VertexList;
	for ( size_t i{}; i < m_TransformedVertex.size(); i++ )
	{
		m_TransformedVertex[i].x = m_VertexList[i].x + m_Position.x;
		m_TransformedVertex[i].y = m_VertexList[i].y + m_Position.y;
		m_TransformedVertex[i].z = m_VertexList[i].z + m_Position.z;
	}

	m_Center = Elite::FPoint3(( Elite::FVector3(m_TransformedVertex[0]) + Elite::FVector3(m_TransformedVertex[1]) + Elite::FVector3(m_TransformedVertex[2]) ) / 3);
	m_Cullmode = cullmode;
}

bool Triangle::Hit(const Ray& ray, HitRecord& hitRecord) const
{	
	Elite::FVector3 edge = m_TransformedVertex[1] - m_TransformedVertex[0];
	Elite::FVector3 opposingEdge = m_TransformedVertex[2] - m_TransformedVertex[0];
	Elite::FVector3 normal = GetNormalized(Elite::Cross(edge, opposingEdge));

	switch ( m_Cullmode )
	{
		case CullMode::BackFace:
			if( !hitRecord.Shadow )
			{
				if ( Elite::Dot(normal, ray.direction) >= 0 )
				{
					return false;
				}
			} else
			{
				if ( Elite::Dot(normal, ray.direction) <= 0 )
				{
					return false;
				}
			}
			break;
		case CullMode::FrontFace:
			if ( !hitRecord.Shadow )
			{
				if ( Elite::Dot(normal, ray.direction) <= 0 )
				{
					return false;
				}
			} else
			{
				if ( Elite::Dot(normal, ray.direction) >= 0 )
				{
					return false;
				}
			}
			break;
		case CullMode::None:
			break;
	}


	Elite::FVector3 L = m_Center - *ray.origin;
	
	float t = Elite::Dot(L, normal) / Elite::Dot(ray.direction, normal);
	
	if ( t < ray.tMin || t > ray.tMax )
	{
		return false;
	}
	
	Elite::FPoint3 point = ( *ray.origin + t * ray.direction ) + normal * FLT_EPSILON;
	
	
	Elite::FVector3 pointToSide = point - m_TransformedVertex[0];
	if ( Elite::Dot(normal, Elite::Cross(edge, pointToSide)) <= 0)
	{
		return false;
	}
	
	edge = m_TransformedVertex[2] - m_TransformedVertex[1];
	pointToSide = point - m_TransformedVertex[1];
	
	if ( Elite::Dot(normal, Elite::Cross(edge, pointToSide)) <= 0 )
	{
		return false;
	}
	
	edge = m_TransformedVertex[0] - m_TransformedVertex[2];
	pointToSide = point - m_TransformedVertex[2];
	
	if ( Elite::Dot(normal, Elite::Cross(edge, pointToSide)) <= 0 )
	{
		return false;
	}
	
	hitRecord.material = m_pMaterial;
	hitRecord.t = t;

	// wery hacky, but flipping the normals to make sure the triangle get's lit, even when the normal points in the same direction as the lightray
	// not sure what's wrong
	if ( !hitRecord.Shadow )
	{
		if ( Elite::Dot(normal, ray.direction) >= 0 )
		{
			hitRecord.normal = normal * -1.f;
		} else
		{
			hitRecord.normal = normal;
		}
	} else
	{
		if ( Elite::Dot(normal, ray.direction) <= 0 )
		{
			hitRecord.normal = normal * -1.f;
		} else
		{
			hitRecord.normal = normal;
		}
	}

	hitRecord.point = point;
	
	return true;
}

void Triangle::Update(const float deltaTime)
{
	for ( size_t i{}; i < m_VertexList.size(); i++ )
	{
		m_VertexList[i] = MakeRotation( deltaTime, Elite::FVector3{ 0.0f, 1.0f, 0.0f }) * m_VertexList[i];
	}

	for ( size_t i{}; i < m_TransformedVertex.size(); i++ )
	{
		m_TransformedVertex[i].x = m_VertexList[i].x + m_Position.x;
		m_TransformedVertex[i].y = m_VertexList[i].y + m_Position.y;
		m_TransformedVertex[i].z = m_VertexList[i].z + m_Position.z;
	}

	m_Center = Elite::FPoint3(( Elite::FVector3(m_TransformedVertex[0]) + Elite::FVector3(m_TransformedVertex[1]) + Elite::FVector3(m_TransformedVertex[2]) ) / 3);
}
