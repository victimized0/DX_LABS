#ifndef _SCENE_H_
#define _SCENE_H_

#include "SceneObjects\SceneObject.h"
#include "SceneObjects\Camera.h"

using SceneObjects = std::vector<std::shared_ptr<SceneObject>>;

class Scene {
public:
									Scene();
									~Scene();

									Scene(Scene const&)				= delete;
									Scene& operator=(Scene const&)	= delete;
									Scene(Scene&&)					= delete;
									Scene& operator=(Scene&&)		= delete;

public:
	void							AddObject(const std::shared_ptr<SceneObject>&);
	SceneObject*					GetSceneObject(const std::string& key)const;
	const SceneObjects&				GetSceneObjects()const { return m_sceneObjects; }
	Camera&							GetMainCamera() { return m_camera; }

	void							Update(float dt);

private:
	Camera							m_camera;
	SceneObjects					m_sceneObjects;

};

#endif //_SCENE_H_
