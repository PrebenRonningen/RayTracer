//External includes
#pragma warning(push)
#pragma warning (disable:26812)
#include "SDL.h"
#include "SDL_surface.h"
#pragma warning(pop)
//Project includes
#include "ERenderer.h"
#include "ERGBColor.h"

#include "Ray.h"
#include "Material.h"
#include "RenderScene.h"
#include "Camera.h"
#include "Object.h"
#include "Light.h"

Elite::Renderer::Renderer(SDL_Window * pWindow)
{
	//Initialize
	m_pWindow = pWindow;
	m_pFrontBuffer = SDL_GetWindowSurface(pWindow);
	int width, height = 0;
	SDL_GetWindowSize(pWindow, &width, &height);
	m_Width = static_cast<uint32_t>(width);
	m_Height = static_cast<uint32_t>(height);
	m_pBackBuffer = SDL_CreateRGBSurface(0, m_Width, m_Height, 32, 0, 0, 0, 0);
	m_pBackBufferPixels = (uint32_t*)m_pBackBuffer->pixels;
	screenSpaceCoord = Elite::FVector3();
}

// will be used to select initiali start scene later
// but for now, just to get the initial vectors of Objects
void Elite::Renderer::SetScene(RenderScene* pScene)
{
	m_pScene = pScene;
}

void Elite::Renderer::Render()
{
	SDL_LockSurface(m_pBackBuffer);
	Camera* pActiveCamera = m_pScene->GetActiveCamera();
	HitRecord* pHitRecord = pActiveCamera->GetHitRecord();
	Ray* pRay = pActiveCamera->GetRay();
	float FOV = pActiveCamera->GetFOV();
	RGBColor intermediateColor{}; // for the pixel being calculated, get's added to finalColor after each light calculation.
	RGBColor finalColor{};
	float cosLaw{};
	//Loop over all the pixels

	for (uint32_t r = 0; r < m_Height; ++r)
	{
		screenSpaceCoord.y = ScreenSpaceY(r) * FOV;

		for (uint32_t c = 0; c < m_Width; ++c)
		{
			ClearPixel(c, r);

			screenSpaceCoord.x = ScreenSpaceX(c) * FOV;
			
			pActiveCamera->SetScreenSpaceCoordinate(screenSpaceCoord);
				pHitRecord->closest = FLT_MAX;
			for ( Object* pObj : m_pScene->GetObjects())
			{
				finalColor *= 0;
				pHitRecord->Shadow = false;
				if ( pObj->Hit(*pRay, *pHitRecord) )
				{
					if ( pHitRecord->t < pHitRecord->closest )
					{
						pHitRecord->closest = pHitRecord->t;

						for ( Light* pLight : m_pScene->GetLight() )
						{
							if ( !pLight->GetIsEnabled() ) continue;

							intermediateColor *= 0;
						
							if ( pLight->GetIsOn() )
							{
								switch ( m_pScene->GetRenderMode() )
								{
									case RenderMode::IrradianceOnly:
										cosLaw = Dot(pHitRecord->normal, pLight->GetDirection(*pHitRecord));
										if ( cosLaw > 0 )
											intermediateColor += pLight->GetBiradiance(pHitRecord->point) * cosLaw;
										break;
									case RenderMode::BRDFOnly:
										cosLaw = Dot(pHitRecord->normal, pLight->GetDirection(*pHitRecord));
										if ( cosLaw > 0 )
											intermediateColor += pHitRecord->material->Shade(*pHitRecord, pRay->direction, pLight->GetDirection(*pHitRecord)) * cosLaw;
										break;
									case RenderMode::AllCombined:
										cosLaw = Dot(pHitRecord->normal, pLight->GetDirection(*pHitRecord));
										if ( cosLaw > 0 )
											intermediateColor += pLight->GetBiradiance(pHitRecord->point) * pHitRecord->material->Shade(*pHitRecord, pRay->direction, pLight->GetDirection(*pHitRecord)) * cosLaw;
										break;
								}
							}

							if ( pLight->GetIsCastingShadow() )
							{
								HitRecord shadowRecord{};
								shadowRecord.Shadow = true;
								Ray shadowRay{ &pHitRecord->point, pLight->GetDirection(*pHitRecord) };
								for ( Object* object : m_pScene->GetObjects() )
								{
									if ( object->Hit(shadowRay, shadowRecord) )
									{
										FVector3 distanceToLight = pLight->GetPosition() - *shadowRay.origin;
									
										if ( ( shadowRecord.t * shadowRecord.t  ) <= Dot(distanceToLight, distanceToLight) )
										{
											intermediateColor *= 0;
											break;
										}
									}
								}
							}
							
							finalColor += intermediateColor;
						}
						finalColor.MaxToOne();
						m_pBackBufferPixels[c + ( r * m_Width )] = SDL_MapRGB(m_pBackBuffer->format,
						static_cast<uint8_t>( finalColor.r * 255 ),
						static_cast<uint8_t>( finalColor.g * 255 ),
						static_cast<uint8_t>( finalColor.b * 255 ));
					}
				}
			}
		}
	}

	SDL_UnlockSurface(m_pBackBuffer);
	SDL_BlitSurface(m_pBackBuffer, 0, m_pFrontBuffer, 0);
	SDL_UpdateWindowSurface(m_pWindow);
}

void Elite::Renderer::Update(const float deltaTime)
{
	for ( Object* pObj : m_pScene->GetObjects() )
	{
		pObj->Update(deltaTime);
	}
}

void Elite::Renderer::ClearPixel(const uint32_t& c, const uint32_t& r)
{
	m_pBackBufferPixels[c + ( r * m_Width )] = SDL_MapRGB(m_pBackBuffer->format,
		static_cast<uint8_t>( 0 ),
		static_cast<uint8_t>( 0 ),
		static_cast<uint8_t>( 0 ));
}

const float Elite::Renderer::ScreenSpaceX(const uint32_t c)
{
	float Xss = ( 2 *  (float(c + 0.5f) / m_Width ) - 1 ) * (float(m_Width) / float(m_Height));
	return Xss;
}
const float Elite::Renderer::ScreenSpaceY(const uint32_t r)
{
	float Yss = ( 1 - ( 2 * ( float(r + 0.5f) / float(m_Height) ) ) );
	return Yss;
}

bool Elite::Renderer::SaveBackbufferToImage() const
{
	return SDL_SaveBMP(m_pBackBuffer, "BackbufferRender.bmp");
}
