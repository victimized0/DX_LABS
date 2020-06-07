#ifndef _SCENE_H_
#define _SCENE_H_

#include "SceneObjects/GameObject.h"
#include "SceneObjects/PointLight.h"
#include "SceneObjects/Camera.h"
#include "ConstBuffer.h"

class Scene {
private:
	typedef std::vector<std::unique_ptr<CPointLight>> UniquePointLightsArray;

public:
									Scene();
									~Scene();

									Scene(Scene const&)				= delete;
									Scene& operator=(Scene const&)	= delete;
									Scene(Scene&&)					= delete;
									Scene& operator=(Scene&&)		= delete;

public:
	void							AddLight(std::unique_ptr<CPointLight>&& pLight);
	//const std::vector<CPointLight>&	GetAllLights();

	void							SetSunLight(DirLight const* pDirLight);

	void							AddObject(std::unique_ptr<GameObject>&& obj, GameObject* parent = nullptr);
	void							DeleteObject(const std::string& key);
	void							ReparentObject(const std::string& key, GameObject* pNewParent);
	GameObject*						GetSceneObject(const std::string& key)const;

	Camera*							GetMainCamera() { return m_camera.get(); }
	void							SetMainCamera(std::unique_ptr<Camera> camera) { m_camera = std::move(camera); }
	void							RenderScene(IDevCon* context, RenderPass pass);

	void							Initialise(IDevice* device);
	void							Update(float dt);

private:
	ConstBuffer<CBPerFrame>			m_cbPerFrame;

	std::unique_ptr<Camera>			m_camera;
	UniquePointLightsArray			m_vPointLights;
	DirLight const*					m_pDirLight;
	Mesh							m_fullscreenQuad;

public:
	std::unique_ptr<GameObject>		Root;

};

#endif //_SCENE_H_
