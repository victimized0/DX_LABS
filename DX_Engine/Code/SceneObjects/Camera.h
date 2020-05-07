#ifndef CAMERA_H
#define CAMERA_H

#include "GameObject.h"
using namespace DirectX;

class Camera : public GameObject {
public:
											Camera(const std::string& name);
	virtual									~Camera() = default;

	virtual void							Translate(float d);
	virtual void							Rotate(float angleX, float angleY);
	void									Zoom(float d);

	void									SetRadius(float radius) { m_radius = radius; }
	void									SetTarget(const DirectX::SimpleMath::Vector3& target);

	virtual DirectX::SimpleMath::Matrix		GetView()const;
	virtual DirectX::SimpleMath::Matrix		GetProj()const;
	virtual DirectX::SimpleMath::Vector3	GetTarget()const { return m_target; }
	virtual DirectX::SimpleMath::Vector3	GetPosition()const;

protected:
	DirectX::SimpleMath::Vector3			m_target;

	float									m_fov;
	float									m_nearZ;
	float									m_farZ;
	float									m_radius;
	float									m_theta;
	float									m_phi;

};

#endif // !CAMERA_H