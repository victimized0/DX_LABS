#ifndef _SCENE_OBJECT_H_
#define _SCENE_OBJECT_H_

#include "../Interfaces/IGameObject.h"

class SceneObject : public IGameObject {
public:
	SceneObject(const std::string& name, DirectX::XMFLOAT3 position);
	SceneObject(const std::string& name);
	virtual ~SceneObject();

	SceneObject(SceneObject const&)					= default;
	SceneObject& operator=(SceneObject const&)		= default;
	SceneObject(SceneObject&&)						= default;
	SceneObject& operator=(SceneObject&&)			= default;

	void											Initialise()override;
	void											Update(float dt)override;
	virtual void									SetPosition(float x, float y, float z);
	virtual const std::string&						GetName()const;
	const DirectX::XMFLOAT3&						GetPosition()const;

protected:
	std::string										m_name;
	DirectX::XMFLOAT3								m_pos;

};

#endif //_SCENE_OBJECT_H_
