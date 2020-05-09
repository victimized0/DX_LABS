#include "pch.h"
#include "Camera.h"

#include "../Math/SimpleMath.h"
#include "../Environment.h"

using namespace DirectX::SimpleMath;

Camera::Camera(const std::string& name)
	: GameObject(name)
	, Target(0.0f, 1.0f, 0.0f)

	, m_fov(XM_PIDIV4)
	, m_nearZ(0.1f)
	, m_farZ(1000.0f)
	
	, Radius(50.0f)
	, m_theta(XM_PIDIV2)
	, m_phi(XM_PI/2.25f)
{

}

Matrix Camera::GetView()const {
	return Matrix::CreateLookAt(GetPosition(), Target, Vector3::Up);
}

Matrix Camera::GetProj()const {
	float aspectRatio = static_cast<float>(gEnv.Width) / static_cast<float>(gEnv.Height);
	return Matrix::CreatePerspectiveFieldOfView(m_fov, aspectRatio, m_nearZ, m_farZ);
}

Vector3 Camera::GetPosition()const {
	float sinphi	= sinf(m_phi);
	float costheta	= cosf(m_theta);
	float cosphi	= cosf(m_phi);
	float sintheta	= sinf(m_theta);

	return Vector3(Target.x + Radius * sinphi * costheta, Target.y + Radius * cosphi, Target.z + Radius * sinphi * sintheta);
}

void Camera::Move(float d) {
	Target.y += d;
}

void Camera::Zoom(float d) {
	Radius += d;
}

void Camera::Orbit(float angleX, float angleY) {
	m_theta	+= angleX;
	m_phi	+= angleY;
	m_phi	= std::clamp(m_phi, 0.01f, XM_PI - 0.01f);
}
