#include "pch.h"
#include "Transform.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

Transform::Transform()
	: Transform(Vector3::Zero, Quaternion::Identity, Vector3::One)
{

}

Transform::Transform(const Vector3& pos, const Quaternion& rot, const Vector3& scale)
	: m_position(pos)
	, m_rotation(rot)
	, m_scale(scale)
{
	m_transform = Matrix::CreateScale(scale) * Matrix::CreateFromQuaternion(rot) * Matrix::CreateTranslation(pos);
}

Transform::~Transform() {

}

Matrix Transform::ToMatrix()const {
	return m_transform;
}

void Transform::Translate(const Vector3& pos) {
	m_position += pos;
	m_transform.Translation(m_position);
}

void Transform::Rotate(const Quaternion& rot) {
	Translate(m_position);
	m_transform *= Matrix::CreateFromQuaternion(rot);
	m_rotation = Quaternion::CreateFromRotationMatrix(m_transform);
	Translate(-m_position);
}

void Transform::Rotate(const Quaternion& rot, const Vector3& target) {
	m_transform = Matrix::CreateLookAt(m_position, target, Vector3::Up);
	//Rotate(rot);
}

//void Transform::Scale(const Vector3& scale) {
//	m_scale += scale;
//	m_transform = XMMatrixAffineTransformation(m_scale, g_XMZero, g_XMZero, m_location);
//}
