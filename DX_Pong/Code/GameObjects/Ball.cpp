#include "pch.h"
#include "Ball.h"
#include "../Game.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

Ball::Ball(const std::string& name, const DirectX::SimpleMath::Vector3& m_position)
	: GameObject(name, m_position)
	, m_speedX(DEFAULT_SPEED_X)
	, m_speedY(DEFAULT_SPEED_Y)
{

}

Ball::Ball(const std::string& name)
	: Ball(name, Vector3::Zero)
{

}

float Ball::GetSpeedX() {
	return m_speedX;
}

float Ball::GetSpeedY() {
	return m_speedY;
}

void Ball::SetSpeedX(float value) {
	m_speedX = value;
}

void Ball::SetSpeedY(float value) {
	m_speedY = value;
}

void Ball::InverseSpeedX() {
	m_speedX *= -1;
}

void Ball::InverseSpeedY() {
	m_speedY *= -1;
}

void Ball::Reset() {
	Position = DirectX::SimpleMath::Vector3::Zero;

	m_speedX = DEFAULT_SPEED_X;
	m_speedY = DEFAULT_SPEED_Y;
}
