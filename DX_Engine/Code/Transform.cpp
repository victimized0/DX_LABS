#include "pch.h"
#include "Transform.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

Transform::Transform()
	: m_location(Vector3::Zero)
	, m_rotation(Quaternion::Identity)
	, m_scale(Vector3::One)
{

}

Transform::Transform(const Vector3& pos, const Quaternion& rot, const Vector3& scale)
	: m_location(pos)
	, m_rotation(rot)
	, m_scale(scale)
{

}

Transform::~Transform() {

}

void Transform::SetTransform(const Vector3& pos, const Quaternion& rot, const Vector3& scale) {
	m_location = pos;
	m_rotation = rot;
	m_scale = scale;
	m_transform = Matrix::CreateTranslation(pos) * Matrix::CreateFromQuaternion(rot) * Matrix::CreateScale(scale);
}

Matrix Transform::ToMatrix()const {
	return m_transform;
}

void Transform::Translate(const Vector3& pos) {
	m_location += pos;
	m_transform = Matrix::CreateScale(m_scale) * Matrix::CreateFromQuaternion(m_rotation) * Matrix::CreateTranslation(m_location);
}

void Transform::Rotate(const Quaternion& rot) {
	m_rotation *= rot;
	m_transform = XMMatrixAffineTransformation(m_scale, g_XMZero, m_rotation, m_location);
}

void Transform::Orbit(float angle, const Vector3& target) {
	if (angle == 0) return;
	m_rotation *= Quaternion::CreateFromAxisAngle(Vector3::Up, angle);
	m_transform = XMMatrixAffineTransformation(m_scale, target, m_rotation, m_location - target);
}

void Transform::Scale(const Vector3& scale) {
	m_scale += scale;
	m_transform = XMMatrixAffineTransformation(m_scale, g_XMZero, g_XMZero, m_location);
}
