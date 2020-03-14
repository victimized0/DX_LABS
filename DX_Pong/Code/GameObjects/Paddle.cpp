#include "pch.h"
#include "Paddle.h"
#include "../Game.h"

Paddle::Paddle(const std::string& name, XMFLOAT3 position)
	: GeometryObject(name, position)
	, m_pBall(nullptr)
	, m_sizeX(0)
	, m_sizeY(0)
{

}

Paddle::Paddle(const std::string& name)
	: Paddle(name, XMFLOAT3(0,0,0))
{

}

void Paddle::Update(float dt) {
	if (m_pBall == nullptr) return;

	float ballSizeDiv2			= (float)m_pBall->GetSize() / 2;
	float ballLeft				= m_pBall->GetPosition().x - ballSizeDiv2;
	float ballRight				= m_pBall->GetPosition().x + ballSizeDiv2;
	float ballBottom			= m_pBall->GetPosition().y - ballSizeDiv2;
	float ballTop				= m_pBall->GetPosition().y + ballSizeDiv2;

	float paddleRight			= GetPosition().x - (float)m_sizeX / 2;
	float paddleLeft			= GetPosition().x + (float)m_sizeX / 2;
	float paddleBottom			= GetPosition().y - (float)m_sizeY / 2;
	float paddleTop				= GetPosition().y + (float)m_sizeY / 2;

	bool collidesLeftPaddle		= fabs(ballLeft - paddleRight) < ballSizeDiv2 + dt;
	bool collidesRightPaddle	= fabs(ballRight - paddleLeft) < ballSizeDiv2 + dt;

	bool collidesHeight			= (ballBottom < (paddleTop + m_pBall->GetSize() / 2)
								&& ((ballTop - paddleTop) < m_pBall->GetSize() / 2))
								&& ((ballBottom + m_pBall->GetSize()) >= paddleBottom);

	if (collidesLeftPaddle  && collidesHeight ||
		collidesRightPaddle && collidesHeight)
	{
		m_pBall->SetSpeedX(m_pBall->GetSpeedX() * -1);
	}

	//if (collidesLeftPaddle  && !collidesHeight ||
	//	collidesRightPaddle && !collidesHeight)
	//{
	//	m_pBall->Reset();
	//}
}

void Paddle::SetBall(Ball* pBall) {
	m_pBall = pBall;
}

void Paddle::SetSize(int x, int y) {
	m_sizeX = x;
	m_sizeY = y;
}

int Paddle::GetSizeX() {
	return m_sizeX;
}

int Paddle::GetSizeY() {
	return m_sizeY;
}
