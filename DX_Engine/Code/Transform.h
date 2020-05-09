#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

#pragma once
#include "Math/SimpleMath.h"

class Transform {
private:
	typedef DirectX::SimpleMath::Matrix Matrix;
	typedef DirectX::SimpleMath::Vector3 Vector3;
	typedef DirectX::SimpleMath::Quaternion Quaternion;

public:
					Transform();
					Transform(const Vector3&, const Quaternion&, const Vector3&);

					~Transform();

					Transform(Transform const&) = default;
					Transform& operator=(Transform const&) = default;
					Transform(Transform&&) = default;
					Transform& operator=(Transform&&) = default;
	
	Matrix			ToMatrix()const;

	void			Move(const Vector3& pos);
	void			Orbit(const Quaternion& rot);
	void			Orbit(const Quaternion& rot, const Vector3& target);
	//void			Scale(const Vector3& scale);

	Vector3&		Position() { return m_position; }
	Quaternion&		Rotation() { return m_rotation; }
	Vector3&		Scale() { return m_scale; }
	Matrix&			WorldTransform() { return m_transform; }

private:
	Vector3			m_position;
	Vector3			m_scale;
	Quaternion		m_rotation;
	Matrix			m_transform;

};

#endif //_TRANSFORM_H_
