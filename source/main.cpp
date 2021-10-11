//External includes
#include "vld.h"

#pragma warning(push)
#pragma warning (disable:26812)
#include "SDL.h"
#include "SDL_surface.h"
#pragma warning(pop)

#undef main

//Standard includes
//#include <string>

//Project includes
#include "ETimer.h"
#include "ERenderer.h"

#pragma region Includeing Scene Items
#include "SceneGraph.h"
#include "MaterialManager.h"
#include "RenderScene.h"
//Objects
#include "Plane.h"
#include "Sphere.h"
#include "Triangle.h"
#include "Mesh.h"
//Materials
#include "Material.h"
#include "Material_Lambert.h"
#include "Material_LambertPhong.h"
#include "Material_LambertCookTorrance.h"
//camera
#include "Camera.h"
//light
#include "PointLight.h"
#include "DirectionalLight.h"

#pragma endregion

void SceneOneInitialize();
void SceneTwoInitialize();
void SceneThreeInitialize();
void SceneFourInitialize();
void SceneFiveInitialize();
void SceneSixInitialize();
void SceneSevenInitialize();
void SceneEightInitialize();

void InitializeMaterials();

void PrintControls();

void Input(float deltaTime);

std::string GetWindowName(std::string windowTitle)
{
	return windowTitle + " - Scene: " + SceneGraph::GetInstance()->GetActiveScene()->GetSceneName() + " - Camera: " + std::to_string(SceneGraph::GetInstance()->GetActiveScene()->GetActiveCamera()->GetID());
}

void ShutDown(SDL_Window* pWindow)
{
	SDL_DestroyWindow(pWindow);
	SDL_Quit();
}

int main(int, char* )
{
	//Create window + surfaces
	SDL_Init(SDL_INIT_VIDEO);
	std::string windowTitle = "RayTracer - **Preben Ronningen**";

	const uint32_t width = 640;
	const uint32_t height = 704;
	SDL_Window* pWindow = SDL_CreateWindow(
		windowTitle.c_str(),
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		width, height, 0);

	if (!pWindow)
		return 1;

	
	//Initialize "framework"
	Elite::Timer* pTimer = new Elite::Timer();
	Elite::Renderer* pRenderer = new Elite::Renderer(pWindow);

	//Initialize "Scenes"
#pragma region Scene Initialization
	InitializeMaterials();
	SceneOneInitialize();
	SceneTwoInitialize();
	SceneThreeInitialize();
	SceneFourInitialize();
	SceneFiveInitialize();
	SceneSixInitialize();
	SceneSevenInitialize();
	SceneEightInitialize();
	SceneGraph::GetInstance()->NextScene();
#pragma endregion

	pRenderer->SetScene(SceneGraph::GetInstance()->GetActiveScene());
	SDL_SetWindowTitle(pWindow, GetWindowName(windowTitle).c_str());

	//Start loop
	pTimer->Start();
	float printTimer = 0.f;
	bool isLooping = true;
	bool takeScreenshot = false;

	PrintControls();

	while (isLooping)
	{
		//--------- Get input events ---------
		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
			switch (e.type)
			{
			case SDL_QUIT:
				isLooping = false;
				break;
			case SDL_KEYUP:
				if(e.key.keysym.scancode == SDL_SCANCODE_X)
					takeScreenshot = true;
				if ( e.key.keysym.scancode == SDL_SCANCODE_I || e.key.keysym.scancode == SDL_SCANCODE_H )
				{
					PrintControls();
				}

				if ( e.key.keysym.scancode == SDL_SCANCODE_C )
				{
					SceneGraph::GetInstance()->GetActiveScene()->NextCamera();
					SDL_SetWindowTitle(pWindow, GetWindowName(windowTitle).c_str());
				}

				if ( e.key.keysym.scancode == SDL_SCANCODE_G )
				{
					for ( Object* pObj : SceneGraph::GetInstance()->GetActiveScene()->GetObjects() )
					{
						pObj->CycleCullMode();
					}

				}

				if ( e.key.keysym.scancode == SDL_SCANCODE_V )
				{
					SceneGraph::GetInstance()->NextScene();
					SDL_SetWindowTitle(pWindow, GetWindowName(windowTitle).c_str());
					pRenderer->SetScene(SceneGraph::GetInstance()->GetActiveScene());
					takeScreenshot = true;
				}

				if ( e.key.keysym.scancode == SDL_SCANCODE_T )
				{
					SceneGraph::GetInstance()->GetActiveScene()->NextRenderMode();
				}

				if ( e.key.keysym.scancode == SDL_SCANCODE_J )
				{
					for ( Light* light : SceneGraph::GetInstance()->GetActiveScene()->GetLight() )
					{
						DirectionalLight* pDirectionalLight = dynamic_cast<DirectionalLight*>( light);
						if ( pDirectionalLight )
						{
							pDirectionalLight->ToggleLight();
							pDirectionalLight->ToggleShadows();
						}
					}
				}

				if ( e.key.keysym.scancode == SDL_SCANCODE_K )
				{
					for ( Light* light : SceneGraph::GetInstance()->GetActiveScene()->GetLight() )
					{
						if ( light->GetPosition().x < 0 )
						{
							PointLight* pPointLight = dynamic_cast<PointLight*>( light );
							if ( pPointLight )
							{
								pPointLight->ToggleLight();
								pPointLight->ToggleShadows();
							}
						}
					}
				}

				if ( e.key.keysym.scancode == SDL_SCANCODE_L )
				{
					for ( Light* light : SceneGraph::GetInstance()->GetActiveScene()->GetLight() )
					{
						if ( light->GetPosition().x > 0 )
						{
							PointLight* pPointLight = dynamic_cast<PointLight*>( light );
							if ( pPointLight )
							{
								pPointLight->ToggleLight();
								pPointLight->ToggleShadows();
							}
						}
					}
				}

				if ( e.key.keysym.scancode == SDL_SCANCODE_Z )
				{
					for ( Light* light : SceneGraph::GetInstance()->GetActiveScene()->GetLight() )
					{
						light->ToggleShadows();
					}
				}

				//TODO: ADD A WAY TO SELECT, DESELECT, SWITCH, ENABLE AND DISABLE OBJECTS
				//TODO: ADD A WAY TO SELECT, DESELECT, SWITCH, ENABLE AND DISABLE LIGHTS
				//TODO: ADD A WAY TO SELECT, DESELECT, SWITCH, ENABLE AND DISABLE SCENES
				//TODO: ADD A WAY TO SELECT, DESELECT, SWITCH, ENABLE AND DISABLE CAMERAS

				break;
			}
		}
		//--------- Input ---------
		Input(pTimer->GetElapsed());

		//--------- Update ---------
		//pRenderer->Update(pTimer->GetElapsed());
		//--------- Render ---------
		pRenderer->Render();
		for (Object* pObj : SceneGraph::GetInstance()->GetActiveScene()->GetObjects())
		{
			pObj->CycleCullMode();
		}

		//--------- Timer ---------
		pTimer->Update();
		printTimer += pTimer->GetElapsed();
		if (printTimer >= 1.f)
		{
			printTimer = 0.f;
			printf("FPS: %i \r", pTimer->GetFPS());
		}

		//Save screenshot after full render
		if (takeScreenshot)
		{
			if ( !pRenderer->SaveBackbufferToImage() )
				printf("Screenshot saved!\n");
			else
				printf("Something went wrong. Screenshot not saved!\n");
			takeScreenshot = false;
		}
	}
	pTimer->Stop();

	//Shutdown "framework"
	delete pRenderer;
	delete pTimer;
	delete SceneGraph::GetInstance();
	delete MaterialManager::GetInstance();

	ShutDown(pWindow);
	return 0;
}

