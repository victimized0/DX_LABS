#ifndef _GEOMETY_OBJECT_H_
#define _GEOMETY_OBJECT_H_

#include "Model.h"

#include "../Math/SimpleMath.h"

using Microsoft::WRL::ComPtr;

class GameObject {
protected:
	using Quaternion = DirectX::SimpleMath::Quaternion;
	using Vector2 = DirectX::SimpleMath::Vector2; 
	using Vector3 = DirectX::SimpleMath::Vector3; 
	using Vector4 = DirectX::SimpleMath::Vector4; 
	using Matrix  = DirectX::SimpleMath::Matrix;

public:
												GameObject(const std::string& name, const Vector3& position, std::string modelPath = "", float scale = 1.0f);
												GameObject(const std::string& name);
	virtual										~GameObject() {}

	virtual void								Initialise();
	virtual void								Update(float dt);
	virtual void								Draw(IDevCon* context, const Matrix& parentTransfom);

	void										Rescale(float dScale);
	void										Translate(const Vector3& dPos);
	void										Rotate(const Vector3& dRot, float angle);
	void										Rotate(float dRoll, float dPitch, float dYaw, float angle);
	Matrix										GetWorld()const;

	std::vector<GameObject*>					GetIntersected(GameObject* obj);
	virtual bool								Intersects(GameObject* other);
	virtual DirectX::BoundingBox&				GetBoundingBox() { return m_boundingBox; }

	Model&										GetModel() { return m_model; }
	const std::string&							GetName() { return m_name; }

	void										Reparent(const std::string& name, GameObject* pNewParent);
	void										AddChild(std::unique_ptr<GameObject>&& child);
	void										RemoveChild(const std::string& name);
	GameObject*									Find(const std::string& name);

protected:
	virtual void								CreateBoundingBox();

protected:
	std::string									m_name;
	DirectX::BoundingBox						m_boundingBox;
	Model										m_model;

	std::vector<std::unique_ptr<GameObject>>	m_children;

public:
	float										Scale;
	Vector3										Position;
	Quaternion									Rotation;

};

#endif //_GEOMETY_OBJECT_H_
