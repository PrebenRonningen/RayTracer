/*=============================================================================*/
// Copyright 2017-2019 Elite Engine
// Authors: Matthieu Delaere
/*=============================================================================*/
// ERenderer.h: class that holds the surface to render to, does traverse the pixels 
// and traces the rays using a tracer
/*=============================================================================*/
#ifndef ELITE_RAYTRACING_RENDERER
#define	ELITE_RAYTRACING_RENDERER

#include <cstdint>
#include <vector>
#include "EMath.h"

struct SDL_Window;
struct SDL_Surface;
class RenderScene;

#include <iostream>
namespace Elite
{
	class Renderer final
	{
	public:
		Renderer(SDL_Window* pWindow);
		~Renderer() = default;

		Renderer(const Renderer&) = delete;
		Renderer(Renderer&&) noexcept = delete;
		Renderer& operator=(const Renderer&) = delete;
		Renderer& operator=(Renderer&&) noexcept = delete;

		void Render();
		void Update(const float deltaTime);
		void ClearPixel(const uint32_t& c, const uint32_t& r);
		bool SaveBackbufferToImage() const;
		void SetScene(RenderScene* scene);
		
	private:
		const float ScreenSpaceX(const uint32_t c);
		const float ScreenSpaceY(const uint32_t r);

		SDL_Window* m_pWindow = nullptr;
		SDL_Surface* m_pFrontBuffer = nullptr;
		SDL_Surface* m_pBackBuffer = nullptr;
		RenderScene* m_pScene = nullptr;

		Elite::FVector3 screenSpaceCoord;

		uint32_t* m_pBackBufferPixels = nullptr;
		uint32_t m_Width = 0;
		uint32_t m_Height = 0;
	};
}

#endif