#pragma region Scene Initialization
void SceneOneInitialize()
{
	RenderScene* pScene = new RenderScene("Scene 1 - Lambert Diffuses");

	pScene->AddObject(new Sphere(Elite::FPoint3(-0.75f, 1.0f, 0.0f), MaterialManager::GetInstance()->GetMaterialByName("Neat_Yellow_Lambert")));
	pScene->AddObject(new Sphere(Elite::FPoint3(0.75f, 1.0f, 0.0f), MaterialManager::GetInstance()->GetMaterialByName("Pleasant_Green_Lambert")));
	
	pScene->AddObject(new Plane(Elite::FPoint3(0.0f, 0.0f, 0.0f), Elite::FVector3(0.0f, 1.0f, 0.0f)));
	pScene->AddObject(new Plane(Elite::FPoint3(0.0f, 0.0f, -15.0f), Elite::FVector3(0.0f, 0.0f, 1.0f)));
	pScene->AddCamera(new Camera(Elite::FPoint3(0.0f, 1.0f, 5.0f), Elite::FVector3(0.0f, 0.0f, 1.0f)));
	pScene->AddLight(new PointLight(Elite::FPoint3(0.0f, 5.0f, -5.0f), Elite::RGBColor(1.0f, 1.0f, 1.0f), 25.0f));
	pScene->AddLight(new PointLight(Elite::FPoint3(0.0f, 2.5f, 5.0f), Elite::RGBColor(1.0f, 1.0f, 1.0f), 25.0f));
	pScene->AddLight(new DirectionalLight(Elite::GetNormalized(Elite::FVector3(0.0f, -1.0f, -1.0f)), Elite::RGBColor(1.0f, 1.0f, 1.0f), 0.75f));

	SceneGraph::GetInstance()->AddScene(pScene);
}

void SceneTwoInitialize()
{

	RenderScene* pScene = new RenderScene("Scene 2 - Experementing with stuff");
	
	pScene->AddObject(new Sphere(Elite::FPoint3(-0.75f, 1.0f, 0.0f), MaterialManager::GetInstance()->GetMaterialByName("Red_Lambert")));
	pScene->AddObject(new Sphere(Elite::FPoint3(0.75f, 1.0f, 0.0f), MaterialManager::GetInstance()->GetMaterialByName("Blue_Lambert")));
	pScene->AddObject(new Plane(Elite::FPoint3(0.0f, 0.0f, 0.0f), Elite::FVector3(0.0f, 1.0f, 0.0f)));
	pScene->AddCamera(new Camera(Elite::FPoint3(0.0f, 1.0f, 5.0f), Elite::FVector3(0.0f, 0.0f, 1.0f)));
	pScene->AddCamera(new Camera(Elite::FPoint3(5.0f, 1.0f, 0.0f), Elite::FVector3(1.0f, 0.0f, 0.0f)));
	pScene->AddCamera(new Camera(Elite::FPoint3(0.0f, 1.0f, -5.0f), Elite::FVector3(0.0f, 0.0f, -1.0f)));
	pScene->AddLight(new PointLight(Elite::FPoint3(0.0f, 5.0f, -5.0f), Elite::RGBColor(0.75f, 0.2f, 0.1f), 25.0f));
	pScene->AddLight(new PointLight(Elite::FPoint3(0.0f, 7.5f, 0.0f), Elite::RGBColor(0.1f, 0.75f, 0.2f), 25.0f));
	pScene->AddLight(new PointLight(Elite::FPoint3(0.0f, 5.0f, 5.0f), Elite::RGBColor(0.2f, 0.1f, 0.75f), 25.0f));
	pScene->AddLight(new DirectionalLight(Elite::GetNormalized(Elite::FVector3(0.0f, -1.0f, -1.0f)), Elite::RGBColor(1.0f, 1.0f, 1.0f), 0.75f));
	pScene->NextCamera();

	SceneGraph::GetInstance()->AddScene(pScene);
}

void SceneThreeInitialize()
{
	RenderScene* pScene = new RenderScene("Scene 3 - PBR BRDF");

	pScene->AddObject(new Sphere(Elite::FPoint3(-2.5f, -1.25f, -10.f), MaterialManager::GetInstance()->GetMaterialByName("Silver_Metal_R1")));
	pScene->AddObject(new Sphere(Elite::FPoint3(0.0f, -1.25f, -10.f), MaterialManager::GetInstance()->GetMaterialByName("Zinc_Metal_R06")));
	pScene->AddObject(new Sphere(Elite::FPoint3(2.5f, -1.25f, -10.f), MaterialManager::GetInstance()->GetMaterialByName("Gold_Metal_R01")));

	pScene->AddObject(new Sphere(Elite::FPoint3(-2.5f, 1.25f, -10.f), MaterialManager::GetInstance()->GetMaterialByName("Silver_Dielectric_R1")));
	pScene->AddObject(new Sphere(Elite::FPoint3(0.0f, 1.25f, -10.f), MaterialManager::GetInstance()->GetMaterialByName("Zinc_Dielectric_R06")));
	pScene->AddObject(new Sphere(Elite::FPoint3(2.5f, 1.25f, -10.f), MaterialManager::GetInstance()->GetMaterialByName("Gold_Dielectric_R01")));

	pScene->AddObject(new Plane(Elite::FPoint3(0.0f, 0.0f, -15.0f), Elite::FVector3(0.0f, 0.0f, 1.0f)));
	pScene->AddObject(new Plane(Elite::FPoint3(0.0f, -3.5f, 0.0f), Elite::FVector3(0.0f, 1.0f, 0.0f)));

	pScene->AddLight(new PointLight(Elite::FPoint3(-5.0f, 0.0f,0.0f), Elite::RGBColor(1.0f, 1.0f, 1.0f), 12.5f));
	pScene->AddLight(new PointLight(Elite::FPoint3(5.0f, 0.0f, 0.0f), Elite::RGBColor(1.0f, 1.0f, 1.0f), 12.5f));
	pScene->AddLight(new DirectionalLight(Elite::GetNormalized(Elite::FVector3(0.0f, -1.0f, -1.0f)), Elite::RGBColor(1.0f, 1.0f, 1.0f), 1.0f));
	pScene->AddCamera(new Camera(Elite::FPoint3(0.0f, 0.0f,7.5f), Elite::FVector3(0.0f, 0.0f, 1.0f)));

	SceneGraph::GetInstance()->AddScene(pScene);
}

