#include "RenderScene.h"

#include "Plane.h"
#include "Sphere.h"
#include "Camera.h"
#include "Light.h"

RenderScene::RenderScene(const char* sceneName)
	: m_pActiveObject{nullptr}
	, m_pActiveCamera{nullptr}
	, m_pSceneName{ (char*) sceneName }
	, m_RenderMode{RenderMode::AllCombined }
{
}

RenderScene::~RenderScene()
{
	for ( Object* pRenderObject : m_Objects )
	{
		delete pRenderObject;
		pRenderObject = nullptr;
		m_pActiveObject = nullptr;
	}

	for ( Camera* pCamera : m_Cameras )
	{
		delete pCamera;
		pCamera = nullptr;
		m_pActiveCamera = nullptr;
	}

	for ( Light* pLight : m_Lights )
	{
		delete pLight;
		pLight = nullptr;
		m_pActiveLight = nullptr;
	}

	m_Objects.clear();
	m_Cameras.clear();
	m_Lights.clear();
}

void RenderScene::AddObject(Object* pNewObject)
{
	printf("Adding - Type: %s, ID: %i \n",pNewObject->GetType(), int(pNewObject->GetID()));

	auto it = find(m_Objects.begin(), m_Objects.end(), pNewObject);
	if ( it == m_Objects.end() )
	{
		this->m_Objects.push_back(pNewObject);
		m_pActiveObject = pNewObject;
	}
}

void RenderScene::RemoveObject(Object* pRemoveObject)
{
	auto it = find(m_Objects.begin(), m_Objects.end(), pRemoveObject);

	if ( it != m_Objects.end() )
	{
		printf("Removing - Type: %s, ID: %i \n", pRemoveObject->GetType(), int(pRemoveObject->GetID()));
		delete ( *it );
		( *it ) = nullptr;
		m_Objects.erase(it);

		m_pActiveObject = ( !m_Objects.empty() ? m_Objects[0] : nullptr );
	}
}

const std::vector<Object*>& RenderScene::GetObjects() const
{
	return m_Objects;
}

Object* RenderScene::GetActiveObject() const
{
	return m_pActiveObject;
}

const std::vector<Camera*>& RenderScene::GetCameras() const
{
	return m_Cameras;
}

Camera* RenderScene::GetActiveCamera() const
{
	return m_pActiveCamera;
}

const std::vector<Light*>& RenderScene::GetLight() const
{
	return m_Lights;
}

RenderMode RenderScene::GetRenderMode() const
{
	return m_RenderMode;
}

void RenderScene::NextRenderMode()
{
	int mode = int(m_RenderMode) + 1;
	mode %= int(RenderMode::AllCombined) + 1;
	m_RenderMode = RenderMode(mode);
}

char* RenderScene::GetSceneName() const
{
	return m_pSceneName;
}

void RenderScene::NextCamera()
{
	if ( !m_Cameras.empty() )
	{
		auto it = std::find(m_Cameras.begin(), m_Cameras.end(), m_pActiveCamera);

		if ( it != m_Cameras.end() )
		{
			m_pActiveCamera = ( ++it == m_Cameras.end() ) ? m_Cameras[0] : *it;
		}
	}
}

void RenderScene::AddLight(Light* pNewLight)
{
	printf("Adding - Type: %s, ID: %i \n", pNewLight->GetType(), int(pNewLight->GetID()));

	auto it = find(m_Lights.begin(), m_Lights.end(), pNewLight);
	if ( it == m_Lights.end() )
	{
		this->m_Lights.push_back(pNewLight);
		m_pActiveLight = pNewLight;
	}
}

void RenderScene::NextObject()
{
	if ( !m_Objects.empty() )
	{
		auto it = std::find(m_Objects.begin(), m_Objects.end(), m_pActiveObject);

		if ( it != m_Objects.end() )
		{
			m_pActiveObject = ( ++it == m_Objects.end() ) ? m_Objects[0] : *it;
		}
	}
}

void RenderScene::AddCamera(Camera* pNewCamera)
{
	printf("Adding - Type: %s, ID: %i \n", pNewCamera->GetType(), int(pNewCamera->GetID()));

	auto it = find(m_Cameras.begin(), m_Cameras.end(), pNewCamera);
	if ( it == m_Cameras.end() )
	{
		this->m_Cameras.push_back(pNewCamera);
		m_pActiveCamera = pNewCamera;
	}
}

void RenderScene::RemoveCamera(Camera* pRemoveCamera)
{
	auto it = find(m_Cameras.begin(), m_Cameras.end(), pRemoveCamera);

	if ( it != m_Cameras.end() )
	{

		printf("Removing - Type: %s, ID: %i \n", pRemoveCamera->GetType(), int(pRemoveCamera->GetID()));

		delete ( *it );
		( *it ) = nullptr;
		m_Cameras.erase(it);

		m_pActiveCamera = ( !m_Cameras.empty() ? m_Cameras[0] : nullptr );
	}
}
