#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "GameObject.h"

class Camera : public GameObject {
private:
	typedef DirectX::SimpleMath::Vector3 Vector3;
	typedef DirectX::SimpleMath::Matrix Matrix;

public:
						Camera(const std::string& name);
	virtual				~Camera() = default;

	virtual void		Move(float d);
	virtual void		Orbit(float angleX, float angleY);
	virtual void		Zoom(float d);

	virtual Matrix		GetView()const;
	virtual Matrix		GetProj()const;
	virtual Vector3		GetPosition()const;

protected:
	float				m_fov;
	float				m_nearZ;
	float				m_farZ;
	float				m_theta;
	float				m_phi;

public:
	Vector3				Target;
	float				Radius;

};

#endif //_CAMERA_H_