void SceneFourInitialize()
{
	RenderScene* pScene = new RenderScene("Scene 4 - Triangle");

	std::vector<Elite::FPoint3> vectorList{ Elite::FPoint3{-0.75f, 1.50f, 0.0f},Elite::FPoint3{-0.75f, 0.0f, 0.0f}, Elite::FPoint3{0.75f, 0.0f, 0.0f}};

	pScene->AddObject(new Triangle(Elite::FPoint3(-2.5f, -1.25f, -10.f), vectorList, MaterialManager::GetInstance()->GetMaterialByName("Iron_Dielectric_R1"), CullMode::BackFace));
	pScene->AddObject(new Triangle(Elite::FPoint3(0.0f, -1.25f, -10.f), vectorList, MaterialManager::GetInstance()->GetMaterialByName("Zinc_Dielectric_R01"), CullMode::FrontFace));
	pScene->AddObject(new Triangle(Elite::FPoint3(2.5f, -1.25f, -10.f), vectorList, MaterialManager::GetInstance()->GetMaterialByName("Iron_Dielectric_R01"), CullMode::None));

	pScene->AddLight(new PointLight(Elite::FPoint3(-5.0f, 2.5f, 0.0f), Elite::RGBColor(1.0f, 1.0f, 1.0f), 25.0f));
	pScene->AddLight(new PointLight(Elite::FPoint3(5.0f, 2.5f, 0.0f), Elite::RGBColor(1.0f, 1.0f, 1.0f), 25.0f));	
	pScene->AddLight(new PointLight(Elite::FPoint3(-5.0f, 2.5f, -15.0f), Elite::RGBColor(1.0f, 1.0f, 1.0f), 25.0f));
	pScene->AddLight(new PointLight(Elite::FPoint3(5.0f, 2.5f, -15.0f), Elite::RGBColor(1.0f, 1.0f, 1.0f), 25.0f));

	pScene->AddObject(new Plane(Elite::FPoint3(0.0f, -5.0f, 0.0f), Elite::FVector3(0.0f, 1.0f, 0.0f)));


	pScene->AddLight(new DirectionalLight(Elite::GetNormalized(Elite::FVector3(0.0f, -1.0f, -1.0f)), Elite::RGBColor(1.0f, 1.0f, 1.0f), 1.0f));
	pScene->AddCamera(new Camera(Elite::FPoint3(0.0f, 0.0f, 7.5f), Elite::FVector3(0.0f, 0.0f, 1.0f)));

	SceneGraph::GetInstance()->AddScene(pScene);
}

void SceneFiveInitialize()
{


	RenderScene* pScene = new RenderScene("Scene 5 - Triangle Mesh");

	std::vector<Elite::FPoint3> vertexBuffer{ Elite::FPoint3{-0.75f, 1.50f, 0.0f},Elite::FPoint3{-0.75f, 0.0f, 0.0f}, Elite::FPoint3{0.75f, 0.0f, 0.0f}, Elite::FPoint3{0.75f, 1.5f, 1.0f} };
	std::vector<unsigned int> idxBuffer{ 0,1,2,0,2,3 };

	pScene->AddObject(new Mesh(Elite::FPoint3(0.0f, 2.0f, 0.0f), vertexBuffer, idxBuffer, MaterialManager::GetInstance()->GetMaterialByName("Default_Lambert")));


	pScene->AddLight(new PointLight(Elite::FPoint3(-5.0f, 2.5f, -5.0f), Elite::RGBColor(1.0f, 1.0f, 1.0f), 50.0f));
	pScene->AddLight(new PointLight(Elite::FPoint3(2.5f, 2.5f, -5.0f), Elite::RGBColor(1.0f, 1.0f, 1.0f), 50.0f));
	pScene->AddLight(new PointLight(Elite::FPoint3(-5.0f, 2.5f, -15.0f), Elite::RGBColor(1.0f, 1.0f, 1.0f), 25.0f));
	pScene->AddLight(new PointLight(Elite::FPoint3(5.0f, 2.5f, -15.0f), Elite::RGBColor(1.0f, 1.0f, 1.0f), 25.0f));

	pScene->AddObject(new Plane(Elite::FPoint3(0.0f, 0.0f, -20.0f), Elite::FVector3(0.0f, 0.0f, 1.0f)));
	pScene->AddObject(new Plane(Elite::FPoint3(5.0f, 0.0f, 0.0f), Elite::FVector3(-1.0f, 0.0f, 0.0f)));
	pScene->AddObject(new Plane(Elite::FPoint3(-5.0f, 0.0f, 0.0f), Elite::FVector3(1.0f, 0.0f, 0.0f)));
	pScene->AddObject(new Plane(Elite::FPoint3(0.0f, 0.0f, 0.0f), Elite::FVector3(0.0f, 1.0f, 0.0f)));
	pScene->AddObject(new Plane(Elite::FPoint3(0.0f, 10.0f, 0.0f), Elite::FVector3(0.0f, -1.0f, 0.0f)));

	pScene->AddLight(new DirectionalLight(Elite::GetNormalized(Elite::FVector3(0.0f, -1.0f, -1.0f)), Elite::RGBColor(1.0f, 1.0f, 1.0f), 1.0f));
	pScene->AddCamera(new Camera(Elite::FPoint3(0.0f, 3.0f, 7.5f), Elite::FVector3(0.0f, 0.0f, 1.0f)));

	SceneGraph::GetInstance()->AddScene(pScene);
}

