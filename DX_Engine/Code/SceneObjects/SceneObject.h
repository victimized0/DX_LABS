#ifndef _SCENE_OBJECT_H_
#define _SCENE_OBJECT_H_

#include "../Interfaces/IGameObject.h"
#include "../Math/SimpleMath.h"
#include "../Transform.h"

class SceneObject : public IGameObject {
public:
										SceneObject(const std::string& name, const DirectX::SimpleMath::Vector3& m_position);
										SceneObject(const std::string& name);
	virtual								~SceneObject();

										SceneObject(SceneObject const&)				= default;
										SceneObject& operator=(SceneObject const&)	= default;
										SceneObject(SceneObject&&)					= default;
										SceneObject& operator=(SceneObject&&)		= default;

	void								Initialise()override;
	void								Update(float dt)override;

	void								SetName(const char* newName) { m_name = newName; }
	const std::string&					GetName()const;

public:


protected:
	std::string							m_name;

};

#endif //_SCENE_OBJECT_H_
