#ifndef _SCENE_H_
#define _SCENE_H_

#include "SceneObjects/GameObject.h"
#include "SceneObjects/Camera.h"

class Scene {
public:
									Scene();
									~Scene();

									Scene(Scene const&)				= delete;
									Scene& operator=(Scene const&)	= delete;
									Scene(Scene&&)					= delete;
									Scene& operator=(Scene&&)		= delete;

public:
	void							AddObject(std::unique_ptr<GameObject>&& obj, GameObject* parent = nullptr);
	void							DeleteObject(const std::string& key);
	void							ReparentObject(const std::string& key, GameObject* pNewParent);
	GameObject*						GetSceneObject(const std::string& key)const;

	Camera*							GetMainCamera() { return m_camera.get(); }
	void							SetMainCamera(std::unique_ptr<Camera> camera) { m_camera = std::move(camera); }
	void							RenderScene(IDevCon* context);

	void							Update(float dt);

private:
	std::unique_ptr<Camera>			m_camera;

public:
	std::unique_ptr<GameObject>		Root;

};

#endif //_SCENE_H_