void SceneSixInitialize()
{
	RenderScene* pScene = new RenderScene("Scene 6 - Spheres");

	pScene->AddObject(new Sphere(Elite::FPoint3(-2.5f, 2.0f, 0.0f), MaterialManager::GetInstance()->GetMaterialByName("Green_Dielectric_R01")));
	pScene->AddObject(new Sphere(Elite::FPoint3(0.0f, 2.0f, 0.0f), MaterialManager::GetInstance()->GetMaterialByName("Blue_Dielectric_R06")));
	pScene->AddObject(new Sphere(Elite::FPoint3(2.5f, 2.0f, 0.0f), MaterialManager::GetInstance()->GetMaterialByName("Red_Dielectric_R08")));

	pScene->AddObject(new Sphere(Elite::FPoint3(-2.5f, 4.5f, 0.0f), MaterialManager::GetInstance()->GetMaterialByName("Silver_Metal_R1")));
	pScene->AddObject(new Sphere(Elite::FPoint3(0.0f, 4.5f, 0.0f), MaterialManager::GetInstance()->GetMaterialByName("Gold_Metal_R06")));
	pScene->AddObject(new Sphere(Elite::FPoint3(2.5f, 4.5f, 0.0f), MaterialManager::GetInstance()->GetMaterialByName("Zinc_Metal_R01")));

	pScene->AddObject(new Plane(Elite::FPoint3(0.0f, 0.0f, -15.0f), Elite::FVector3(0.0f, 0.0f, 1.0f), MaterialManager::GetInstance()->GetMaterialByName("Blue_Dielectric_R08")));
	pScene->AddObject(new Plane(Elite::FPoint3(6.0f, 0.0f, 0.0f), Elite::FVector3(-1.0f, 0.0f, 0.0f), MaterialManager::GetInstance()->GetMaterialByName("Blue_Dielectric_R08")));
	pScene->AddObject(new Plane(Elite::FPoint3(-6.0f, 0.0f, 0.0f), Elite::FVector3(1.0f, 0.0f, 0.0f), MaterialManager::GetInstance()->GetMaterialByName("Blue_Dielectric_R08")));
	pScene->AddObject(new Plane(Elite::FPoint3(0.0f, -3.0f, 0.0f), Elite::FVector3(0.0f, 1.0f, 0.0f), MaterialManager::GetInstance()->GetMaterialByName("Blue_Dielectric_R08")));
	pScene->AddObject(new Plane(Elite::FPoint3(0.0f, 10.0f, 0.0f), Elite::FVector3(0.0f, -1.0f, 0.0f), MaterialManager::GetInstance()->GetMaterialByName("Blue_Dielectric_R08")));

	pScene->AddLight(new PointLight(Elite::FPoint3(0.001f, 8.0f, -3.0f), Elite::RGBColor(1.0f, 0.84f, 0.77f), 25.0f));
	pScene->AddLight(new PointLight(Elite::FPoint3(-0.001f, 5.0f, 9.0f), Elite::RGBColor(1.0f, 1.0f, 1.0f), 50.0f));
	
	pScene->AddLight(new DirectionalLight(Elite::GetNormalized(Elite::FVector3(0.0f, -1.0f, 0.0f)), Elite::RGBColor(0.79f, 1.0f, 1.0f), 2.0f));

	pScene->AddCamera(new Camera(Elite::FPoint3(0.0f, 3.0f, 15.0f), Elite::FVector3(0.0f, 0.0f, 1.0f)));

	SceneGraph::GetInstance()->AddScene(pScene);
}

void SceneSevenInitialize()
{
		RenderScene* pScene = new RenderScene("Scene 7 - Triangles and Spheres");

		std::vector<Elite::FPoint3> vectorList{ Elite::FPoint3{-0.75f, 1.50f, 0.0f},Elite::FPoint3{-0.75f, 0.0f, 0.0f}, Elite::FPoint3{0.75f, 0.0f, 0.0f} };

		pScene->AddObject(new Triangle(Elite::FPoint3(-2.5f, 2.5f, -10.0f), vectorList, MaterialManager::GetInstance()->GetMaterialByName("Gray_Dielectric_R1"), CullMode::BackFace));
		pScene->AddObject(new Triangle(Elite::FPoint3(0.0f, 2.5f, -10.0f), vectorList, MaterialManager::GetInstance()->GetMaterialByName("Gray_Dielectric_R1"), CullMode::FrontFace));
		pScene->AddObject(new Triangle(Elite::FPoint3(2.5f, 2.5f, -10.0f), vectorList, MaterialManager::GetInstance()->GetMaterialByName("Gray_Dielectric_R1"), CullMode::None));

		pScene->AddObject(new Sphere(Elite::FPoint3(-2.5f, -1.25f, -10.0f), MaterialManager::GetInstance()->GetMaterialByName("Green_Dielectric_R1")));
		pScene->AddObject(new Sphere(Elite::FPoint3(0.0f, -1.25f, -10.0f), MaterialManager::GetInstance()->GetMaterialByName("Blue_Dielectric_R06")));
		pScene->AddObject(new Sphere(Elite::FPoint3(2.5f, -1.25f, -10.0f), MaterialManager::GetInstance()->GetMaterialByName("Red_Dielectric_R01")));

		pScene->AddObject(new Sphere(Elite::FPoint3(-2.5f, 1.25f, -10.0f), MaterialManager::GetInstance()->GetMaterialByName("Silver_Metal_R1")));
		pScene->AddObject(new Sphere(Elite::FPoint3(0.0f, 1.25f, -10.0f), MaterialManager::GetInstance()->GetMaterialByName("Gold_Metal_R06")));
		pScene->AddObject(new Sphere(Elite::FPoint3(2.5f, 1.25f, -10.0f), MaterialManager::GetInstance()->GetMaterialByName("Zinc_Metal_R01")));

		pScene->AddLight(new PointLight(Elite::FPoint3(-2.5f, 2.75f, 0.0f), Elite::RGBColor(0.8f, 0.8f, 0.75f), 25.0f));
		pScene->AddLight(new PointLight(Elite::FPoint3(2.5f, 1.5f, -5.0f), Elite::RGBColor(0.2f, 0.5f, 0.8f), 20.0f));
		pScene->AddLight(new PointLight(Elite::FPoint3(-0.5f, 3.5f, -12.0f), Elite::RGBColor(0.9f, 0.75f, 0.5f), 12.5f));
		
		pScene->AddObject(new Plane(Elite::FPoint3(0.0f, 0.0f, -15.0f), Elite::FVector3(0.0f, 0.f, 1.0f), MaterialManager::GetInstance()->GetMaterialByName("Blue_Dielectric_R04")));
		pScene->AddObject(new Plane(Elite::FPoint3(6.0f, 0.0f, 0.0f), Elite::FVector3(-1.0f, 0.0f, 0.0f), MaterialManager::GetInstance()->GetMaterialByName("Blue_Dielectric_R04")));
		pScene->AddObject(new Plane(Elite::FPoint3(-6.0f, 0.0f, 0.0f), Elite::FVector3(1.0f, 0.0f, 0.0f), MaterialManager::GetInstance()->GetMaterialByName("Blue_Dielectric_R04")));
		pScene->AddObject(new Plane(Elite::FPoint3(0.0f, -3.5f, 0.0f), Elite::FVector3(0.0f, 1.0f, 0.0f), MaterialManager::GetInstance()->GetMaterialByName("Blue_Dielectric_R04")));
		pScene->AddObject(new Plane(Elite::FPoint3(0.0f, 6.0f, 0.0f), Elite::FVector3(0.0f, -1.0f, 0.0f), MaterialManager::GetInstance()->GetMaterialByName("Blue_Dielectric_R04")));

		pScene->AddLight(new DirectionalLight(Elite::GetNormalized(Elite::FVector3(0.0f, -1.0f, -1.0f)), Elite::RGBColor(0.75f, 0.75f, 0.75f), 1.0f));
		pScene->AddCamera(new Camera(Elite::FPoint3(0.0f, 0.75f, 4.5f), Elite::FVector3(0.0f, 0.0f, 1.0f)));

		SceneGraph::GetInstance()->AddScene(pScene);
}

