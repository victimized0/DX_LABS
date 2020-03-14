#include "pch.h"
#include "Paddle.h"
#include "../Game.h"

Paddle::Paddle(const std::string& name, XMFLOAT3 position)
	: GeometryObject(name, position)
	, m_sizeX(0)
	, m_sizeY(0)
	, m_bb(0, 0, 0, 0)
{

}

Paddle::Paddle(const std::string& name)
	: Paddle(name, XMFLOAT3(0, 0, 0))
{

}

void Paddle::Update(float dt) {
	UpdateBB();
}

void Paddle::UpdateBB() {
	float sizeXDiv2 = (float)m_sizeX / 2;
	float sizeYDiv2 = (float)m_sizeY / 2;

	m_bb.Left	= GetPosition().x + sizeXDiv2;
	m_bb.Right	= GetPosition().x - sizeXDiv2;
	m_bb.Top	= GetPosition().y + sizeXDiv2;
	m_bb.Bottom	= GetPosition().y - sizeXDiv2;
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

bool Paddle::Collides(BoundingBox objBB, float objSizeX, float objSizeY, float threshold) {
	bool collidesLeft	= fabs(objBB.Left - m_bb.Right) < threshold;
	bool collidesRight	= fabs(objBB.Right - m_bb.Left) < threshold;

	bool collidesHeight	= (objBB.Bottom < (m_bb.Top + objSizeY)
					  && ((objBB.Top    - m_bb.Top) < objSizeY))
					  && ((objBB.Bottom + 2 * objSizeY) >= m_bb.Bottom);

	return (collidesLeft && collidesHeight || collidesRight && collidesHeight);
}
