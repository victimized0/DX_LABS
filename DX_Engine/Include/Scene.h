#ifndef _SCENE_H_
#define _SCENE_H_

#include "SceneObjects\SceneObject.h"

using pVecSceneObj = std::vector<std::shared_ptr<SceneObject>>;

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
	std::shared_ptr<SceneObject>	GetSceneObject(const std::string& key)const;
	pVecSceneObj					GetSceneObjects()const { return m_sceneObjects; }

	void							Initialise();
	void							Update(float dt);

private:
	pVecSceneObj					m_sceneObjects;

};

#endif //_SCENE_H_