void SceneEightInitialize()
{
	RenderScene* pScene = new RenderScene("Scene 8 - Bunny");

	pScene->AddObject(new Mesh(Elite::FPoint3(0.0f, -3.5f, -10.0f), "./Resources/lowpoly_bunny.obj", MaterialManager::GetInstance()->GetMaterialByName("White_Lambert"), CullMode::BackFace));
	
	pScene->AddLight(new PointLight(Elite::FPoint3(-0.5f, 3.5f, -12.0f), Elite::RGBColor(0.9f, 0.75f, 0.5f), 12.5f));
	pScene->AddLight(new PointLight(Elite::FPoint3(2.5f, 1.5f, -5.0f), Elite::RGBColor(0.2f, 0.5f, 0.8f), 20.0f));
	pScene->AddLight(new PointLight(Elite::FPoint3(-2.5f, 2.75f, 0.0f), Elite::RGBColor(0.8f, 0.8f, 0.75f), 25.0f));

	pScene->AddObject(new Plane(Elite::FPoint3(0.0f, 0.0f, -15.0f), Elite::FVector3(0.0f, 0.f, 1.0f), MaterialManager::GetInstance()->GetMaterialByName("Blue_Dielectric_R04")));
	pScene->AddObject(new Plane(Elite::FPoint3(6.0f, 0.0f, 0.0f), Elite::FVector3(-1.0f, 0.0f, 0.0f), MaterialManager::GetInstance()->GetMaterialByName("Blue_Dielectric_R04")));
	pScene->AddObject(new Plane(Elite::FPoint3(-6.0f, 0.0f, 0.0f), Elite::FVector3(1.0f, 0.0f, 0.0f), MaterialManager::GetInstance()->GetMaterialByName("Blue_Dielectric_R04")));
	pScene->AddObject(new Plane(Elite::FPoint3(0.0f, -3.5f, 0.0f), Elite::FVector3(0.0f, 1.0f, 0.0f), MaterialManager::GetInstance()->GetMaterialByName("Blue_Dielectric_R04")));
	pScene->AddObject(new Plane(Elite::FPoint3(0.0f, 6.0f, 0.0f), Elite::FVector3(0.0f, -1.0f, 0.0f), MaterialManager::GetInstance()->GetMaterialByName("Blue_Dielectric_R04")));


	pScene->AddLight(new DirectionalLight(Elite::GetNormalized(Elite::FVector3(0.0f, -1.0f, -1.0f)), Elite::RGBColor(0.75f, 0.75f, 0.75f), 1.0f));
	//pScene->AddLight(new DirectionalLight(Elite::GetNormalized(Elite::FVector3(0.0f, -1.0f, 1.0f)), Elite::RGBColor(0.75f, 0.75f, 0.75f), 1.0f));
	//pScene->AddLight(new DirectionalLight(Elite::GetNormalized(Elite::FVector3(-1.0f, -1.0f, 0.0f)), Elite::RGBColor(0.75f, 0.75f, 0.75f), 1.0f));
	//pScene->AddLight(new DirectionalLight(Elite::GetNormalized(Elite::FVector3(1.0f, -1.0f, 0.0f)), Elite::RGBColor(0.75f, 0.75f, 0.75f), 1.0f));
	//
	//pScene->AddCamera(new Camera(Elite::FPoint3(0.0f, 10.f, -10.0f), Elite::FVector3(0.0f, 1.0f, 0.0001f)));
	//pScene->AddCamera(new Camera(Elite::FPoint3(-12.0f, 0.75f, -10.0f), Elite::FVector3(-1.0f, 0.0f, 0.0f)));
	pScene->AddCamera(new Camera(Elite::FPoint3(0.0f, 0.75f, 7.5f), Elite::FVector3(0.0f, 0.0f, 1.0f)));

	SceneGraph::GetInstance()->AddScene(pScene);
}

#pragma endregion

