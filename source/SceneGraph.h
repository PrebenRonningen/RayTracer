#pragma once
#include "Singleton.h"
#include <vector>

class RenderScene;

class SceneGraph : public Singleton<SceneGraph>
{
public:
	~SceneGraph()
	{
		Reset();
	}

	void AddScene(RenderScene* pNewObject);
	void RemoveScene(RenderScene* pRenderScene);

	void NextScene();

	RenderScene* GetActiveScene() const;

private:
	void Reset() override;

	std::vector<RenderScene*> m_Scenes;
	RenderScene* m_pActiveScene = nullptr;
};
