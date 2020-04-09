#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

#pragma once
#include "Math/SimpleMath.h"

class Transform {
public:
										Transform();
										~Transform();

										Transform(const DirectX::SimpleMath::Vector3&, const DirectX::SimpleMath::Quaternion&, const DirectX::SimpleMath::Vector3&);

										Transform(Transform const&) = default;
										Transform& operator=(Transform const&) = default;
										Transform(Transform&&) = default;
										Transform& operator=(Transform&&) = default;

	DirectX::SimpleMath::Vector3		GetPosition()const { return m_location; }
	DirectX::SimpleMath::Quaternion		GetRotation()const { return m_rotation; }
	DirectX::SimpleMath::Vector3		GetScale()const { return m_scale; }

	void								SetTransform(const DirectX::SimpleMath::Vector3&, const DirectX::SimpleMath::Quaternion&, const DirectX::SimpleMath::Vector3&);
	void								SetPosition(const DirectX::SimpleMath::Vector3& pos) { m_location = pos; }
	void								SetRotation(const DirectX::SimpleMath::Quaternion& rot) { m_rotation = rot; }
	void								SetScale(const DirectX::SimpleMath::Vector3& scale) { m_scale = scale; }
	
	DirectX::SimpleMath::Matrix			ToMatrix()const;

	void								Translate(const DirectX::SimpleMath::Vector3& pos);
	void								Rotate(const DirectX::SimpleMath::Quaternion& rot);
	void								Scale(const DirectX::SimpleMath::Vector3& scale);
	void								Orbit(float angle, const DirectX::SimpleMath::Vector3& pos);

private:
	//void								Update();

private:
	DirectX::SimpleMath::Vector3		m_location;
	DirectX::SimpleMath::Vector3		m_scale;
	DirectX::SimpleMath::Quaternion		m_rotation;
	DirectX::SimpleMath::Matrix			m_transform;

};

#endif //_TRANSFORM_H_
