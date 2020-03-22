#ifndef CAMERA_H
#define CAMERA_H

#include "SceneObject.h"
using namespace DirectX;

class Camera : public SceneObject {
public:
				Camera(const std::string& name);
	virtual		~Camera();

	void		SetLookAt(FXMVECTOR& eyePosition, FXMVECTOR& target, FXMVECTOR& up);
	void		SetProj(float fov, uint32_t width, uint32_t height, float nearZ, float farZ);
	void		SetTarget(float x, float y, float z);
	//void		SetPosition(float x, float y, float z);

	void		Update(float dt);
	void		Move(float d);
	void		Zoom(float d);
	void		Rotate(float angleX, float angleY);
	void		ResetPosition();

	void		SetRadius(float radius) { m_radius = radius; }

	XMMATRIX	GetView()const;
	XMMATRIX	GetProj()const;
	
	//XMVECTOR	GetPositionV()const;
	//XMFLOAT3	GetPosition()const;
	XMFLOAT3	GetTarget()const;
	XMVECTOR	GetTargetV()const;

private:
	XMFLOAT4X4	m_view;
	XMFLOAT4X4	m_proj;

	uint32_t	m_width;
	uint32_t	m_height;

	XMFLOAT3	m_target;
	//XMFLOAT3	m_pos;

	float		m_fov;
	float		m_nearZ;
	float		m_farZ;
	float		m_radius;
	float		m_theta;
	float		m_phi;

};

#endif // !CAMERA_H