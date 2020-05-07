#include "pch.h"
#include "Camera.h"
#include "../Math/SimpleMath.h"
#include "../Environment.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

Camera::Camera(const std::string& name)
	: GameObject(name)
	, m_target(0.0f, 1.0f, 0.0f)

	, m_fov(XM_PIDIV4)
	, m_nearZ(0.1f)
	, m_farZ(1000.0f)
	
	, m_radius(50.0f)
	, m_theta(XM_PIDIV2)
	, m_phi(XM_PI/2.25f)
{

}

void Camera::SetTarget(const Vector3& target) {
	m_target = target;
}

Matrix Camera::GetView()const {
	float sinphi	= sinf(m_phi);
	float costheta	= cosf(m_theta);
	float cosphi	= cosf(m_phi);
	float sintheta	= sinf(m_theta);

	float x = m_target.x + m_radius * sinphi * costheta;
	float y = m_target.y + m_radius * cosphi;
	float z = m_target.z + m_radius * sinphi * sintheta;

	return Matrix::CreateLookAt({ x,y,z }, m_target, Vector3::Up);
}

Matrix Camera::GetProj()const {
	float aspectRatio = static_cast<float>(gEnv.Width) / static_cast<float>(gEnv.Height);
	return Matrix::CreatePerspectiveFieldOfView(m_fov, aspectRatio, m_nearZ, m_farZ);
}

Vector3 Camera::GetPosition()const {
	float sinphi = sinf(m_phi);
	float costheta = cosf(m_theta);
	float cosphi = cosf(m_phi);
	float sintheta = sinf(m_theta);

	return Vector3(m_target.x + m_radius * sinphi * costheta, m_target.y + m_radius * cosphi, m_target.z + m_radius * sinphi * sintheta);
}

void Camera::Translate(float d) {
	m_target.y += d;
}

void Camera::Zoom(float d) {
	m_radius += d;
}

void Camera::Rotate(float angleX, float angleY) {
	m_theta	+= angleX;
	m_phi	+= angleY;
	m_phi	= std::clamp(m_phi, 0.01f, XM_PI - 0.01f);
}
