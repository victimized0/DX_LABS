#ifndef _SCENE_OBJECT_H_
#define _SCENE_OBJECT_H_

#include "../Interfaces/IGameObject.h"

class SceneObject : public IGameObject {
public:
												SceneObject(const std::string& name, DirectX::XMFLOAT3 position);
												SceneObject(const std::string& name);
	virtual										~SceneObject();

												SceneObject(SceneObject const&)				= default;
												SceneObject& operator=(SceneObject const&)	= default;
												SceneObject(SceneObject&&)					= default;
												SceneObject& operator=(SceneObject&&)		= default;

	void										Initialise()override;
	void										Update(float dt)override;

	void										SetName(const char* newName) { m_name = newName; }
	void										SetPosition(float x, float y, float z);

	const std::string&							GetName()const;
	DirectX::XMFLOAT3							GetPosition()const;
	DirectX::XMVECTOR							GetPositionV()const;

protected:
	std::string									m_name;
	DirectX::XMFLOAT4X4							m_transform;

};

#endif //_SCENE_OBJECT_H_
