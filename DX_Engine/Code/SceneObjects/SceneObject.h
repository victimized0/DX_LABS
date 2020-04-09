#ifndef _SCENE_OBJECT_H_
#define _SCENE_OBJECT_H_

#include "../Interfaces/IGameObject.h"
#include "../Math/SimpleMath.h"
#include "../Transform.h"

class SceneObject : public IGameObject {
public:
										SceneObject(const std::string& name, const DirectX::SimpleMath::Vector3& position);
										SceneObject(const std::string& name);
	virtual								~SceneObject();

										SceneObject(SceneObject const&)				= default;
										SceneObject& operator=(SceneObject const&)	= default;
										SceneObject(SceneObject&&)					= default;
										SceneObject& operator=(SceneObject&&)		= default;

	void								Initialise()override;
	void								Update(float dt)override;

	void								SetName(const char* newName) { m_name = newName; }
	void								SetPosition(float x, float y, float z);
	void								SetPosition(const DirectX::SimpleMath::Vector3& position);

	const std::string&					GetName()const;
	DirectX::SimpleMath::Vector3		GetPosition()const;

public:
	Transform							Transform;

protected:
	std::string							m_name;

};

#endif //_SCENE_OBJECT_H_