#pragma region Material Initialization
void InitializeMaterials()
{
	MaterialManager::GetInstance()->AddMaterial(new Material_Lambert(Elite::RGBColor(0.5f, 0.5f, 0.5f), 0.5f), "Default_Lambert");

	MaterialManager::GetInstance()->AddMaterial(new Material_Lambert(Elite::RGBColor(1.0f, 1.0f, 1.0f), 1.f), "White_Lambert");
	MaterialManager::GetInstance()->AddMaterial(new Material_Lambert(Elite::RGBColor(1.0f, 0.0f, 0.0f), 1.f), "Red_Lambert");
	MaterialManager::GetInstance()->AddMaterial(new Material_Lambert(Elite::RGBColor(0.0f, 1.0f, 0.0f), 1.f), "Green_Lambert");
	MaterialManager::GetInstance()->AddMaterial(new Material_Lambert(Elite::RGBColor(0.0f, 0.0f, 1.0f), 1.f), "Blue_Lambert");

	MaterialManager::GetInstance()->AddMaterial(new Material_Lambert(Elite::RGBColor(0.15f, 0.5f, 0.35f), .9f), "Pleasant_Green_Lambert");
	MaterialManager::GetInstance()->AddMaterial(new Material_Lambert(Elite::RGBColor(0.6f, 0.6f, 0.2f), .9f), "Neat_Yellow_Lambert");

	MaterialManager::GetInstance()->AddMaterial(new Material_LambertPhong(Elite::RGBColor(0.69f, 0.68f, 0.25f), 1.f, 1.f, 60), "Yellow_LambertPhong");

	MaterialManager::GetInstance()->AddMaterial(new Material_LambertCookTorrance(Elite::RGBColor(0.25f, 0.25f, 0.25f), Elite::RGBColor(0.25f, 0.25f, 0.25f), 1.0f, false), "Gray_Dielectric_R1");
	MaterialManager::GetInstance()->AddMaterial(new Material_LambertCookTorrance(Elite::RGBColor(0.25f, 0.25f, 0.25f), Elite::RGBColor(0.25f, 0.25f, 0.25f), 0.8f, false), "Gray_Dielectric_R08");
	MaterialManager::GetInstance()->AddMaterial(new Material_LambertCookTorrance(Elite::RGBColor(0.25f, 0.25f, 0.25f), Elite::RGBColor(0.25f, 0.25f, 0.25f), 0.6f, false), "Gray_Dielectric_R06");
	MaterialManager::GetInstance()->AddMaterial(new Material_LambertCookTorrance(Elite::RGBColor(0.25f, 0.25f, 0.25f), Elite::RGBColor(0.25f, 0.25f, 0.25f), 0.4f, false), "Gray_Dielectric_R04");
	MaterialManager::GetInstance()->AddMaterial(new Material_LambertCookTorrance(Elite::RGBColor(0.25f, 0.25f, 0.25f), Elite::RGBColor(0.25f, 0.25f, 0.25f), 0.2f, false), "Gray_Dielectric_R02");
	MaterialManager::GetInstance()->AddMaterial(new Material_LambertCookTorrance(Elite::RGBColor(0.25f, 0.25f, 0.25f), Elite::RGBColor(0.25f, 0.25f, 0.25f), 0.1f, false), "Gray_Dielectric_R01");

	MaterialManager::GetInstance()->AddMaterial(new Material_LambertCookTorrance(Elite::RGBColor(0.75f, 0.75f, 0.75f), Elite::RGBColor(0.75f, 0.75f, 0.75f), 1.0f, false), "White_Dielectric_R1");
	MaterialManager::GetInstance()->AddMaterial(new Material_LambertCookTorrance(Elite::RGBColor(0.75f, 0.75f, 0.75f), Elite::RGBColor(0.75f, 0.75f, 0.75f), 0.8f, false), "White_Dielectric_R08");
	MaterialManager::GetInstance()->AddMaterial(new Material_LambertCookTorrance(Elite::RGBColor(0.75f, 0.75f, 0.75f), Elite::RGBColor(0.75f, 0.75f, 0.75f), 0.6f, false), "White_Dielectric_R06");
	MaterialManager::GetInstance()->AddMaterial(new Material_LambertCookTorrance(Elite::RGBColor(0.75f, 0.75f, 0.75f), Elite::RGBColor(0.75f, 0.75f, 0.75f), 0.4f, false), "White_Dielectric_R04");
	MaterialManager::GetInstance()->AddMaterial(new Material_LambertCookTorrance(Elite::RGBColor(0.75f, 0.75f, 0.75f), Elite::RGBColor(0.75f, 0.75f, 0.75f), 0.2f, false), "White_Dielectric_R02");
	MaterialManager::GetInstance()->AddMaterial(new Material_LambertCookTorrance(Elite::RGBColor(0.75f, 0.75f, 0.75f), Elite::RGBColor(0.75f, 0.75f, 0.75f), 0.1f, false), "White_Dielectric_R01");

	MaterialManager::GetInstance()->AddMaterial(new Material_LambertCookTorrance(Elite::RGBColor(0.718f, 0.277f, 0.285f), Elite::RGBColor(0.718f, 0.277f, 0.285f), 1.0f, false), "Red_Dielectric_R1");
	MaterialManager::GetInstance()->AddMaterial(new Material_LambertCookTorrance(Elite::RGBColor(0.718f, 0.277f, 0.285f), Elite::RGBColor(0.718f, 0.277f, 0.285f), 0.8f, false), "Red_Dielectric_R08");
	MaterialManager::GetInstance()->AddMaterial(new Material_LambertCookTorrance(Elite::RGBColor(0.718f, 0.277f, 0.285f), Elite::RGBColor(0.718f, 0.277f, 0.285f), 0.6f, false), "Red_Dielectric_R06");
	MaterialManager::GetInstance()->AddMaterial(new Material_LambertCookTorrance(Elite::RGBColor(0.718f, 0.277f, 0.285f), Elite::RGBColor(0.718f, 0.277f, 0.285f), 0.4f, false), "Red_Dielectric_R04");
	MaterialManager::GetInstance()->AddMaterial(new Material_LambertCookTorrance(Elite::RGBColor(0.718f, 0.277f, 0.285f), Elite::RGBColor(0.718f, 0.277f, 0.285f), 0.2f, false), "Red_Dielectric_R02");
	MaterialManager::GetInstance()->AddMaterial(new Material_LambertCookTorrance(Elite::RGBColor(0.718f, 0.277f, 0.285f), Elite::RGBColor(0.718f, 0.277f, 0.285f), 0.1f, false), "Red_Dielectric_R01");

	MaterialManager::GetInstance()->AddMaterial(new Material_LambertCookTorrance(Elite::RGBColor(0.597f, 0.777f, 0.41f), Elite::RGBColor(0.597f, 0.777f, 0.41f), 1.0f, false), "Green_Dielectric_R1");
	MaterialManager::GetInstance()->AddMaterial(new Material_LambertCookTorrance(Elite::RGBColor(0.597f, 0.777f, 0.41f), Elite::RGBColor(0.597f, 0.777f, 0.41f), 0.8f, false), "Green_Dielectric_R08");
	MaterialManager::GetInstance()->AddMaterial(new Material_LambertCookTorrance(Elite::RGBColor(0.597f, 0.777f, 0.41f), Elite::RGBColor(0.597f, 0.777f, 0.41f), 0.6f, false), "Green_Dielectric_R06");
	MaterialManager::GetInstance()->AddMaterial(new Material_LambertCookTorrance(Elite::RGBColor(0.597f, 0.777f, 0.41f), Elite::RGBColor(0.597f, 0.777f, 0.41f), 0.4f, false), "Green_Dielectric_R04");
	MaterialManager::GetInstance()->AddMaterial(new Material_LambertCookTorrance(Elite::RGBColor(0.597f, 0.777f, 0.41f), Elite::RGBColor(0.597f, 0.777f, 0.41f), 0.2f, false), "Green_Dielectric_R02");
	MaterialManager::GetInstance()->AddMaterial(new Material_LambertCookTorrance(Elite::RGBColor(0.597f, 0.777f, 0.41f), Elite::RGBColor(0.597f, 0.777f, 0.41f), 0.1f, false), "Green_Dielectric_R01");

	MaterialManager::GetInstance()->AddMaterial(new Material_LambertCookTorrance(Elite::RGBColor(0.278f, 0.458f, 0.724f), Elite::RGBColor(0.278f, 0.458f, 0.724f), 1.0f, false), "Blue_Dielectric_R1");
	MaterialManager::GetInstance()->AddMaterial(new Material_LambertCookTorrance(Elite::RGBColor(0.278f, 0.458f, 0.724f), Elite::RGBColor(0.278f, 0.458f, 0.724f), 0.8f, false), "Blue_Dielectric_R08");
	MaterialManager::GetInstance()->AddMaterial(new Material_LambertCookTorrance(Elite::RGBColor(0.278f, 0.458f, 0.724f), Elite::RGBColor(0.278f, 0.458f, 0.724f), 0.6f, false), "Blue_Dielectric_R06");
	MaterialManager::GetInstance()->AddMaterial(new Material_LambertCookTorrance(Elite::RGBColor(0.278f, 0.458f, 0.724f), Elite::RGBColor(0.278f, 0.458f, 0.724f), 0.4f, false), "Blue_Dielectric_R04");
	MaterialManager::GetInstance()->AddMaterial(new Material_LambertCookTorrance(Elite::RGBColor(0.278f, 0.458f, 0.724f), Elite::RGBColor(0.278f, 0.458f, 0.724f), 0.2f, false), "Blue_Dielectric_R02");
	MaterialManager::GetInstance()->AddMaterial(new Material_LambertCookTorrance(Elite::RGBColor(0.278f, 0.458f, 0.724f), Elite::RGBColor(0.278f, 0.458f, 0.724f), 0.1f, false), "Blue_Dielectric_R01");


	MaterialManager::GetInstance()->AddMaterial(new Material_LambertCookTorrance(Elite::RGBColor(0.562f, 0.565f, 0.578f), Elite::RGBColor(0.562f, 0.565f, 0.578f), 1.0f, true), "Iron_Metal_R1");
	MaterialManager::GetInstance()->AddMaterial(new Material_LambertCookTorrance(Elite::RGBColor(0.562f, 0.565f, 0.578f), Elite::RGBColor(0.562f, 0.565f, 0.578f), 0.6f, true), "Iron_Metal_R06");
	MaterialManager::GetInstance()->AddMaterial(new Material_LambertCookTorrance(Elite::RGBColor(0.562f, 0.565f, 0.578f), Elite::RGBColor(0.562f, 0.565f, 0.578f), 0.1f, true), "Iron_Metal_R01");

	MaterialManager::GetInstance()->AddMaterial(new Material_LambertCookTorrance(Elite::RGBColor(0.562f, 0.565f, 0.578f), Elite::RGBColor(0.562f, 0.565f, 0.578f), 1.0f, false), "Iron_Dielectric_R1");
	MaterialManager::GetInstance()->AddMaterial(new Material_LambertCookTorrance(Elite::RGBColor(0.562f, 0.565f, 0.578f), Elite::RGBColor(0.562f, 0.565f, 0.578f), 0.6f, false), "Iron_Dielectric_R06");
	MaterialManager::GetInstance()->AddMaterial(new Material_LambertCookTorrance(Elite::RGBColor(0.562f, 0.565f, 0.578f), Elite::RGBColor(0.562f, 0.565f, 0.578f), 0.1f, false), "Iron_Dielectric_R01");

	MaterialManager::GetInstance()->AddMaterial(new Material_LambertCookTorrance(Elite::RGBColor(0.664f, 0.824f, 0.850f), Elite::RGBColor(0.664f, 0.824f, 0.850f), 1.0f, true), "Zinc_Metal_R1");
	MaterialManager::GetInstance()->AddMaterial(new Material_LambertCookTorrance(Elite::RGBColor(0.664f, 0.824f, 0.850f), Elite::RGBColor(0.664f, 0.824f, 0.850f), 0.6f, true), "Zinc_Metal_R06");
	MaterialManager::GetInstance()->AddMaterial(new Material_LambertCookTorrance(Elite::RGBColor(0.664f, 0.824f, 0.850f), Elite::RGBColor(0.664f, 0.824f, 0.850f), 0.1f, true), "Zinc_Metal_R01");

	MaterialManager::GetInstance()->AddMaterial(new Material_LambertCookTorrance(Elite::RGBColor(0.664f, 0.824f, 0.850f), Elite::RGBColor(0.664f, 0.824f, 0.850f), 1.0f, false), "Zinc_Dielectric_R1");
	MaterialManager::GetInstance()->AddMaterial(new Material_LambertCookTorrance(Elite::RGBColor(0.664f, 0.824f, 0.850f), Elite::RGBColor(0.664f, 0.824f, 0.850f), 0.6f, false), "Zinc_Dielectric_R06");
	MaterialManager::GetInstance()->AddMaterial(new Material_LambertCookTorrance(Elite::RGBColor(0.664f, 0.824f, 0.850f), Elite::RGBColor(0.664f, 0.824f, 0.850f), 0.1f, false), "Zinc_Dielectric_R01");


	MaterialManager::GetInstance()->AddMaterial(new Material_LambertCookTorrance(Elite::RGBColor(0.972f, 0.96f, 0.915f), Elite::RGBColor(0.972f, 0.96f, 0.915f), 1.0f, true), "Silver_Metal_R1");
	MaterialManager::GetInstance()->AddMaterial(new Material_LambertCookTorrance(Elite::RGBColor(0.972f, 0.96f, 0.915f), Elite::RGBColor(0.972f, 0.96f, 0.915f), 0.6f, true), "Silver_Metal_R06");
	MaterialManager::GetInstance()->AddMaterial(new Material_LambertCookTorrance(Elite::RGBColor(0.972f, 0.96f, 0.915f), Elite::RGBColor(0.972f, 0.96f, 0.915f), 0.1f, true), "Silver_Metal_R01");

	MaterialManager::GetInstance()->AddMaterial(new Material_LambertCookTorrance(Elite::RGBColor(0.972f, 0.96f, 0.915f), Elite::RGBColor(0.972f, 0.96f, 0.915f), 1.0f, false), "Silver_Dielectric_R1");
	MaterialManager::GetInstance()->AddMaterial(new Material_LambertCookTorrance(Elite::RGBColor(0.972f, 0.96f, 0.915f), Elite::RGBColor(0.972f, 0.96f, 0.915f), 0.4f, false), "Silver_Dielectric_R06");
	MaterialManager::GetInstance()->AddMaterial(new Material_LambertCookTorrance(Elite::RGBColor(0.972f, 0.96f, 0.915f), Elite::RGBColor(0.972f, 0.96f, 0.915f), 0.1f, false), "Silver_Dielectric_R01");

	MaterialManager::GetInstance()->AddMaterial(new Material_LambertCookTorrance(Elite::RGBColor(1.0f, 0.782f, 0.344f), Elite::RGBColor(1.0f, 0.782f, 0.344f), 1.0f, true), "Gold_Metal_R1");
	MaterialManager::GetInstance()->AddMaterial(new Material_LambertCookTorrance(Elite::RGBColor(1.0f, 0.782f, 0.344f), Elite::RGBColor(1.0f, 0.782f, 0.344f), 0.6f, true), "Gold_Metal_R06");
	MaterialManager::GetInstance()->AddMaterial(new Material_LambertCookTorrance(Elite::RGBColor(1.0f, 0.782f, 0.344f), Elite::RGBColor(1.0f, 0.782f, 0.344f), 0.1f, true), "Gold_Metal_R01");
																								 									   
	MaterialManager::GetInstance()->AddMaterial(new Material_LambertCookTorrance(Elite::RGBColor(1.0f, 0.782f, 0.344f), Elite::RGBColor(1.0f, 0.782f, 0.344f), 1.0f, false), "Gold_Dielectric_R1");
	MaterialManager::GetInstance()->AddMaterial(new Material_LambertCookTorrance(Elite::RGBColor(1.0f, 0.782f, 0.344f), Elite::RGBColor(1.0f, 0.782f, 0.344f), 0.6f, false), "Gold_Dielectric_R06");
	MaterialManager::GetInstance()->AddMaterial(new Material_LambertCookTorrance(Elite::RGBColor(1.0f, 0.782f, 0.344f), Elite::RGBColor(1.0f, 0.782f, 0.344f), 0.1f, false), "Gold_Dielectric_R01");
}

