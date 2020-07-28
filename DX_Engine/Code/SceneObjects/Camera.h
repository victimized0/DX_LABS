#ifndef _CAMERA_H_
#define _CAMERA_H_
#pragma once

#include "GameObject.h"

class Camera : public GameObject {
private:
	typedef DirectX::SimpleMath::Vector3 Vector3;
	typedef DirectX::SimpleMath::Matrix Matrix;

public:
						Camera(const std::string& name);
	virtual				~Camera() = default;

	void				Initialise()override;
	void				Update(float dt)override;
	virtual void		Move(float d);
	virtual void		Rotate(float angleX, float angleY);
	virtual void		Zoom(float d);

	virtual Matrix		GetView()const;
	virtual Matrix		GetProj(int width, int height)const;
	virtual Vector3		GetPosition()const;

protected:
	float				m_nearZ;
	float				m_farZ;
	float				m_theta;
	float				m_phi;

public:
	float				FoV;
	Vector3				Target;
	float				Radius;

};

#endif //_CAMERA_H_