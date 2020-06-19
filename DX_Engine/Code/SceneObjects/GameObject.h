#ifndef _GAME_OBJECT_H_
#define _GAME_OBJECT_H_

#include "../Model.h"
#include "../Math/SimpleMath.h"

class GameObject {
protected:
	typedef DirectX::SimpleMath::Quaternion		Quaternion;
	typedef DirectX::SimpleMath::Vector3		Vector3;
	typedef DirectX::SimpleMath::Matrix			Matrix;

public:
												GameObject(const std::string& name, const Vector3& position = Vector3::Zero, const std::string& modelPath = "", float scale = 1.0f, const Quaternion& rot = Quaternion::Identity);
	virtual										~GameObject()								= default;

												GameObject(GameObject const&)				= default;
												GameObject& operator=(GameObject const&)	= default;
												GameObject(GameObject&&)					= default;
												GameObject& operator=(GameObject&&)			= default;

	virtual void								Initialise();
	virtual void								Update(float dt);
	virtual void								Draw(IDevCon* context, const Matrix& parentTransfom = Matrix::Identity, RenderPass pass = RenderPass::Geometry);

	void										Rescale(float dScale);
	void										Translate(const Vector3& dPos);
	void										Rotate(const Vector3& dRot, float angle);
	void										Rotate(float dRoll, float dPitch, float dYaw, float angle);

	Matrix										GetWorld()const;
	void										SetTransform(Matrix& mat);

	std::vector<GameObject*>					GetIntersected(GameObject* obj);
	virtual bool								Intersects(GameObject* other);
	virtual DirectX::BoundingBox&				GetBoundingBox() { return m_boundingBox; }

	Model&										GetModel() { return m_model; }
	const std::string&							GetName() { return m_name; }
	bool										IsInit() { return m_isInit; }

	void										Reparent(const std::string& name, GameObject* pNewParent);
	void										AddChild(std::unique_ptr<GameObject>&& child);
	void										RemoveChild(const std::string& name);
	GameObject*									Find(const std::string& name);

protected:
	virtual void								CreateBoundingBox();

private:
	bool										m_isInit;

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
