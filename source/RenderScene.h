#pragma once
#include <vector>

class Object;
class Camera;
class Light;

enum class RenderMode
{
	IrradianceOnly = 0,
	BRDFOnly = 1,
	AllCombined = 2
};

class RenderScene
{
public:
	RenderScene(const char* sceneName);
	~RenderScene();

	void AddObject(Object* pNewObject);
	void RemoveObject(Object* pRemoveObject);

	void NextObject();

	void AddCamera(Camera* pNewCamera);
	void RemoveCamera(Camera* pCamera);

	void NextCamera();

	void AddLight(Light* pNewLight);
	void RemoveLight(Light* pLight);

	void NextLight();

	char* GetSceneName() const;

	const std::vector<Object*>& GetObjects() const;
	Object* GetActiveObject() const;

	const std::vector<Camera*>& GetCameras() const;
	Camera* GetActiveCamera() const;

	const std::vector<Light*>& GetLight() const;
	Light* GetActiveLight() const;

	RenderMode GetRenderMode() const;
	void NextRenderMode();

protected:

	std::vector<Object*> m_Objects;
	std::vector<Camera*> m_Cameras;
	std::vector<Light*> m_Lights;
	Object* m_pActiveObject = nullptr;
	Camera* m_pActiveCamera = nullptr;
	Light* m_pActiveLight = nullptr;
	char* m_pSceneName;
private:
	RenderMode m_RenderMode;
};