#pragma endregion

void Input(float deltaTime)
{
	int x{}, y{};
	const Uint8* keyState = SDL_GetKeyboardState(NULL);

	Camera* pActiveCamera = SceneGraph::GetInstance()->GetActiveScene()->GetActiveCamera();
	
	( keyState[SDL_SCANCODE_LSHIFT] ) ? pActiveCamera->Sprint(true): pActiveCamera->Sprint(false);


	if ( keyState[SDL_SCANCODE_A] )
	{
		pActiveCamera->MoveLocalX(deltaTime, false);
	}
	if ( keyState[SDL_SCANCODE_Q] )
	{
		pActiveCamera->MoveLocalY(deltaTime, false);
	}
	if ( keyState[SDL_SCANCODE_W] )
	{
		pActiveCamera->MoveLocalZ(deltaTime, false);
	}
	if ( keyState[SDL_SCANCODE_D] )
	{
		pActiveCamera->MoveLocalX(deltaTime, true);
	}
	if ( keyState[SDL_SCANCODE_E] )
	{
		pActiveCamera->MoveLocalY(deltaTime, true);
	}
	if ( keyState[SDL_SCANCODE_S] )
	{
		pActiveCamera->MoveLocalZ(deltaTime, true);
	}

	if ( keyState[SDL_SCANCODE_LEFT] )
	{
		pActiveCamera->Pan(deltaTime, true);
	}
	if ( keyState[SDL_SCANCODE_RIGHT] )
	{
		pActiveCamera->Pan(deltaTime, false);
	}
	if ( keyState[SDL_SCANCODE_DOWN] )
	{
		pActiveCamera->Tilt(deltaTime, false);
	}
	if ( keyState[SDL_SCANCODE_UP] )
	{
		pActiveCamera->Tilt(deltaTime, true);
	}

	if ( SDL_GetRelativeMouseState(&x, &y) == NULL ) return;

	if ( SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(1) && SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(3) )
	{
		pActiveCamera->MoveUpDownMouse(deltaTime, y);
	} else
	{
		if ( SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(1) )
		{
			pActiveCamera->RotateAndMoveLRMouse(deltaTime, x, y);

		}
		if ( SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(3) )
		{
			pActiveCamera->RotateMouse(deltaTime, x, y);
		}
	}
}

void PrintControls()
{
	//---------- CONTROLS ----------
	printf("||--------------- CONTROLS --------------\n");
	printf("|| Move --------------------------------- WASDQE\n");
	printf("|| Pan/Tilt ----------------------------- Arrows\n||\n");
		
	printf("|| Move --------------------------------- LMB+Drag\n");
	printf("|| Move UP/DOWN-------------------------- LMB+RMB+Drag\n");
	printf("|| Pan/Tilt ----------------------------- RMB+Drag\n||\n");
			
	printf("|| Toggle directional light ------------- J\n");
	printf("|| Toggle left/right point light -------- K-L\n");
	printf("|| Toggle shadows ----------------------- Z\n");
	printf("|| Switch Render Mode ------------------- T\n");
	printf("|| Cycle CullMode ----------------------- G\n||\n");

	printf("|| Switch Scene ------------------------- V\n");
	printf("|| Switch Camera ------------------------ C\n||\n");
			
	printf("|| Show Controls ------------------------ H & I\n");
	printf("||--------------- CONTROLS --------------\n");
}