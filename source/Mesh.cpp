#include "Mesh.h"
#include <sstream>
#include <fstream>
#include <regex>

Mesh::Mesh(const Elite::FPoint3 position, const char* filePath, Material* pMaterial, const CullMode cullmode)
	: m_Position{position}
	, m_Cullmode{cullmode}
	, m_pMaterial{pMaterial}
{
	ParseFile(filePath);
	for ( size_t i{}; i < m_IndexBuffer.size(); i+=3 )
	{
		std::vector<Elite::FPoint3> vertexList{ m_VertexBuffer[m_IndexBuffer[i] - 1],m_VertexBuffer[m_IndexBuffer[i + 1] - 1], m_VertexBuffer[m_IndexBuffer[i + 2] - 1] };
		m_Triangles.push_back(new Triangle(m_Position, vertexList, m_pMaterial, m_Cullmode));
	}

	Elite::FPoint3 tempCenter;
	for ( Triangle* t : m_Triangles )
	{
		tempCenter = t->GetCenter();
		m_Center.x += tempCenter.x;
		m_Center.y += tempCenter.y;
		m_Center.z += tempCenter.z;
	}
	m_Center.x /= m_Triangles.size();
	m_Center.y /= m_Triangles.size();
	m_Center.z /= m_Triangles.size();

}

Mesh::Mesh(const Elite::FPoint3 position, std::vector<Elite::FPoint3> vertexBuffer, std::vector<unsigned int> idxBuffer, Material* pMaterial, const CullMode cullmode)
	: m_Position{position}
	, m_VertexBuffer{vertexBuffer}
	, m_IndexBuffer{idxBuffer}
	, m_pMaterial{pMaterial}
	, m_Cullmode{cullmode}
	, m_Center{}
{
	for ( size_t i{}; i < m_IndexBuffer.size(); i+=3 )
	{
		std::vector<Elite::FPoint3> vertexList{ m_VertexBuffer[m_IndexBuffer[i]],m_VertexBuffer[m_IndexBuffer[i + 1]], m_VertexBuffer[m_IndexBuffer[i + 2]] };
		m_Triangles.push_back(new Triangle(m_Position, vertexList, m_pMaterial, m_Cullmode));
	}

	Elite::FPoint3 tempCenter;
	for ( Triangle* t : m_Triangles )
	{
		tempCenter = t->GetCenter();
		m_Center.x += tempCenter.x;
		m_Center.y += tempCenter.y;
		m_Center.z += tempCenter.z;
	}
	m_Center.x /= m_Triangles.size();
	m_Center.y /= m_Triangles.size();
	m_Center.z /= m_Triangles.size();
}

bool Mesh::Hit(const Ray& ray, HitRecord& hitRecord) const
{
	for ( Triangle* t : m_Triangles )
	{
		if ( t->Hit(ray, hitRecord) )
		{
			return true;
		}
	}
	return false;
}

void Mesh::Update(const float deltaTime)
{
	for ( Triangle* t : m_Triangles )
	{
		t->Update(deltaTime);
	}
}

void Mesh::CycleCullMode()
{
	for ( Triangle* t : m_Triangles)
	{
		t->CycleCullMode();
	}
}

void Mesh::ParseFile(const char* filePath)
{
	std::ifstream file;
	std::string line;

	file.open(filePath);

	if ( file.is_open() )
	{

		while ( !file.eof() )
		{
			std::stringstream lineLable;
			std::getline(file, line);

			lineLable << line;
			std::string lable;
			lineLable >> lable;
			if ( line.size() < 1 || lable == "#" )
			{
				continue;
			}

			if ( lable == "v" )
			{
				Elite::FPoint3 tempVertex{};
				lineLable >> tempVertex.x;
				lineLable >> tempVertex.y;
				lineLable >> tempVertex.z;

				m_VertexBuffer.push_back(tempVertex);
			} else if ( lable == "f" )
			{
				Elite::IPoint3 tempIdx{};
				lineLable >> tempIdx.x;
				lineLable >> tempIdx.y;
				lineLable >> tempIdx.z;

				m_IndexBuffer.push_back(tempIdx.x);
				m_IndexBuffer.push_back(tempIdx.y);
				m_IndexBuffer.push_back(tempIdx.z);
			}
		}
		file.close();
	}
}